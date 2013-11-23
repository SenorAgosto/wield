#include "platform/UnitTestSupport.h"
#include "platform/Regex.h"

#include "test/Scheduler.h"
#include "test/Dispatcher.h"
#include "test/Queue.h"

#include <wield/logging/Log.h>
#include <wield/logging/LoggingPolicy.h>
#include <wield/platform/thread>

#include <cstddef>

namespace {

    TEST(verifySchedulerInstantiation)
    {
        Dispatcher d;
        Scheduler scheduler(d);
    }

    TEST(verifySchedulerCreatesThreads)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;
        Stage s(Stages::Stage1, d, q, f);

        Scheduler scheduler(d);

        Message::smartptr m = new TestMessage();
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
    
    class LogToStr : public wield::logging::LoggingPolicy
    {
    public:
        LogToStr(std::stringstream& ss)
            : ss_(ss)
        {
        }
        
        void Info(const std::string& info) const override { ss_ << "[Info]" << info << std::endl; }
        void Warning(const std::string& warning) const override { ss_ << "[Warning]" << warning << std::endl; }
        void Error(const std::string& error) const override { ss_ << "[Error]" << error << std::endl; }
        
    private:
        std::stringstream& ss_;
    };
    
    TEST(verifyAnExceptionThrownByStageIsCaughtAndLogged)
    {
        std::stringstream ss;
        wield::logging::Log::SetLoggingPolicy(wield::logging::LoggingPolicyType(new LogToStr(ss)));

        Dispatcher d;
        Queue q;
        ThrowingProcessingFunctor f;
        Stage s(Stages::Stage1, d, q, f);

        static const std::size_t numberOfThreads = 1;
        Scheduler scheduler(d, numberOfThreads);

        Message::smartptr m = new TestMessage();
        d.dispatch(Stages::Stage1, *m);

        CHECK_EQUAL(1, q.unsafeSize());
        
        scheduler.start();
        scheduler.join();

        CHECK(std::regex_match(ss.str(), std::regex(".*Scheduler: an exception occurred: I'm broke.\n")));
    }
}
