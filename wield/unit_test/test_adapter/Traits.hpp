#pragma once
#include <wield/adapters/polymorphic/PassThroughStageQueue.hpp>
#include <wield/adapters/polymorphic/QueueInterface.hpp>
#include <wield/policies/ExhaustivePollingPolicy.hpp>
#include <wield/Traits.hpp>

#include "Scheduler.hpp"
#include "Stages.hpp"

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
        using QueueType = wield::adapters::polymorphic::QueueInterface<MessagePtrType>;

        using PollingPolicy = wield::policies::ExhaustivePollingPolicy<StageEnumType>;
        
        template<typename Dispatcher>
        using SchedulingPolicy = SchedulingPolicy<Dispatcher, PollingPolicy>;
        
        // an etra field because I happen to want it for convenience
        template<typename MessagePtr>
        using PassThroughStageQueue = wield::adapters::polymorphic::PassThroughStageQueue<ProcessingFunctor, MessagePtr>;
    };
    
    using Traits = wield::Traits<TestTraits>;
}
