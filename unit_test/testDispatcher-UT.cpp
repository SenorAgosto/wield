#include <UnitTest++/UnitTest++.h>

#include "TestProcessingFunctor.h"
#include "TestDispatcher.h"
#include "TestStage.h"
#include "TestQueue.h"
#include "TestMessage.h"

namespace {

    using namespace wield;

    TEST(verifyDispatchingCanGetAMessageFromOneStageToAnother)
    {
        TestDispatcher d;
        TestQueue q;
        TestQueue q2;
        TestProcessingFunctorWithDispatcher<TestDispatcher> f(d);
        TestProcessingFunctor f2;

        TestStage s(Stages::Stage1, d, q, f);
        TestStage s2(Stages::Stage2, d, q2, f2);

        TestMessage::smartptr m = new TestMessage();
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
        TestDispatcher d;
        TestQueue q;
        TestProcessingFunctorWithDispatcher<TestDispatcher> f(d);

        TestStage s(Stages::Stage1, d, q, f);

        TestStage& sr = d[Stages::Stage1];
        CHECK(&sr);
    }
}
