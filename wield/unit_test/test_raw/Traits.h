#pragma once
#include <wield/DispatcherBase.h>
#include <wield/StageBase.h>
#include <wield/SchedulerBase.h>

#include <wield/policies/ExhaustivePollingPolicy.h>

#include "../platform/ConcurrentQueue.h"

#include "./Scheduler.h"
#include "./Stages.h"
#include "./TestRawPointerMessage.h"

namespace test_raw {

    // forward declare the ProcessingFunctor type
    class ProcessingFunctorInterface;

	// For this test, we want to demonstrate putting raw pointers 
    // into our queues. You'd do this for performance and when you
    // have a really clear picture of the lifetime of the messages
    // (needs to be deterministic). Determinism can be achieved 
    // in most cases by introducing a garbage collection stage. 
    struct TestTraits
    {
        using StageEnumType = Stages;
        using ProcessingFunctor = ProcessingFunctorInterface;

		using Message = TestRawPointerMessage<ProcessingFunctor>;
		using MessagePtr = typename Message::smartptr;

        using Queue = Concurrency::concurrent_queue<MessagePtr>;

        template<typename StageEnum, typename Stage>
        using DispatcherType = wield::DispatcherBase<StageEnum, Stage>;
        
        using Stage = wield::StageBase<StageEnumType, ProcessingFunctor, Message, Queue>;
        using Dispatcher = DispatcherType<StageEnumType, Stage>;

        using PollingPolicy = wield::policies::ExhaustivePollingPolicy<StageEnumType>;
        
        using SchedulingPolicy = SchedulingPolicy<Dispatcher, PollingPolicy>;
		using Scheduler = wield::SchedulerBase<SchedulingPolicy>;
        
        static_assert(std::is_same<Message, TestRawPointerMessage<ProcessingFunctor>>::value, "Message type is not the expected type.");
    };

	// here we are using our own traits definition, not using the 
	// wield::Traits<> convenience class template. We're doing this
	// because for this set of tests, we need a different message base
    // (raw pointers). 
	using Traits = TestTraits;
}
