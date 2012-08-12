#pragma once

namespace wield
{
    template<typename StageEnum, typename ProcessingFunctor, typename QueueType, template<typename StageEnum, typename Stage> class Dispatcher>
    class StageBase
    {
    public:
        typedef MessageBase<ProcessingFunctor> Message;

        StageBase(StageEnum stageName, Dispatcher<StageEnum, StageBase>& dispatcher, QueueType& queue, ProcessingFunctor& processingFunctor )
            : processingFunctor_(processingFunctor)
            , queue_(queue)
        {
            dispatcher.registerStage(stageName, this);
        }

        ~StageBase()
        {
        }

        void push(const typename MessageBase<ProcessingFunctor>::smartptr& m)
        {
            queue_.push(m);
        }

        void process(void) const
        {
            MessageBase<ProcessingFunctor>::smartptr m;
            if( queue_.try_pop(m) )
            {
                m->ProcessWith(processingFunctor_);
            }
        }

    private:
        ProcessingFunctor& processingFunctor_;
        QueueType& queue_;
    };
}
