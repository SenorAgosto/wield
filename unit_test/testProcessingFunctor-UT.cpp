#include <UnitTest++/UnitTest++.h>
#include <wield/MessageBase.h>

#include "Message.h"
#include "TestProcessingFunctor.h"

namespace {

	using namespace wield;

	TEST(verifyProcessingFunctorInitialization)
	{
		TestProcessingFunctor tpf;
	}
	
	TEST(verifyProcessingFunctorIsCalledWhenProcessingTestMessage)
	{
		TestProcessingFunctor tpf;
		Message::smartptr m = new TestMessage();
		
		m->ProcessWith(tpf);
        CHECK(!tpf.messageBaseCalled);
		CHECK(tpf.message1Called);
		CHECK(! tpf.message2Called);
	}

    TEST(verifyProcessingFunctorIsCalledWhenProcessingTestMessage2)
    {
        TestProcessingFunctor tpf;
        Message::smartptr m = new TestMessage2();

        m->ProcessWith(tpf);
        CHECK(!tpf.messageBaseCalled);
        CHECK(!tpf.message1Called);
        CHECK(tpf.message2Called);
    }

    TEST(verifyProcessingFunctorMessageBaseIsCalledWhenUnknownMessageTypeIsProcessed)
    {
        TestProcessingFunctor tpf;
        Message::smartptr m = new TestMessage3();

        m->ProcessWith(tpf);
        CHECK(tpf.messageBaseCalled);
        CHECK(!tpf.message1Called);
        CHECK(!tpf.message2Called);
    }
}