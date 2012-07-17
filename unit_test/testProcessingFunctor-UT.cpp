#include <UnitTest++/UnitTest++.h>
#include <wield/MessageBase.h>

namespace {

	using namespace wield;
	
    class TestMessage;
    class TestMessage2;

    class ProcessingFunctor;
    typedef MessageBase<ProcessingFunctor> Message;

    class ProcessingFunctor
    {
    public:
        ProcessingFunctor()
        {
        }

        virtual void operator()(Message& msg) = 0;
        virtual void operator()(TestMessage& msg) = 0;
        virtual void operator()(TestMessage2& msg) = 0;
    };
    
    class TestProcessingFunctor : public ProcessingFunctor
    {
    public:
        TestProcessingFunctor()
            : message1Called(false)
            , message2Called(false)
        {
        }

        virtual void operator()(Message& msg){}
        virtual void operator()(TestMessage& msg){ message1Called = true; }
        virtual void operator()(TestMessage2& msg) { message2Called = true;}

        bool message1Called;
        bool message2Called;
    };

    class TestMessage : public MessageBase<ProcessingFunctor>
    {
    public:
        virtual void ProcessWith(ProcessingFunctor& process) override
        {
            process(*this);
        }
    };

    class TestMessage2 : public MessageBase<ProcessingFunctor>
    {
    public:
        virtual void ProcessWith(ProcessingFunctor& process) override
        {
            process(*this);
        }
    };


	
	
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