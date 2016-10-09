#include "./platform/UnitTestSupport.hpp"

#include "./test_batch/Message.hpp"
#include "./test_batch/ProcessingFunctor.hpp"

#include <algorithm>

namespace {

    using namespace test_batch;
    
    TEST(verifyBatchMessageProcessingWorksWithInlinableNonVirtualCalls)
    {
        BatchProcessingFunctor pf;
        
        // create a batch message, then add some messages to it.
        BatchMessage batch;
        batch.addMessage(new TestMessage());
        batch.addMessage(new TestMessage());
        batch.addMessage(new TestMessage());
        
        // process the batch message
        batch.processWith(pf);
        CHECK_EQUAL(3U, pf.message1Count);
        CHECK_EQUAL(1U, pf.message2Count);
    }
}
