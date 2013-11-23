#include "platform/UnitTestSupport.h"

#include <wield/adapters/PassThroughStageQueue.h>

#include "test_adapter/ProcessingFunctor.h"
#include "test_adapter/Dispatcher.h"
#include "test_adapter/Stage.h"
#include "test_adapter/Queue.h"
#include "test_adapter/Message.h"

namespace {

    TEST(verifyPassThroughStageQueueUsage)
    {
        Dispatcher d;
        
        ProcessingFunctorWithDispatcher<Dispatcher> f(d);
        ProcessingFunctor f2;

        // use pass-through stage queues for every stage in the pipeline.
        using PassThroughStageQueue = wield::adapters::PassThroughStageQueue<ProcessingFunctorInterface>;
        
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