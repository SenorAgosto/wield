#include <UnitTest++/UnitTest++.h> 
#include "TestQueue.h"

#include <cstddef>

namespace {

    TEST(verifyQueueBaseInstantiationWithStdQueue)
    {
        TestQueue q;
    }

    TEST(verifyQueuePushAndPop)
    {
        TestQueue q;
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;

        q.push(m);
        CHECK(q.try_pop(m2));
        CHECK(m2);
    }

    TEST(verifyQueuePushAndPopWithMultipleMessages)
    {
        TestQueue q;
        Message::smartptr m = nullptr;

        for(std::size_t i = 0; i < 10; ++i)
        {
            m = new TestMessage();
            q.push(m);
        }

        CHECK_EQUAL(10, q.unsafe_size() );
        for(std::size_t i = 0; i < 10; ++i)
        {
            CHECK(q.try_pop(m));
            CHECK(m);
        }
    }
}
