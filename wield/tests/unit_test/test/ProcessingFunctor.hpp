#pragma once
#include <cstddef>
#include <stdexcept>

#include "./Traits.hpp"

namespace test {
    
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
            , messageBaseCallCount_(0)
            , message1CallCount_(0)
            , message2CallCount_(0)
            , dispatcher_(dispatcher)
        {
        }

        void operator()(Message& msg) override
        {
            messageBaseCallCount_++;
            messageBaseCalled_ = true;
            dispatcher_.dispatch(Stages::Stage2, msg);   //forward the message to stage2
        }

        void operator()(TestMessage& msg) override
        {
            message1CallCount_++;
            message1Called_ = true;
            dispatcher_.dispatch(Stages::Stage2, msg);
        }

        void operator()(TestMessage2& msg) override
        {
            message2CallCount_++;
            message2Called_ = true;
            dispatcher_.dispatch(Stages::Stage2, msg);
        }

        bool messageBaseCalled_;
        bool message1Called_;
        bool message2Called_;

        std::size_t messageBaseCallCount_;
        std::size_t message1CallCount_;
        std::size_t message2CallCount_;
        
    private:
        ProcessingFunctorWithDispatcher(const ProcessingFunctorWithDispatcher&) = delete;
        ProcessingFunctorWithDispatcher& operator=(const ProcessingFunctorWithDispatcher&) = delete;

    protected:
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

    // This processing functor handles one specific message type,
    // ignoring the remaining types, allowing the base class
    // to handle them with default function implementations.
    class PartialProcessingFunctor : public ProcessingFunctor
    {
    public:
        PartialProcessingFunctor()
            : hazah_(0)
        {
        }

        void operator()(TestMessage2&) override
        {
            hazah_++;
        }

        std::size_t hazah_;
    };
}
