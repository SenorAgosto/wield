#pragma once
#include <cstddef>
#include <stdexcept>

#include "./Traits.hpp"

namespace poison_pill {
    
    class LineMessage;
    class PoisonPill;

    class ProcessingFunctorInterface
    {
    public:
        using Message = Traits::Message;
        
        ProcessingFunctorInterface()
        {
        }

        virtual ~ProcessingFunctorInterface(){}
        
        virtual void operator()(Message& msg) = 0;
        virtual void operator()(LineMessage& msg) = 0;
        virtual void operator()(PoisonPill& msg) = 0;
    };

    class BaseProcessingFunctor : public ProcessingFunctorInterface
    {
    public:
        using Message = Traits::Message;
        
        BaseProcessingFunctor()
            : lineCount(0)
        {
        }
        
        void operator()(Message&) override {}
        void operator()(LineMessage&) override { lineCount++; }
        void operator()(PoisonPill& msg) override { msg.done(); }
        
        std::size_t lineCount;
    };
    
    template<typename Dispatcher>
    class ProcessingFunctorWithDispatcherAndNextStage : public BaseProcessingFunctor
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
        
        void operator()(LineMessage& msg) override
        {
            dispatcher_.dispatch(next_, msg);
        }

        void operator()(PoisonPill& msg) override
        {
            msg.done();
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
