#pragma once
#include <wield/MessageBase.h>

namespace wield {

    template<typename StageEnum, class ProcessingFunctor, class QueueType, template<typename StageEnum2, typename Stage> class DispatcherType>
    class StageBase final
    {
    public:
        static_assert(std::is_enum<StageEnum>::value, "StageEnum parameter is not an enum type.");
        using MessageType = MessageBase<ProcessingFunctor>;

        StageBase(StageEnum stageName, DispatcherType<StageEnum, StageBase>& dispatcher, QueueType& queue, ProcessingFunctor& processingFunctor)
            : processingFunctor_(processingFunctor)
            , queue_(queue)
            , stageName_(stageName)
        {
            dispatcher.registerStage(stageName, this);
        }

        ~StageBase()
        {
        }

        void push(const typename MessageType::smartptr& m)
        {
            queue_.push(m);
        }

        bool process(void)
        {
            typename MessageType::smartptr m;
            if( queue_.try_pop(m) )
            {
                m->processWith(processingFunctor_);
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
