#include "./platform/UnitTestSupport.hpp"
#include "./platform/ConcurrentQueue.hpp"

#include <wield/adapters/polymorphic/QueueAdapter.hpp>
#include <wield/details/QueueCreator.hpp>
#include <vector>

#include "./test/Traits.hpp"
#include "./test/Message.hpp"

#include "./test_adapter/Traits.hpp"
#include "./test_adapter/Message.hpp"

namespace {

    TEST(verifyQueueCreatorAddsRightNumberOfQueuesToVector)
    {
        using namespace test;
        using Queue = Traits::Queue;

        std::deque<Queue> queues;
        CHECK_EQUAL(0, queues.size());

        wield::details::QueueCreator<Queue, 10> creator(queues);
        CHECK_EQUAL(10, queues.size());
    }

    TEST(verifyQueueCreatorSupportsQueueAdapter)
    {
        using namespace test_adapter;
        using MessagePtr = Traits::MessagePtr;

        using Queue = wield::adapters::polymorphic::QueueAdapter<MessagePtr, Concurrency::concurrent_queue<MessagePtr>>;

        std::deque<Queue> queues;
        CHECK_EQUAL(0, queues.size());

        wield::details::QueueCreator<Queue, 5> creator(queues);
        CHECK_EQUAL(5, queues.size());
    }
}
