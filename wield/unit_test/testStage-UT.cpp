#include "platform/UnitTestSupport.h"
#include "test/ProcessingFunctor.h"
#include "test/Queue.h"
#include "test/Dispatcher.h"
#include "test/Stage.h"

namespace {

    using namespace wield;

    TEST(verifyStageInstantiation)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctor f;

        TestStage s(Stages::Stage1, d, q, f);
        CHECK(Stages::Stage1 == s.name());
    }
    
    TEST(verifyStageProcessWorks)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctor f;
        TestMessage::smartptr m = new TestMessage();
        
        TestStage s(Stages::Stage1, d, q, f);
        s.push(m);

        CHECK(s.process());
        CHECK(f.message1Called_);
    }

    TEST(verifyStageProcessingForMultipleMessages)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctor f;
        TestMessage::smartptr m = new TestMessage();

        TestStage s(Stages::Stage1, d, q, f);
        s.push(m);
        s.push(m);
        
        CHECK(s.process());
        CHECK(s.process());
        CHECK(!s.process());
    }
}