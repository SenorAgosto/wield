#include "./platform/UnitTestSupport.hpp"
#include <wield/schedulers/RoundRobin.hpp>
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include "./test/Stages.hpp"
#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"

namespace {

    struct RoundRobinFixture
    {
        RoundRobinFixture()
            : schedulingPolicy(dispatcher)
        {
        }

        using Dispatcher = test::Traits::Dispatcher;
        using Stage = test::Traits::Stage;
        using SchedulingPolicy = wield::schedulers::RoundRobin<test::Stages, Dispatcher, Stage, wield::polling_policies::ExhaustivePollingPolicy<test::Stages>>;

        Dispatcher dispatcher;
        SchedulingPolicy schedulingPolicy;
    };

    TEST_FIXTURE(RoundRobinFixture, verifyInstantiationOfRoundRobin)
    {
    }

    TEST_FIXTURE(RoundRobinFixture, verifyRoundRobinOrdering)
    {
        CHECK_EQUAL(test::Stages::Stage1, schedulingPolicy.nextStage(0));
        CHECK_EQUAL(test::Stages::Stage2, schedulingPolicy.nextStage(0));
        CHECK_EQUAL(test::Stages::Stage3, schedulingPolicy.nextStage(0));
        CHECK_EQUAL(test::Stages::Stage1, schedulingPolicy.nextStage(0));
    }

    TEST_FIXTURE(RoundRobinFixture, verifyCanBeUsedInSchedulerBase)
    {
        using Scheduler = wield::SchedulerBase<SchedulingPolicy>;
        Scheduler scheduler(dispatcher);
    }
}
