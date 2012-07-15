#include <UnitTest++/UnitTest++.h>

namespace {

    using namespace wield;
    
    TEST(verifyStageInstantiation)
    {
        TestDispactcher d;
        TestQueue q;
        TestProcessingFunctor f;
        
        Stage s("stage1", d, q, f);
    }
    
    TEST(verifyStageProcessWorks)
    {
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctor f;
        TestMessage m;
       
        //put something in the queue for processing
        q.enque(m);
        
        Stage s("stage1", d, q, f);
        s.process();
        
        CHECK(f.testMessageFunctorCalled);
    }
}