#include "platform/UnitTestSupport.h"
#include "test/Queue.h"

#include <cstddef>

namespace {

    TEST(verifyQueueBaseInstantiationWithStdQueue)
    {
        Queue q;
    }

    TEST(verifyQueuePushAndPop)
    {
        Queue q;
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;

        q.push(m);
        CHECK(q.tryPop(m2));
        CHECK(m2);
    }

    TEST(verifyQueuePushAndPopWithMultipleMessages)
    {
        Queue q;
        Message::smartptr m = nullptr;

        for(std::size_t i = 0; i < 10; ++i)
        {
            m = new TestMessage();
            q.push(m);
        }

        CHECK_EQUAL(10, q.unsafeSize() );
        for(std::size_t i = 0; i < 10; ++i)
        {
            CHECK(q.tryPop(m));
            CHECK(m);
        }
    }
}
