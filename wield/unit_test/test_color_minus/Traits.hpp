#pragma once
#include <wield/MessageBase.hpp>
#include <wield/StageBase.hpp>
#include <wield/SchedulerBase.hpp>

#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>
#include <wield/schedulers/color_minus/ColorMinus.hpp>
#include <wield/schedulers/color_minus/Dispatcher.hpp>

#include "../platform/ConcurrentQueue.hpp"

#include "./Scheduler.hpp"
#include "./Stages.hpp"

namespace test_color_minus {

    // forward declare the ProcessingFunctor type
    class ProcessingFunctorInterface;

    // For this test, we're using a different dispatcher.
    struct TestTraits
    {
        using StageEnumType = Stages;
        using ProcessingFunctor = ProcessingFunctorInterface;

		using Message = wield::MessageBase<ProcessingFunctor>;
		using MessagePtr = typename Message::smartptr;

        using Queue = Concurrency::concurrent_queue<MessagePtr>;

        template<typename StageEnum, typename Stage>
        using DispatcherType = wield::schedulers::color_minus::Dispatcher<StageEnum, Stage>;
        
        using Stage = wield::StageBase<StageEnumType, ProcessingFunctor, Message, Queue>;
        using Dispatcher = DispatcherType<StageEnumType, Stage>;

        using PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<StageEnumType>;
        using SchedulingPolicy = wield::schedulers::color_minus::ColorMinus<
              StageEnumType
            , Dispatcher
            , Stage
            , PollingPolicy>;
		using Scheduler = wield::SchedulerBase<SchedulingPolicy>;
    };

	// here we are using our own traits definition, not using the 
	// wield::Traits<> convenience class template. We're doing this
	// because for this set of tests, we need a different dispatcher.
	using Traits = TestTraits;
}
