#include "platform/UnitTestSupport.h"

#include <mutex>
#include <queue>

#include <wield/adapters/QueueAdapter.h>
#include <wield/MessageBase.h>

#include "platform/ConcurrentQueue.h"

#include "test_adapter/ProcessingFunctor.h"
#include "test_adapter/Dispatcher.h"
#include "test_adapter/Stage.h"
#include "test_adapter/Queue.h"
#include "test_adapter/Message.h"

namespace {
   
    using namespace wield;
    using namespace test_adapter;
    
    // A basic concurrent queue implementation using mutex to protect std::queue.
    template<class ProcessingFunctor>
    class SimpleConcurrentQueue
    {
    public:
        using MessagePtr = typename MessageBase<ProcessingFunctor>::smartptr;
        
        void push(const MessagePtr& message)
        {
            std::lock_guard<std::mutex> lock(lock_);
            queue_.push(message);
        }
        
        bool try_pop(MessagePtr& message)
        {
            std::lock_guard<std::mutex> lock(lock_);
            
            if(!queue_.empty())
            {
                // front returns &, make a copy to increment ref count before pop().
                MessagePtr m = queue_.front();
                message = std::move(m);
                
                queue_.pop();
                return true;
            }
            
            return false;
        }
        
        std::size_t unsafe_size(void) const
        {
            std::lock_guard<std::mutex> lock(lock_);
            return queue_.size();
        }
        
    private:
        mutable std::mutex lock_;
        std::queue<MessagePtr> queue_;
    };
    
    TEST(verifyCanInstantiateQueueAdapterWithConcurrentQueue)
    {
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;
        
        adapters::QueueAdapter<ProcessingFunctorInterface, Concurrency::concurrent_queue<Message::smartptr>> queue;
        queue.push(m);
        
        CHECK_EQUAL(1, queue.unsafeSize());
        CHECK(queue.tryPop(m2));
        CHECK(m2 != nullptr);
    }
    
    TEST(verifyCanInstantiateQueueAdapterWithOurQueueImplementation)
    {
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;
        
        adapters::QueueAdapter<ProcessingFunctorInterface, SimpleConcurrentQueue<ProcessingFunctorInterface>> queue;
        queue.push(m);
        
        CHECK_EQUAL(1, queue.unsafeSize());
        CHECK(queue.tryPop(m2));
    }
    
    TEST(verifyCanDispatcheBetweenStagesUsingQueueAdapterAsQueueTypeForStage)
    {
        Dispatcher d;
        
        ProcessingFunctorWithDispatcher<Dispatcher> f(d);
        ProcessingFunctor f2;
        
        // construct concrete queues with different implementations.
        using ConcreteQueue1 = adapters::QueueAdapter<ProcessingFunctorInterface, Concurrency::concurrent_queue<Message::smartptr>>;
        ConcreteQueue1 q1;
        
        using ConcreteQueue2 = adapters::QueueAdapter<ProcessingFunctorInterface, SimpleConcurrentQueue<ProcessingFunctorInterface>>;
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
