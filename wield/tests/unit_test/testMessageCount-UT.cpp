#include "./platform/UnitTestSupport.hpp"

#include <wield/schedulers/utils/MessageCount.hpp>
#include "./test/Stages.hpp"

namespace {

    struct MessageCountFixture
    {
        wield::schedulers::utils::MessageCount<test::Stages> stats;
    };

    TEST_FIXTURE(MessageCountFixture, verifyInstantiationOfMessageCount)
    {
        using namespace test;

        CHECK_EQUAL(0U, stats.estimatedDepth(Stages::Stage1));
        CHECK_EQUAL(0U, stats.estimatedDepth(Stages::Stage2));
        CHECK_EQUAL(0U, stats.estimatedDepth(Stages::Stage3));
    }

    TEST_FIXTURE(MessageCountFixture, verifyIncrement)
    {
        using namespace test;

        stats.increment(Stages::Stage1);
        CHECK_EQUAL(1U, stats.estimatedDepth(Stages::Stage1));

        stats.increment(Stages::Stage2, 5);
        CHECK_EQUAL(5U, stats.estimatedDepth(Stages::Stage2));
    }

    TEST_FIXTURE(MessageCountFixture, verifyUpdatePrevious)
    {
        using namespace test;

        stats.increment(Stages::Stage1);
        CHECK_EQUAL(1U, stats.estimatedDepth(Stages::Stage1));

        stats.updatePrevious(Stages::Stage1);
        CHECK_EQUAL(0U, stats.estimatedDepth(Stages::Stage1));
    }

    TEST_FIXTURE(MessageCountFixture, verifyHighWaterStageIsFirstStageWhenThereIsNoWork)
    {
        using namespace test;

        Stages stage = stats.highwaterStage();
        CHECK_EQUAL(Stages::Stage1, stage);
    }

    TEST_FIXTURE(MessageCountFixture, verifyHighWaterStage)
    {
        using namespace test;

        stats.increment(Stages::Stage1, 10);
        stats.increment(Stages::Stage2, 100);
        stats.increment(Stages::Stage3, 200);

        CHECK_EQUAL(Stages::Stage3, stats.highwaterStage());
    }

    TEST_FIXTURE(MessageCountFixture, verifyReset)
    {
        using namespace test;

        stats.increment(Stages::Stage1, 10);
        CHECK_EQUAL(10U, stats.estimatedDepth(Stages::Stage1));

        stats.reset();

        CHECK_EQUAL(0U, stats.estimatedDepth(Stages::Stage1));
    }
}
