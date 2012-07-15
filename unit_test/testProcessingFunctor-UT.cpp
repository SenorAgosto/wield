#include <UnitTest++/UnitTest++.h>
#include <wield/Message.h>
#include <wield/ProcessingFunctor.h>

namespace {

	using namespace wield;
	
	class TestProcessingFunctor : public ProcessingFunctor
	{
	public:
		TestProcessingFunctor()
			: messageFunctorCalled(false)
			, testMessageFunctorCalled(false)
		{
		}
		
		virtual void operator()(Message& msg) { messageFunctorCalled = true; }
		
		bool messageFunctorCalled;
		bool testMessageFunctorCalled;
	};
	
	
	TEST(verifyProcessingFunctorInitialization)
	{
		TestProcessingFunctor tpf;
	}
	
	TEST(verifyProcessingFunctorIsCalledWhenProcessingMessage)
	{
		TestProcessingFunctor tpf;
		Message::smartptr m = new Message();
		
		m->ProcessWith(tpf);
		CHECK(tpf.messageFunctorCalled);
		CHECK(! tpf.testMessageFunctorCalled);
	}
}