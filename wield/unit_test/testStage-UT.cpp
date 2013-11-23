#include "platform/UnitTestSupport.h"
#include "test/ProcessingFunctor.h"
#include "test/Queue.h"
#include "test/Dispatcher.h"
#include "test/Stage.h"

namespace {

    using namespace test;
    
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
        s.push(m);

        CHECK(s.process());
        CHECK(f.message1Called_);
    }

    TEST(verifyStageProcessingForMultipleMessages)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;
        Message::smartptr m = new TestMessage();

        Stage s(Stages::Stage1, d, q, f);
        s.push(m);
        s.push(m);
        
        CHECK(s.process());
        CHECK(s.process());
        CHECK(!s.process());
    }
}