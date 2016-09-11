#pragma once
#include <wield/DispatcherInterface.hpp>
#include <wield/MessageBase.hpp>

#include <wield/details/SmartPtrCreator.hpp>

namespace wield {

    /* Stage
       @StageEnum the enum defining names of all stages.
       @ProcessingFunctor this stage's processing function.
       @QueueType the type of queue this stage will get inputs from.
       @DispatcherType the type of the dispatcher the stage will use to dispatch messages to other stages.
     
       A stage is the basic unit of processing in a SEDA architecture. It consists of a queue, and a function
       to apply to messages in the queue. 
    */
    template<typename StageEnum, class ProcessingFunctor, class Message, class QueueType>
    class StageBase 
    {
    public:
        static_assert(std::is_enum<StageEnum>::value, "StageEnum parameter is not an enum type.");
        using MessageType = Message;
        
        StageBase(const StageEnum stageName, DispatcherInterface<StageEnum, StageBase>& dispatcher, QueueType& queue, ProcessingFunctor& processingFunctor);
        
        // make container friendly.
        StageBase(StageBase&& stage);

        // Insert a message onto the stage's queue
        // @m the message to insert
        void push(const typename MessageType::ptr& m);
        
        // process a message:
        // pump the queue, if there is a message, process it.
        // @return true if a message was processed, false otherwise.
        bool process(void);

        // get the stage's name
        StageEnum name(void) const;

    private:
        StageBase(const StageBase&) = delete;
        StageBase& operator=(const StageBase&) = delete;

    private:
        ProcessingFunctor& processingFunctor_;
        QueueType& queue_;

        const StageEnum stageName_;
    };
    
    
    template<typename StageEnum, class ProcessingFunctor, class Message, class QueueType>
    StageBase<StageEnum, ProcessingFunctor, Message, QueueType>::StageBase(const StageEnum stageName, DispatcherInterface<StageEnum, StageBase>& dispatcher, QueueType& queue, ProcessingFunctor& processingFunctor)
        : processingFunctor_(processingFunctor)
        , queue_(queue)
        , stageName_(stageName)
    {
        dispatcher.registerStage(stageName, this);
    }
    
    template<typename StageEnum, class ProcessingFunctor, class Message, class QueueType>
    StageBase<StageEnum, ProcessingFunctor, Message, QueueType>::StageBase(StageBase&& stage)
        : processingFunctor_(stage.processingFunctor_)
        , queue_(stage.queue_)
        , stageName_(stage.stageName_)
    {
    }

    template<typename StageEnum, class ProcessingFunctor, class Message, class QueueType>
    inline
    void StageBase<StageEnum, ProcessingFunctor, Message, QueueType>::push(const typename MessageType::ptr& m)
    {
        queue_.push(m);
    }
    
    template<typename StageEnum, class ProcessingFunctor, class Message, class QueueType>
    bool StageBase<StageEnum, ProcessingFunctor, Message, QueueType>::process(void)
    {
        typename MessageType::ptr m = nullptr;
        if(queue_.try_pop(m))
        {
            typename MessageType::smartptr message(details::create_smartptr<MessageType>(m, no_increment));

            message->processWith(processingFunctor_);
            return true;
        }

        return false;
    }

    template<typename StageEnum, class ProcessingFunctor, class Message, class QueueType>
    inline
    StageEnum StageBase<StageEnum, ProcessingFunctor, Message, QueueType>::name(void) const
    {
        return stageName_;
    }
}
