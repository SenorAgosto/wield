#pragma once
#include <wield/MessageBase.h>

namespace wield {

    template<typename StageEnum, typename ProcessingFunctor, typename QueueType, template<typename StageEnum, typename Stage> class Dispatcher>
    class StageBase final
    {
    public:
        typedef MessageBase<ProcessingFunctor> message_t;

        StageBase(StageEnum stageName, Dispatcher<StageEnum, StageBase>& dispatcher, QueueType& queue, ProcessingFunctor& processingFunctor )
            : processingFunctor_(processingFunctor)
            , queue_(queue)
            , stageName_(stageName)
        {
            dispatcher.registerStage(stageName, this);
        }

        ~StageBase()
        {
        }

        void push(const typename message_t::smartptr& m)
        {
            queue_.push(m);
        }

        bool process(void)
        {
            message_t::smartptr m;
            if( queue_.try_pop(m) )
            {
                m->ProcessWith(processingFunctor_);
                return true;
            }

            return false;
        }

        inline StageEnum name(void) const
        {
            return stageName_;
        }

    private:
        ProcessingFunctor& processingFunctor_;
        QueueType& queue_;

        StageEnum stageName_;
    };
}
