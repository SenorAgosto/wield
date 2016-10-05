#pragma once
#include <wield/CloneMessageTag.hpp>
#include <utility>
#include <vector>

namespace wield { namespace adapters { namespace dynamic {
    
    // An abstract base class which implements logic necessary to
    // fanout a message to multiple downstream stages. The stages
    // to fanout to determined at construction.
    template<class Dispatcher, class ProcessingFunctorInterface>
    class FanoutProcessingFunctor : public ProcessingFunctorInterface
    {
    public:
        using StageEnumType = typename Dispatcher::StageEnumType;
        using Message = typename ProcessingFunctorInterface::Message;
        
        FanoutProcessingFunctor(Dispatcher& dispatcher, const std::vector<StageEnumType>& stages);
        FanoutProcessingFunctor(Dispatcher& dispatcher, std::vector<StageEnumType>&& stages);
        
        // NOTE: use this dispatch() method instead of Dispatcher::dispatch() in
        // your concrete processing functor.
        void dispatch(Message& m);
        
        // NOTE: use this dispatch() method instead of Dispatcher::dispatch() in
        // your concrete processing functor.
        template<class ConcreteMessageType>
        void dispatch(ConcreteMessageType& message, CloneMessageTagType);
        
        // swap the current fanout configuration with @stages
        void updateStages(const std::vector<StageEnumType>& stages);
        void updateStages(std::vector<StageEnumType>&& stages);
        
    private:
        std::vector<StageEnumType> stages_;
        Dispatcher& dispatcher_;
    };
    
    
    template<class Dispatcher, class ProcessingFunctorInterface>
    FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::FanoutProcessingFunctor(Dispatcher& dispatcher, const std::vector<StageEnumType>& stages)
        : stages_(stages)
        , dispatcher_(dispatcher)
    {
    }
    
    template<class Dispatcher, class ProcessingFunctorInterface>
    FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::FanoutProcessingFunctor(Dispatcher& dispatcher, std::vector<StageEnumType>&& stages)
        : stages_(std::move(stages))
        , dispatcher_(dispatcher)
    {
    }
    
    template<class Dispatcher, class ProcessingFunctorInterface>
    inline
    void FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::dispatch(Message& m)
    {
        std::for_each(begin(stages_), end(stages_), [this, &m](const StageEnumType stage){
            this->dispatcher_.dispatch(stage, m);
        });
    }
    
    template<class Dispatcher, class ProcessingFunctorInterface>
    template<class ConcreteMessageType>
    inline
    void FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::dispatch(ConcreteMessageType& m, CloneMessageTagType)
    {
        std::for_each(begin(stages_), end(stages_), [this, &m](const StageEnumType stage){
            this->dispatcher_.dispatch(stage, m, clone_message);
        });
    }
    
    template<class Dispatcher, class ProcessingFunctorInterface>
    inline
    void FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::updateStages(const std::vector<StageEnumType>& stages)
    {
        stages_ = stages;
    }
    
    template<class Dispatcher, class ProcessingFunctorInterface>
    inline
    void FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::updateStages(std::vector<StageEnumType>&& stages)
    {
        stages_.swap(stages);
    }
}}}

