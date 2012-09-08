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
        TestProcessingFunctor f;
        TestStage s(Stages::Stage1, d, q, f);

        TestScheduler scheduler(d);

        TestMessage::smartptr m = new TestMessage();
        std::thread producer([&d, m]()
        {
            d.dispatch(Stages::Stage1, *m);
        });

        std::thread t([&scheduler]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            scheduler.stop();
        });

        scheduler.start();

        scheduler.join();
        t.join();
        producer.join();
    }
}
