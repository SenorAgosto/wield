#include "./platform/UnitTestSupport.hpp"

#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"
#include "./test/Message.hpp"

namespace {

    using namespace test;
    using Dispatcher = Traits::Dispatcher;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Message = Traits::Message;
    
    TEST(verifyStageInstantiation)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;

        Stage s(Stages::Stage1, d, q, f);
        CHECK(Stages::Stage1 == s.name());
    }
    
    TEST(verifyStageProcessWorks)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;
        Message::smartptr m = new TestMessage();
        
        Stage s(Stages::Stage1, d, q, f);
        s.push(m.get());

        CHECK(s.process());
        CHECK(f.message1Called_);
    }

    TEST(verifyStageProcessingForMultipleMessages)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;
        
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = new TestMessage();

        Stage s(Stages::Stage1, d, q, f);
        s.push(m.get());
        s.push(m2.get());
        
        CHECK(s.process());
        CHECK(s.process());
        CHECK(!s.process());
    }
}