#include "./platform/UnitTestSupport.hpp"

#include <wield/adapters/polymorphic/PassThroughStageQueue.hpp>

#include "./test_adapter/Traits.hpp"
#include "./test_adapter/ProcessingFunctor.hpp"
#include "./test_adapter/Message.hpp"

namespace {

    using namespace test_adapter;

    using Dispatcher = Traits::Dispatcher;

    using PassThroughStageQueue = Traits::ApplicationTraits::PassThroughStageQueue;
    using Stage = Traits::Stage;
    
    TEST(verifyPassThroughStageQueueUsage)
    {
        Dispatcher d;
        
        ProcessingFunctorWithDispatcher<Dispatcher> f(d);
        ProcessingFunctor f2;
        
        // Note: realize that whatever functor is assigned to the pass-through stage queue is
        // the one employed. The processing functor passed to Stage during construction will be
        // ignored.
        PassThroughStageQueue q1(f);
        PassThroughStageQueue q2(f2);
        
        Stage s(Stages::Stage1, d, q1, f);
        Stage s2(Stages::Stage2, d, q2, f2);
        
        Message::smartptr m = new TestMessage();
        
        // s uses a pass-through stage queues, pushing this message
        // onto s should cause it to be immediately processed and dispatched to s2,
        // which also uses a pass-through stage queue and should get immediately
        // processed.
        //
        // Note: DO NOT scheduling pass-through stages is a pointless waste of time.
        // It probably wouldn't hurt anything as attempting to pump them for messages
        // will fail. Just take note.
        s.push(m);
        
        CHECK(!f.messageBaseCalled_);
        CHECK(f.message1Called_);
        CHECK(!f.message2Called_);
        
        CHECK(!f2.messageBaseCalled_);
        CHECK(f2.message1Called_);
        CHECK(!f2.message2Called_);
    }
}