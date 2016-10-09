#pragma once
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>
#include <wield/Traits.hpp>

#include "../platform/ConcurrentQueue.hpp"

#include "./Scheduler.hpp"
#include "./Stages.hpp"

namespace test_batch {

    // forward declare the ProcessingFunctor type
    class ProcessingFunctorInterface;

    // Required declarations:
    //      StageEnumType     | enum of stage names
    //      ProcessingFunctor | your processing functor abstract base class.
    //      QueueType         | the queue (or adapter) you wish to use.
    //      SchedulingPolicy  | your scheduling policy. must take dispatcher and polling policy as parameters.
    struct TestTraits
    {
        using StageEnumType = Stages;
        using ProcessingFunctor = ProcessingFunctorInterface;
        
        template<typename MessagePtrType>
        using QueueType = Concurrency::concurrent_queue<MessagePtrType>;

        using PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<StageEnumType>;
        
        template<typename Dispatcher>
        using SchedulingPolicy = SchedulingPolicy<Dispatcher, PollingPolicy>;
    };
    
    using Traits = wield::Traits<TestTraits>;
}
