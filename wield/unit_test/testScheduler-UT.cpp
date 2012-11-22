#include "platform/UnitTestSupport.h"
#include "platform/Regex.h"

#include "test/Scheduler.h"
#include "test/Dispatcher.h"

#include <wield/logging/ScopedClogRedirector.h>
#include <wield/platform/thread>

#include <cstddef>

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

    TEST(verifyAnExceptionThrownByStageIsCaughtAndLogged)
    {
        wield::logging::ScopedClogRedirector<std::ostringstream> loggingRedirector;
        TestDispatcher d;
        TestQueue q;
        ThrowingProcessingFunctor f;
        TestStage s(Stages::Stage1, d, q, f);

        static const std::size_t numberOfThreads = 1;
        TestScheduler scheduler(d, numberOfThreads);

        TestMessage::smartptr m = new TestMessage();
        d.dispatch(Stages::Stage1, *m);

        scheduler.start();
        scheduler.join();

        CHECK(std::regex_match(loggingRedirector.str(), std::regex(".*Scheduler: an exception occurred: I'm broke.\n")));
    }
}
