#pragma once
#include <wield/MessageBase.h>
#include <wield/Concepts.h>

namespace wield {

    template<typename StageEnum, typename ProcessingFunctor, typename QueueType, template<typename StageEnum2, typename Stage> class Dispatcher>
    class StageBase final
        : private IsEnumConcept<StageEnum>
    {
    public:
        using message_t = MessageBase<ProcessingFunctor>;

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
            typename message_t::smartptr m;
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
        StageBase(const StageBase&) = delete;
        StageBase& operator=(const StageBase&) = delete;

    private:
        ProcessingFunctor& processingFunctor_;
        QueueType& queue_;

        StageEnum stageName_;
    };
}
