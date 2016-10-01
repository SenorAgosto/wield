#include "./platform/UnitTestSupport.hpp"
#include <wield/schedulers/RandomVisit.hpp>
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include "./test/Stages.hpp"
#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"

namespace {

    struct RandomVisitFixture
    {
        RandomVisitFixture()
            : schedulingPolicy(dispatcher)
        {
        }

        static const std::size_t TableSize = 8;
        
        using Dispatcher = test::Traits::Dispatcher;
        using Stage = test::Traits::Stage;
        using PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<test::Stages>;
        using SchedulingPolicy = wield::schedulers::RandomVisit<test::Stages, Dispatcher, Stage, PollingPolicy, TableSize>;
        using Queue = test::Traits::Queue;

        Dispatcher dispatcher;
        SchedulingPolicy schedulingPolicy;
    };

    TEST_FIXTURE(RandomVisitFixture, verifyInstantiationOfRandomVisit)
    {
    }

    TEST_FIXTURE(RandomVisitFixture, verifyRandomVisitOrdering)
    {
        test::ProcessingFunctor pf;
        Queue q;

        Stage s1(test::Stages::Stage1, dispatcher, q, pf);
        Stage s2(test::Stages::Stage2, dispatcher, q, pf);
        Stage s3(test::Stages::Stage3, dispatcher, q, pf);

        // the order is random, so we can't validate it here,
        // but make some calls to smoke test the code anyway.
        schedulingPolicy.nextStage(0);
        schedulingPolicy.nextStage(0);
        schedulingPolicy.nextStage(0);
        schedulingPolicy.nextStage(0);
        schedulingPolicy.nextStage(0);
    }

    TEST_FIXTURE(RandomVisitFixture, verifyCanBeUsedInSchedulerBase)
    {
        using Scheduler = wield::SchedulerBase<SchedulingPolicy>;
        Scheduler scheduler(dispatcher);
    }
}
