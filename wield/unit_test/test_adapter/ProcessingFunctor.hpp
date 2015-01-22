#pragma once
#include "Message.hpp"
#include "Stages.hpp"

#include <stdexcept>

namespace test_adapter {
    
    class TestMessage;
    class TestMessage2;

    class ProcessingFunctorInterface
    {
    public:
        ProcessingFunctorInterface()
        {
        }

        virtual ~ProcessingFunctorInterface(){}
        
        virtual void operator()(Message& msg ) = 0;
        virtual void operator()(TestMessage& msg) = 0;
        virtual void operator()(TestMessage2& msg) = 0;
    };

    class ProcessingFunctor : public ProcessingFunctorInterface
    {
    public:
        ProcessingFunctor()
            : messageBaseCalled_(false)
            , message1Called_(false)
            , message2Called_(false)
            , messageBaseCallCount_(0)
            , message1CallCount_(0)
            , message2CallCount_(0)
        {
        }

        void operator() (Message&) override
        {
            messageBaseCalled_ = true;
            messageBaseCallCount_++;
        }

        void operator()(TestMessage&) override
        { 
            message1Called_ = true;
            message1CallCount_++;
        }

        void operator()(TestMessage2&) override
        { 
            message2Called_ = true;
            message2CallCount_++;
        }

        bool messageBaseCalled_;
        bool message1Called_;
        bool message2Called_;
        
        std::size_t messageBaseCallCount_;
        std::size_t message1CallCount_;
        std::size_t message2CallCount_;
    };


    template<typename Dispatcher>
    class ProcessingFunctorWithDispatcher : public ProcessingFunctorInterface
    {
    public:
        ProcessingFunctorWithDispatcher(Dispatcher& dispatcher)
            : messageBaseCalled_(false)
            , message1Called_(false)
            , message2Called_(false)
            , dispatcher_(dispatcher)
        {
        }

        void operator()(Message& msg) override
        {
            messageBaseCalled_ = true;
            dispatcher_.dispatch(Stages::Stage2, msg);   //forward the message to stage2
        }

        void operator()(TestMessage& msg) override
        { 
            message1Called_ = true;
            dispatcher_.dispatch(Stages::Stage2, msg);
        }

        void operator()(TestMessage2& msg) override
        { 
            message2Called_ = true;
            dispatcher_.dispatch(Stages::Stage2, msg);
        }

        bool messageBaseCalled_;
        bool message1Called_;
        bool message2Called_;

    private:
        ProcessingFunctorWithDispatcher(const ProcessingFunctorWithDispatcher&) = delete;
        ProcessingFunctorWithDispatcher& operator=(const ProcessingFunctorWithDispatcher&) = delete;

    private:
        Dispatcher& dispatcher_;
    };

    class ThrowingProcessingFunctor : public ProcessingFunctor
    {
    public:
        ThrowingProcessingFunctor()
        {
        }

        void operator()(Message&) override { throw std::runtime_error("I'm broke."); }
        void operator()(TestMessage&) override { throw std::runtime_error("I'm broke."); }
        void operator()(TestMessage2&) override { throw std::runtime_error("I'm broke."); }
    };
}
