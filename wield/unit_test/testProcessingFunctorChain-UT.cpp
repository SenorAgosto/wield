#include "./platform/UnitTestSupport.hpp"

#include "./test_adapter/Traits.hpp"
#include "./test_adapter/ProcessingFunctor.hpp"
#include "./test_adapter/Message.hpp"

#include <wield/adapters/ProcessingFunctorChain.hpp>

namespace {
    
    using namespace test_adapter;
    
    using Dispatcher = Traits::Dispatcher;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Message = Traits::Message;

    // This test demonstrates the use case for
    // ProcessingFunctorChain and the helper function
    // CreateProcessorChain.
    //
    // CAVEAT: if any of the processing functors in the chain
    // besides the last one dispatch the message to another
    // stage, you could introduce a race condition in your
    // application.
    TEST(verifyCreateProcessorChainWorks)
    {
        Dispatcher d;
        ProcessingFunctor f;
        
        // in practice, each processing functor would likely be different, here we'll call f() 3 times.
        // f -> f -> f
        auto processorChain = wield::adapters::CreateProcessorChain<Message::ptr, ProcessingFunctorInterface>(&f, &f, &f);

        // then use the processorChain as the queue argument to the stage.
        Stage s(Stages::Stage1, d, processorChain, f);  // f is a dummy.
        
        CHECK_EQUAL(0U, f.messageBaseCallCount_);
        CHECK_EQUAL(0U, f.message1CallCount_);
        CHECK_EQUAL(0U, f.message2CallCount_);
        
        Message::smartptr m = new TestMessage;
        d.dispatch(Stages::Stage1, *m); // as a pass-through stage, processed on the push.
        
        CHECK_EQUAL(0U, f.messageBaseCallCount_);
        CHECK_EQUAL(3U, f.message1CallCount_);
        CHECK_EQUAL(0U, f.message2CallCount_);
        
        m = new TestMessage2;
        d.dispatch(Stages::Stage1, *m);
        
        CHECK_EQUAL(0U, f.messageBaseCallCount_);
        CHECK_EQUAL(3U, f.message1CallCount_);
        CHECK_EQUAL(3U, f.message2CallCount_);
    }
}
