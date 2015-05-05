#include "platform/UnitTestSupport.hpp"
#include "platform/Regex.hpp"

#include "test/Traits.hpp"
#include "test/Message.hpp"
#include "test/ProcessingFunctor.hpp"
#include "test/Scheduler.hpp"

#include <wield/logging/Log.hpp>
#include <wield/logging/LoggingPolicy.hpp>
#include <wield/platform/thread>

#include <cstddef>
#include <thread>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma clang diagnostic pop 

namespace {

    using namespace test;
    
    using Dispatcher = Traits::Dispatcher;
    using Message = Traits::Message;
    using Scheduler = Traits::Scheduler;
    using Stage = Traits::Stage;
    using Queue = Traits::Queue;

    // a timer callback
    void createMessage(
          const boost::system::error_code& 
        , boost::asio::deadline_timer& timer
        , Dispatcher& dispatcher
        , Scheduler& scheduler
        , int& count)
    {
        if(count++ < 4)
        {
            // create and dispatch the message.
            Message::smartptr m = new TestMessage();
            dispatcher.dispatch(Stages::Stage1, *m);

            // reschedule the timer
            timer.expires_at(timer.expires_at() + boost::posix_time::milliseconds(250));
            timer.async_wait(
                boost::bind(createMessage, boost::asio::placeholders::error, boost::ref(timer), boost::ref(dispatcher), boost::ref(scheduler), count)
            );
        }
        else
        {
            scheduler.stop();
        }
    }

    TEST(exampleTimerDrivenEventSource)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;
        Stage s(Stages::Stage1, d, q, f);

        Scheduler scheduler(d, std::thread::hardware_concurrency());

        std::thread producer([&d, &scheduler]()
        {
            boost::asio::io_service io;
            boost::asio::deadline_timer timer(io, boost::posix_time::milliseconds(250));
            boost::system::error_code error;
            int count = 0;

            timer.async_wait(
                boost::bind(createMessage, boost::asio::placeholders::error, boost::ref(timer), boost::ref(d), boost::ref(scheduler), count)
            );

            io.run();   // runs until there is no work left on asio
        });

        scheduler.start();

        producer.join();
        scheduler.join();

        // verify there were 4 recieved test messages.
        CHECK_EQUAL(4U, f.message1CallCount_);
    }
}
