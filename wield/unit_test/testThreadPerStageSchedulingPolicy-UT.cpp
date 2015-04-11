#include "./platform/UnitTestSupport.hpp"
#include <wield/schedulers/ThreadPerStage.hpp>

#include "./test/Stages.hpp"
#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"

namespace {

    struct ThreadPerStageFixture
    {
        ThreadPerStageFixture()
            : schedulingPolicy(dispatcher)
        {
        }

        using Dispatcher = test::Traits::Dispatcher;
        using Stage = test::Traits::Stage;
        using SchedulingPolicy =
            wield::schedulers::ThreadPerStage<test::Stages, Dispatcher, Stage>;

        Dispatcher dispatcher;
        SchedulingPolicy schedulingPolicy;
    };

    TEST_FIXTURE(ThreadPerStageFixture, verifyInstantiationOfThreadPerStageScheduler)
    {
    }

    TEST_FIXTURE(ThreadPerStageFixture, verifyNumberOfThreadsIsCorrect)
    {
        std::size_t expectedThreads = static_cast<std::size_t>(test::Stages::NumberOfEntries);
        CHECK_EQUAL(expectedThreads, schedulingPolicy.numberOfThreads());
    }

    TEST_FIXTURE(ThreadPerStageFixture, verifyThreadsAreScheduledToCorrespondingStage)
    {
        using namespace test;
        using Queue = Traits::Queue;

        Queue q;
        ProcessingFunctor pf;

        Stage stage1(Stages::Stage1, dispatcher, q, pf);
        Stage stage2(Stages::Stage2, dispatcher, q, pf);
        Stage stage3(Stages::Stage3, dispatcher, q, pf);

        CHECK_EQUAL(&stage1, & schedulingPolicy.nextStage(0));
        CHECK_EQUAL(&stage2, & schedulingPolicy.nextStage(1));
        CHECK_EQUAL(&stage3, & schedulingPolicy.nextStage(2));
    }

    TEST_FIXTURE(ThreadPerStageFixture, verifyCanBeUsedBySchedulerBase)
    {
        using Scheduler = wield::SchedulerBase<SchedulingPolicy>;
        Scheduler scheduler(dispatcher);
    }
}