#pragma once 
#include <wield/Traits.hpp>
#include <wield/schedulers/ThreadPerStage.hpp>
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include <queue_stress/Stages.hpp>
#include <queue_stress/platform/ConcurrentQueue.hpp>

namespace queue_stress {

    class ProcessingFunctorBase;

    struct AppTraits 
    {
        using StageEnumType = Stages;
        using ProcessingFunctor = ProcessingFunctorBase;

        template<class MessagePtrType>
        using QueueType = Concurrency::concurrent_queue<MessagePtrType>;

        using PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<StageEnumType>;
        
        template<class Dispatcher>
        using SchedulingPolicy = wield::schedulers::ThreadPerStage<Dispatcher, PollingPolicy>;
    };

    using Traits = wield::Traits<AppTraits>;
}

