#include <UnitTest++/UnitTest++.h>
#include "TestProcessingFunctor.h"
#include "TestQueue.h"
#include "TestDispatcher.h"
#include "TestStage.h"

namespace {

    using namespace wield;

    TEST(verifyStageInstantiation)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctor f;

        TestStage s(Stages::Stage1, d, q, f);
    }
    
    TEST(verifyStageProcessWorks)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctor f;
        TestMessage::smartptr m = new TestMessage();
        
        TestStage s(Stages::Stage1, d, q, f);
        s.push(m);
        s.process();
        
        CHECK(f.message1Called_);
    }
}