#pragma once
#include <wield/policies/ExhaustivePollingPolicy.h>
#include <wield/Traits.h>

#include "../platform/ConcurrentQueue.h"

#include "Scheduler.h"
#include "Stages.h"

namespace test {

    // forward declare the ProcessingFunctor type
    class ProcessingFunctorInterface;

    // Required declarations:
    //      StageEnumType     | enum of stage names
    //      ProcessingFunctor | your processing functor abstract base class.
    //      QueueType         | the queue (or adapter) you wish to use.
    //      SchedulingPolicy  | your scheduling policy. must take dispatcher as sole template parameter.
    struct TestTraits
    {
        using StageEnumType = Stages;
        using ProcessingFunctor = ProcessingFunctorInterface;
        
        template<typename MessagePtrType>
        using QueueType = Concurrency::concurrent_queue<MessagePtrType>;

        using PollingPolicy = wield::policies::ExhaustivePollingPolicy<StageEnumType>;
        
        template<typename Dispatcher>
        using SchedulingPolicy = SchedulingPolicy<Dispatcher, PollingPolicy>;
    };
    
    using Traits = wield::Traits<TestTraits>;
}
