#include "./platform/UnitTestSupport.hpp"
#include <cstddef>

#include "./test/Traits.hpp"
#include "./test/Message.hpp"

namespace {
/*
    using namespace test;
    using Queue = Traits::Queue;
    using Message = Traits::Message;
    
    TEST(verifyQueueBaseInstantiationWithStdQueue)
    {
        Queue q;
    }

    TEST(verifyQueuePushAndPop)
    {
        Queue q;
        Message::smartptr m = new TestMessage();
        Message::smartptr m2 = nullptr;

        q.push(m.get());
        CHECK(q.try_pop(m2));
        CHECK(m2);
    }

    TEST(verifyQueuePushAndPopWithMultipleMessages)
    {
        Queue q;
        Message::smartptr m = nullptr;
        Message::ptr m_ptr = nullptr;

        for(std::size_t i = 0; i < 10; ++i)
        {
            m = new TestMessage();
            q.push(m.get());
        }

        CHECK_EQUAL(10U, q.unsafe_size() );
        for(std::size_t i = 0; i < 10; ++i)
        {
            CHECK(q.try_pop(m_ptr));
            CHECK(m_ptr);
        }
    }
*/
}
