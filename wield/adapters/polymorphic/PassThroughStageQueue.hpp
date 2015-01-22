#pragma once
#include <wield/adapters/polymorphic/QueueInterface.hpp>
#include <wield/MessageBase.hpp>

namespace wield { namespace adapters { namespace polymorphic {
  
    // A pass-through stage is a stage that does not queue a message
    // but processes it immediately upon dispatch.
    //
    // This can be used to easily combine stages into aggregate
    // stages without having to rework the application logic.
    //
    // This is convenient while experimenting with work breakout
    // during development and performance testing.
    //
    // I hope to eventually provide tools for doing pass-through
    // stage combination at compile time (without requiring client
    // code to introduce virtual function calls for queue functionality)
    // but that is a long way off I think.
    template<class ProcessingFunctor>
    class PassThroughStageQueue : public QueueInterface<ProcessingFunctor>
    {
    public:
        using MessagePtr = typename MessageBase<ProcessingFunctor>::smartptr;
        
        PassThroughStageQueue(ProcessingFunctor& pf)
            : processingFunctor_(pf)
        {
        }
        
        // called in the same thread as the stage invoking dispatch to
        // the stage owning this queue (the previous stage in the
        // stage graph).
        void push(const MessagePtr& message) override
        {
            // process the message immediately.
            message->processWith(processingFunctor_);
        }
        
        bool try_pop(MessagePtr& message) override { return false; }
        std::size_t unsafe_size(void) const override { return 0; }

    private:
        ProcessingFunctor& processingFunctor_;
    };
}}}