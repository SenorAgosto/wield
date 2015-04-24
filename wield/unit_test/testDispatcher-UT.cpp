#include "./platform/UnitTestSupport.hpp"
#include <exception>

#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"
#include "./test/Message.hpp"

namespace {

    using namespace test;
    
    using Dispatcher = Traits::Dispatcher;
    using Stage = Traits::Stage;
    using Queue = Traits::Queue;

    TEST(verifyDispatcherDispatch)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;

        Stage s(Stages::Stage1, d, q, f);
        Message::smartptr m = new TestMessage();

        CHECK_EQUAL(0U, q.unsafe_size());
        d.dispatch(Stages::Stage1, *m);
        CHECK_EQUAL(1U, q.unsafe_size());
    }

    TEST(verifyDispatcherDispatchByCloning)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;

        Stage s(Stages::Stage1, d, q, f);

        // inside processing functors, they have the actual concrete type,
        // simulate that here by creating concrete type on heap.
        std::unique_ptr<TestMessage> m(new TestMessage());

        CHECK_EQUAL(0U, q.unsafe_size());
        d.dispatch(Stages::Stage1, *m, wield::clone_message);
        CHECK_EQUAL(1U, q.unsafe_size());

        Message::smartptr m2;
        q.try_pop(m2);

        CHECK(m2);
        CHECK(m.get() != m2);    // verify we cloned.
    }

    TEST(verifyDispatchingCanGetAMessageFromOneStageToAnother)
    {
        Dispatcher d;
        Queue q;
        Queue q2;
        ProcessingFunctorWithDispatcher<Dispatcher> f(d);
        ProcessingFunctor f2;

        Stage s(Stages::Stage1, d, q, f);
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

    TEST(verifyBracketOperator)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctorWithDispatcher<Dispatcher> f(d);

        Stage s(Stages::Stage1, d, q, f);

        Stage& sr = d[Stages::Stage1];
        CHECK(&sr);
    }

    TEST(verifyDispatcherThrowsIfStageNameIsRegisteredTwice)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;

        Stage s(Stages::Stage1, d, q, f);
        CHECK_THROW(Stage s2(Stages::Stage1, d, q, f);, std::runtime_error );
    }
}
