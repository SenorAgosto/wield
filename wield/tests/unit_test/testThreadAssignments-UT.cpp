#include "./platform/UnitTestSupport.hpp"

#include <wield/schedulers/utils/ThreadAssignments.hpp>

#include "./test/Traits.hpp"

namespace {

    TEST(verifyThreadAssignmentDefaultConstructor)
    {
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages>;
        Assignments assignments;
    }
    
    struct ThreadAssignmentFixture
    {
        ThreadAssignmentFixture()
            : assignments(2)
        {
        }
        
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages>;
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
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages>;

        Assignments::MaxConcurrencyContainer maximumConcurrency = {{2, 2, 2 }};
        Assignments assignments(maximumConcurrency, 3);

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(assignments.tryAssign(1, Stages::Stage1));
        CHECK(!assignments.tryAssign(2, Stages::Stage1));
    }

    TEST(verifyInstantiationUsingMaximumConcurrencyConstructorAndDefaultNumberOfThreads)
    {
        using namespace test;
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages>;

        Assignments::MaxConcurrencyContainer maximumConcurrency = {{2, 2, 2 }};
        Assignments assignments(maximumConcurrency);

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(assignments.tryAssign(1, Stages::Stage1));
        CHECK(!assignments.tryAssign(2, Stages::Stage1));
    }

    TEST(verifyInstantiationUsingMaximumConcurrencyMoveConstructor)
    {
        using namespace test;
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages>;

        Assignments assignments(Assignments::MaxConcurrencyContainer{{2, 2, 2}}, 3);

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(assignments.tryAssign(1, Stages::Stage1));
        CHECK(!assignments.tryAssign(2, Stages::Stage1));
    }
    
    TEST(verifyInstantiationUsingMaximumConcurrencyMoveConstructorAndDefaultNumberOfThreads)
    {
        using namespace test;
        using Assignments = wield::schedulers::utils::ThreadAssignments<test::Stages>;

        Assignments assignments(Assignments::MaxConcurrencyContainer{{2, 2, 2}});

        CHECK(assignments.tryAssign(0, Stages::Stage1));
        CHECK(assignments.tryAssign(1, Stages::Stage1));
        CHECK(!assignments.tryAssign(2, Stages::Stage1));
    }
}
