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

        CHECK_EQUAL(0, stats.estimatedDepth(Stages::Stage1));
        CHECK_EQUAL(0, stats.estimatedDepth(Stages::Stage2));
        CHECK_EQUAL(0, stats.estimatedDepth(Stages::Stage3));
    }

    TEST_FIXTURE(MessageCountFixture, verifyIncrement)
    {
        using namespace test;

        stats.increment(Stages::Stage1);
        CHECK_EQUAL(1, stats.estimatedDepth(Stages::Stage1));

        stats.increment(Stages::Stage2, 5);
        CHECK_EQUAL(5, stats.estimatedDepth(Stages::Stage2));
    }

    TEST_FIXTURE(MessageCountFixture, verifyUpdatePrevious)
    {
        using namespace test;

        stats.increment(Stages::Stage1);
        CHECK_EQUAL(1, stats.estimatedDepth(Stages::Stage1));

        stats.updatePrevious(Stages::Stage1);
        CHECK_EQUAL(0, stats.estimatedDepth(Stages::Stage1));
    }

    TEST_FIXTURE(MessageCountFixture, verifyReset)
    {
        using namespace test;

        stats.increment(Stages::Stage1, 10);
        CHECK_EQUAL(10, stats.estimatedDepth(Stages::Stage1));

        stats.reset();

        CHECK_EQUAL(0, stats.estimatedDepth(Stages::Stage1));
    }
}
