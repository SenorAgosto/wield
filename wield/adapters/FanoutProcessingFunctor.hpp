#pragma once
#include <wield/CloneMessageTag.hpp>

#include <array>
#include <utility>

namespace wield { namespace adapters {
    
    // An abstract base class which implements logic necessary to
    // fanout a message to multiple downstream stages. With this
    // version, the stage fanout list is determined at compile-time.
    //
    // @Dispatcher is the dispatcher implementation,
    // @ProcessingFunctorInterface is the base class for processing functors,
    // and @StageList is a list of StageEnums which messages should be dispatched to.
    template<class Dispatcher, class ProcessingFunctorInterface, typename Dispatcher::StageEnumType... StageList>
    class FanoutProcessingFunctor : public ProcessingFunctorInterface
    {
    public:
        using StageEnumType = typename Dispatcher::StageEnumType;
        using Message = typename ProcessingFunctorInterface::Message;
        
        FanoutProcessingFunctor(Dispatcher& dispatcher);
        
        // NOTE: use this dispatch() method instead of Dispatcher::dispatch() in
        // your concrete processing functor.
        void dispatch(Message& m);
        
        // NOTE: use this dispatch() method instead of Dispatcher::dispatch() in
        // your concrete processing functor.
        template<class ConcreteMessageType>
        void dispatch(ConcreteMessageType& message, CloneMessageTagType);
        
    private:
        std::array<StageEnumType, sizeof...(StageList)> stages_;
        Dispatcher& dispatcher_;
    };
    

    template<class Dispatcher, class ProcessingFunctorInterface, typename Dispatcher::StageEnumType... StageList>
    FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface, StageList...>::FanoutProcessingFunctor(Dispatcher& dispatcher)
        : stages_({{StageList...}})
        , dispatcher_(dispatcher)
    {
    }

    template<class Dispatcher, class ProcessingFunctorInterface, typename Dispatcher::StageEnumType... StageList>
    inline
    void FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface, StageList...>::dispatch(Message& m)
    {
        std::for_each(begin(stages_), end(stages_), [this, &m](const StageEnumType stage){
            this->dispatcher_.dispatch(stage, m);
        });
    }
    
    template<class Dispatcher, class ProcessingFunctorInterface, typename Dispatcher::StageEnumType... StageList>
    template<class ConcreteMessageType>
    inline
    void FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface, StageList...>::dispatch(ConcreteMessageType& m, CloneMessageTagType)
    {
        std::for_each(begin(stages_), end(stages_), [this, &m](const StageEnumType stage){
            this->dispatcher_.dispatch(stage, m, clone_message);
        });
    }

}}
