#include "platform/UnitTestSupport.h"
#include <exception>

#include "test/ProcessingFunctor.h"
#include "test/Dispatcher.h"
#include "test/Stage.h"
#include "test/Queue.h"
#include "test/Message.h"

namespace {

    using namespace wield;

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
