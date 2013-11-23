#include "platform/UnitTestSupport.h"
#include <wield/MessageBase.h>

#include "test/Message.h"
#include "test/ProcessingFunctor.h"

namespace {

    using namespace test;
    
	TEST(verifyProcessingFunctorInitialization)
	{
		ProcessingFunctor tpf;
	}
	
	TEST(verifyProcessingFunctorIsCalledWhenProcessingTestMessage)
	{
		ProcessingFunctor tpf;
		Message::smartptr m = new TestMessage();
		
		m->processWith(tpf);
        CHECK(!tpf.messageBaseCalled_);
		CHECK(tpf.message1Called_);
		CHECK(! tpf.message2Called_);
	}

    TEST(verifyProcessingFunctorIsCalledWhenProcessingTestMessage2)
    {
        ProcessingFunctor tpf;
        Message::smartptr m = new TestMessage2();

        m->processWith(tpf);
        CHECK(!tpf.messageBaseCalled_);
        CHECK(!tpf.message1Called_);
        CHECK(tpf.message2Called_);
    }

    TEST(verifyProcessingFunctorMessageBaseIsCalledWhenUnknownMessageTypeIsProcessed)
    {
        ProcessingFunctor tpf;
        Message::smartptr m = new TestMessage3();

        m->processWith(tpf);
        CHECK(tpf.messageBaseCalled_);
        CHECK(!tpf.message1Called_);
        CHECK(!tpf.message2Called_);
    }
}