#include "./platform/UnitTestSupport.hpp"

#include <wield/schedulers/utils/ThreadAssignments.hpp>

#include "./test/Traits.hpp"

namespace {

    struct ThreadAssignmentFixture
    {
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages, 2>;
        Assignments assignments;
    };

    TEST_FIXTURE(ThreadAssignmentFixture, verifyInstantiationOfThreadAssignments)
    {
        using namespace test;

        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(0));
        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(1));
    }

    TEST_FIXTURE(ThreadAssignmentFixture, verifyAssignmentOfThread)
    {
        using namespace test;

        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(0));
        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK_EQUAL(Stages::Stage1, assignments.currentAssignment(0));
    }

    TEST_FIXTURE(ThreadAssignmentFixture, verifyAssignmentDoesntExceedMaximum)
    {
        using namespace test;

        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(0));
        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(1));

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(!assignments.tryAssign(1, Stages::Stage1));

        CHECK_EQUAL(Stages::Stage1, assignments.currentAssignment(0));
        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(1));
    }

    TEST_FIXTURE(ThreadAssignmentFixture, verifyRemoveAssignment)
    {
        using namespace test;

        assignments.tryAssign(0, Stages::Stage1);
        CHECK_EQUAL(Stages::Stage1, assignments.currentAssignment(0));

        assignments.removeCurrentAssignment(0);
        CHECK_EQUAL(Stages::NumberOfEntries, assignments.currentAssignment(0));
    }

    TEST(verifyInstantiationUsingMaximumConcurrencyConstructor)
    {
        using namespace test;
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages, 3>;

        Assignments::MaxConcurrencyContainer maximumConcurrency = {{2, 2, 2 }};
        Assignments assignments(maximumConcurrency);

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(assignments.tryAssign(1, Stages::Stage1));
        CHECK(!assignments.tryAssign(2, Stages::Stage1));
    }

    TEST(verifyInstantiationUsingMaximumConcurrencyMoveConstructor)
    {
        using namespace test;
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages, 3>;

        Assignments assignments(Assignments::MaxConcurrencyContainer{{2, 2, 2}});

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(assignments.tryAssign(1, Stages::Stage1));
        CHECK(!assignments.tryAssign(2, Stages::Stage1));
    }
}
