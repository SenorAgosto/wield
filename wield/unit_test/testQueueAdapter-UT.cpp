#include "platform/UnitTestSupport.h"

#include <mutex>
#include <queue>

#include <wield/adapters/polymorphic/QueueAdapter.h>
#include <wield/MessageBase.h>

#include "platform/ConcurrentQueue.h"

#include "test_adapter/Traits.h"
#include "test_adapter/ProcessingFunctor.h"
#include "test_adapter/Message.h"
#include "test_adapter/SimpleConcurrentQueue.h"

namespace {
   
    using namespace wield;
    using namespace test_adapter;
    
    using Dispatcher = test_adapter::Traits::Dispatcher;
    using Stage = test_adapter::Traits::Stage;
        
    TEST(verifyCanInstantiateQueueAdapterWithConcurrentQueue)
    {
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;
        
        adapters::polymorphic::QueueAdapter<ProcessingFunctorInterface, Concurrency::concurrent_queue<Message::smartptr>> queue;
        queue.push(m);
        
        CHECK_EQUAL(1, queue.unsafe_size());
        CHECK(queue.try_pop(m2));
        CHECK(m2 != nullptr);
    }
    
    TEST(verifyCanInstantiateQueueAdapterWithOurQueueImplementation)
    {
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;
        
        adapters::polymorphic::QueueAdapter<ProcessingFunctorInterface, SimpleConcurrentQueue> queue;
        queue.push(m);
        
        CHECK_EQUAL(1, queue.unsafe_size());
        CHECK(queue.try_pop(m2));
    }
    
    TEST(verifyCanDispatcheBetweenStagesUsingQueueAdapterAsQueueTypeForStage)
    {
        Dispatcher d;
        
        ProcessingFunctorWithDispatcher<Dispatcher> f(d);
        ProcessingFunctor f2;
        
        // construct concrete queues with different implementations.
        using ConcreteQueue1 = adapters::polymorphic::QueueAdapter<ProcessingFunctorInterface, Concurrency::concurrent_queue<Message::smartptr>>;
        ConcreteQueue1 q1;
        
        using ConcreteQueue2 = adapters::polymorphic::QueueAdapter<ProcessingFunctorInterface, SimpleConcurrentQueue>;
        ConcreteQueue2 q2;
        
        Stage s(Stages::Stage1, d, q1, f);
        Stage s2(Stages::Stage2, d, q2, f2);
        
        Message::smartptr m = new TestMessage();
        s.push(m);
        s.process();    // this should pass the message on to Stage2.
        s2.process();
        
        CHECK(!f.messageBaseCalled_);
        CHECK(f.message1Called_);
        CHECK(!f.message2Called_);
        
        CHECK(!f2.messageBaseCalled_);
        CHECK(f2.message1Called_);
        CHECK(!f2.message2Called_);
    }
}
