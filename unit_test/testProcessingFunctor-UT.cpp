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
	
	TEST(verifyProcessingFunctorIsCalledWhenProcessingMessage)
	{
		TestProcessingFunctor tpf;
		Message::smartptr m = new TestMessage();
		
		m->ProcessWith(tpf);
		CHECK(tpf.message1Called);
		CHECK(! tpf.message2Called);
	}

    TEST(verifyProcessingFunctorIsCalledWhenProcessingMessage2)
    {
        TestProcessingFunctor tpf;
        Message::smartptr m = new TestMessage2();

        m->ProcessWith(tpf);
        CHECK(!tpf.message1Called);
        CHECK(tpf.message2Called);
    }
}