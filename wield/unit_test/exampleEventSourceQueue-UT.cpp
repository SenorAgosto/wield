#include "platform/UnitTestSupport.hpp"

#include "test_adapter/Traits.hpp"
#include "test_adapter/ProcessingFunctor.hpp"
#include "test_adapter/Message.hpp"

#include <wield/adapters/polymorphic/QueueAdapter.hpp>

namespace {
    
    using namespace test_adapter;
    
    using Dispatcher = Traits::Dispatcher;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Message = Traits::Message;
    using MessagePtr = typename Message::smartptr;
    
    struct EventSourceQueueImpl
    {
        void push(const MessagePtr& message)
        {
            // here we do nothing. You *could* have source queues
            // implement actual queues also so they could recieve
            // message from other stages in addition to creating
            // events.
        }
        
        // create an event here
        bool try_pop(MessagePtr& message)
        {
            message = new TestMessage();
            return true;
        }
        
        std::size_t unsafe_size(void) const
        {
            return 0;
        }
    };
    
    // note: use queue adaptor, input stages have different implementations, regular
    // processing stages continue to use actual queues, etc.
    struct EventSourceQueue : public wield::adapters::polymorphic::QueueAdapter<ProcessingFunctorInterface, EventSourceQueueImpl>
    {
    };
    
    // This example demonstrates how events can be created when a
    // thread visits a stage. This is the preferred way to do source
    // events, e.g. select/reading from a socket creating an event with
    // the buffer (or going ahead and parsing the data here then
    // forwarding the message/event on.
    TEST(exampleVisitingAStageGeneratesAnEvent)
    {
        Dispatcher d;
        EventSourceQueue q;
        ProcessingFunctor f;    // you would do the routing in f, or parsing/processing/forwarding in f.
        
        Stage s(Stages::Stage1, d, q, f);
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(0, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);
        
        s.process();
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(1, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);
        
        s.process();
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(2, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);
    }
}