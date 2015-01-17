#pragma once
#include <cstddef>
#include <stdexcept>

#include "./Stages.h"
#include "./Traits.h"

namespace test_raw {
    
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
        
        virtual void operator()(Message& msg) = 0;
        virtual void operator()(TestMessage& msg) = 0;
        virtual void operator()(TestMessage2& msg) = 0;
    };

	class ProcessingFunctor : public ProcessingFunctorInterface
	{
        void operator()(Message& msg) override {}
        void operator()(TestMessage& msg) override {}
        void operator()(TestMessage2& msg) override {}
	};

    // An example of a trivial garbage collection stage for freeing
    // memory allocated and assigned to raw pointers in the queue.
    // Using this GC processing functor would require a linear
    // pipeline.
    //
    // If the pipeline is non-linear (i.e. more than one stage
    // has a pointer to the message at the same time), you'll
    // have to count the occurances an address comes in and
    // free it when it hits the correct count.
    class GarbageCollector : public ProcessingFunctorInterface
    {
    public:
        void operator()(Message& msg) override
        {
            Message* m = &msg;
            delete m;
        }

        void operator()(TestMessage& msg) override
        {
            Message* m = &msg;
            delete m;
        }

        void operator()(TestMessage2& msg) override
        {
            Message* m = &msg;
            delete m;
        }
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
