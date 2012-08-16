#include <UnitTest++/UnitTest++.h>
#include "TestScheduler.h"

#include <thread>

namespace {

    TEST(verifySchedulerInstantiation)
    {
        TestScheduler scheduler;
    }

    TEST(verifySchedulerCreatesThreads)
    {
        TestScheduler scheduler;
        std::thread t([&scheduler]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            scheduler.stop();
        });

        scheduler.start();
        t.join();
    }
}
