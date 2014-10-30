#pragma once
#include <wield/adapters/polymorphic/QueueInterface.h>
#include <wield/MessageBase.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace wield { namespace adapters {
  
    // <ProcessingFunctorChain> is a pass-through queue used to chain multiple
    // processing functors together (to be handled by a single stage).
    //
    // An aggregate pass-through stage is a stage that does not queue a message
    // but processes it immediately upon dispatch & the processing involves
    // multiple processing functors.
    //
    // This is most useful as a tool to keep the logical aspects of processing a
    // message separated prior to actually separating the processing into different
    // stages. This is convenient while experimenting with work breakout
    // during development and performance testing.
    //
    // CAVEAT: the functors used should probably not dispatch the message to a
    // different stage (unless it is the last in the chain) as doing so can
    // introduce race conditions into your application.
    //
    template<class ProcessingFunctorType, std::size_t NumberOfProcessingFunctors>
    class ProcessingFunctorChain
        : public polymorphic::QueueInterface<ProcessingFunctorType>
    {
    public:
        using MessagePtr = typename MessageBase<ProcessingFunctorType>::smartptr;
        
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wmissing-braces"
        template<typename... Args>
        ProcessingFunctorChain(Args... args)
            : processingFunctors_ {args...}
        {
        }
        #pragma clang diagnostic pop
        
        // called in the same thread as the stage invoking dispatch to
        // the stage owning this queue (the previous stage in the
        // stage graph).
        void push(const MessagePtr& message) override
        {
            // process the message immediately with each of the ProcessingFunctors
            std::for_each(begin(processingFunctors_), end(processingFunctors_), [&message](ProcessingFunctorType* func)
            {
                message->processWith(*func);
            });
        }

        bool try_pop(MessagePtr& message) override { return false; }
        std::size_t unsafe_size(void) const override { return 0; }

    private:
        ProcessingFunctorChain() = delete;

    private:
        std::array<ProcessingFunctorType*, NumberOfProcessingFunctors> processingFunctors_;
    };


    // A helper to create ProcessingFunctorChain from list of processing functors.
    //     auto processorChain = CreateProcessorChain<PrcoessingFunctorBaseClass>(&fp, &fp2, &fp3);
    template<class ProcessingFunctor, class ...Args>
    ProcessingFunctorChain<ProcessingFunctor, sizeof...(Args)> CreateProcessorChain(Args... args)
    {
        return {args...};
    }
}}