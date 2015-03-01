#include "./platform/UnitTestSupport.hpp"
#include <wield/MessageBase.hpp>

#include "./test/Message.hpp"
#include "./test/ProcessingFunctor.hpp"

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

    TEST(verifyExtendedProcessingFunctorCanDeferMessageProcessingToBaseClass)
    {
        PartialProcessingFunctor ppf;

        // TestMessage is not handled by PartialProcessingFunctor,
        // it will work normally, incrementing counters.
        Message::smartptr m = new TestMessage();
        m->processWith(ppf);

        CHECK(!ppf.messageBaseCalled_);
        CHECK(ppf.message1Called_);
        CHECK(!ppf.message2Called_);

        // TestMessage2 is handled by PartialProcessingFunctor,
        // it will prevent incrementing counters.
        PartialProcessingFunctor ppf2;
        m = new TestMessage2();
        m->processWith(ppf2);

        CHECK(!ppf2.messageBaseCalled_);
        CHECK(!ppf2.message1Called_);
        CHECK(!ppf2.message2Called_);

        CHECK_EQUAL(1, ppf2.hazah_);
    }
}