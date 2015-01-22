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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace {

    using namespace test;
    
    using Dispatcher = Traits::Dispatcher;
    using Message = Traits::Message;
    using Scheduler = Traits::Scheduler;
    using Stage = Traits::Stage;
    using Queue = Traits::Queue;

    // a timer callback
    void createMessage(
          const boost::system::error_code& error
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

        Scheduler scheduler(d);

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
        CHECK_EQUAL(4, f.message1CallCount_);
    }
}
