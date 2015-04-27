#pragma once
#include <cstddef>
#include <stdexcept>

#include "./Stages.hpp"
#include "./Traits.hpp"

namespace test_color {
    
    class TestMessage;
    class TestMessage2;

    class ProcessingFunctorInterface
    {
    public:
        using Message = Traits::Message;
        
        ProcessingFunctorInterface()
        {
        }

        virtual ~ProcessingFunctorInterface(){}
        
        virtual void operator()(Message&) = 0;
        virtual void operator()(TestMessage&) = 0;
        virtual void operator()(TestMessage2&) = 0;
    };

	class ProcessingFunctor : public ProcessingFunctorInterface
	{
        void operator()(Message&) override {}
        void operator()(TestMessage&) override {}
        void operator()(TestMessage2&) override {}
	};

    template<typename Dispatcher>
    class ProcessingFunctorWithDispatcherAndNextStage : public ProcessingFunctorInterface
    {
    public:
        ProcessingFunctorWithDispatcherAndNextStage(Dispatcher& dispatcher, Stages next)
            : dispatcher_(dispatcher)
            , next_(next)
        {
        }

        void operator()(Message& msg) override
        {
            dispatcher_.dispatch(next_, msg);
        }

        void operator()(TestMessage& msg) override
        {
            dispatcher_.dispatch(next_, msg);
        }

        void operator()(TestMessage2& msg) override
        {
            dispatcher_.dispatch(next_, msg);
        }

    private:
        ProcessingFunctorWithDispatcherAndNextStage(const ProcessingFunctorWithDispatcherAndNextStage&) = delete;
        ProcessingFunctorWithDispatcherAndNextStage& operator=(const ProcessingFunctorWithDispatcherAndNextStage&) = delete;

    protected:
        Dispatcher& dispatcher_;
		Stages next_;		// and here we have a nice example of how you could
                            // imagine configuring the stage pipeline on the fly.
    };
}
