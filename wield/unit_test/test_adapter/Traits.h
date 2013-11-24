#pragma once
#include <wield/adapters/polymorphic/PassThroughStageQueue.h>
#include <wield/adapters/polymorphic/QueueInterface.h>
#include <wield/policies/ExhaustivePollingPolicy.h>
#include <wield/Traits.h>

#include "Scheduler.h"
#include "Stages.h"

namespace test_adapter {

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
        using QueueType = wield::adapters::polymorphic::QueueInterface<ProcessingFunctor>;

        using PollingPolicy = wield::policies::ExhaustivePollingPolicy<StageEnumType>;
        
        template<typename Dispatcher>
        using SchedulingPolicy = SchedulingPolicy<Dispatcher, PollingPolicy>;
        
        // an etra field because I happen to want it for convenience
        using PassThroughStageQueue = wield::adapters::polymorphic::PassThroughStageQueue<ProcessingFunctor>;
    };
    
    using Traits = wield::Traits<TestTraits>;
}
