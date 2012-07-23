#pragma once

namespace wield
{
    template<typename ProcessingFunctor, typename Dispatcher, typename QueueType, typename StageEnum>
    class StageBase
    {
    public:
        StageBase(StageEnum stageName, Dispatcher& dispatcher, QueueType& queue, ProcessingFunctor& processingFunctor )
            : processingFunctor_(processingFunctor)
            , queue_(queue)
        {
            //dispatcher.registerStage(stageName, *this);
        }

        ~StageBase()
        {
        }

    private:
        ProcessingFunctor& processingFunctor_;
        QueueType& queue_;
    };
}
