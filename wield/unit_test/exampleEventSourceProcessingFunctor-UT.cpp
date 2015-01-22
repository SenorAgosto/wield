#include "platform/UnitTestSupport.hpp"

#include "test/Traits.hpp"
#include "test/ProcessingFunctor.hpp"
#include "test/Message.hpp"

namespace {
    
    using namespace test;
    
    using Dispatcher = Traits::Dispatcher;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Message = Traits::Message;
    
    // when invoked to process a message, we create a new message
    // and dispatch it. We could translate the original message into
    // a new message, discard the old message, or process the old message.
    class EventSourceFunctor : public ProcessingFunctorWithDispatcher<Dispatcher>
    {
    public:
        EventSourceFunctor(Dispatcher& dispatcher)
            : ProcessingFunctorWithDispatcher<Dispatcher>(dispatcher)
        {
        }
        
        // When we get a TestMessage, create a TestMessage2 and dispatch it
        // to stage2.
        void operator()(TestMessage& m) override
        {
            message1CallCount_++;
            
            Message::smartptr n = new TestMessage2();
            dispatcher_.dispatch(Stages::Stage2, *n);
        }
    };
    
    TEST(exampleShowHowAProcessingFunctorCanGenerateMessages)
    {
        Dispatcher d;
        
        Queue q;
        EventSourceFunctor f(d);
        Stage s(Stages::Stage1, d, q, f);
        
        Queue q2;
        ProcessingFunctor f2;
        Stage s2(Stages::Stage2, d, q2, f2);
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(0, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);
        
        CHECK_EQUAL(0, f2.messageBaseCallCount_);
        CHECK_EQUAL(0, f2.message1CallCount_);
        CHECK_EQUAL(0, f2.message2CallCount_);
        
        // when stage s is sent a TestMessage, it sends a TestMessage2 to s2.
        Message::smartptr m = new TestMessage();
        s.push(m);
        s.process();
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(1, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);

        s2.process();
        
        CHECK_EQUAL(0, f2.messageBaseCallCount_);
        CHECK_EQUAL(0, f2.message1CallCount_);
        CHECK_EQUAL(1, f2.message2CallCount_);
    }
}
