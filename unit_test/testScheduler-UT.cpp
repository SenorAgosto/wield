#include <UnitTest++/UnitTest++.h>
#include "TestScheduler.h"
#include "TestDispatcher.h"

#include <thread>

namespace {

    TEST(verifySchedulerInstantiation)
    {
        TestDispatcher d;
        TestScheduler scheduler(d);
    }

    TEST(verifySchedulerCreatesThreads)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctorWithDispatcher<TestDispatcher> f(d);
        TestStage s(Stages::Stage1, d, q, f);

        TestScheduler scheduler(d);

        std::thread t([&scheduler]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            scheduler.stop();
        });

        scheduler.start();
        t.join();
    }
}
