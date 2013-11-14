#pragma once
#include "Message.h"
#include "Stages.h"

#include <stdexcept>

class TestMessage;
class TestMessage2;

class ProcessingFunctor
{
public:
    ProcessingFunctor()
    {
    }

    virtual void operator()(Message& msg ) = 0;
    virtual void operator()(TestMessage& msg) = 0;
    virtual void operator()(TestMessage2& msg) = 0;
};

class TestProcessingFunctor : public ProcessingFunctor
{
public:
    TestProcessingFunctor()
        : messageBaseCalled_(false)
        , message1Called_(false)
        , message2Called_(false)
    {
    }

    virtual void operator()(Message&)
    {
        messageBaseCalled_ = true;
    }

    virtual void operator()(TestMessage&)
    { 
        message1Called_ = true;
    }

    virtual void operator()(TestMessage2&) 
    { 
        message2Called_ = true;
    }

    bool messageBaseCalled_;
    bool message1Called_;
    bool message2Called_;
};


template<typename Dispatcher>
class TestProcessingFunctorWithDispatcher : public ProcessingFunctor
{
public:
    TestProcessingFunctorWithDispatcher(Dispatcher& dispatcher)
        : messageBaseCalled_(false)
        , message1Called_(false)
        , message2Called_(false)
        , dispatcher_(dispatcher)
    {
    }

    virtual void operator()(Message& msg)
    {
        messageBaseCalled_ = true;
        dispatcher_.dispatch(Stages::Stage2, msg);   //forward the message to stage2
    }

    virtual void operator()(TestMessage& msg)
    { 
        message1Called_ = true;
        dispatcher_.dispatch(Stages::Stage2, msg);
    }

    virtual void operator()(TestMessage2& msg) 
    { 
        message2Called_ = true;
        dispatcher_.dispatch(Stages::Stage2, msg);
    }

    bool messageBaseCalled_;
    bool message1Called_;
    bool message2Called_;

private:
    // disable copy-constructor and assignment operator
    TestProcessingFunctorWithDispatcher(const TestProcessingFunctorWithDispatcher&);
    TestProcessingFunctorWithDispatcher& operator=(const TestProcessingFunctorWithDispatcher&);

private:
    Dispatcher& dispatcher_;
};

class ThrowingProcessingFunctor : public ProcessingFunctor
{
public:
    ThrowingProcessingFunctor()
    {
    }

    virtual void operator()(Message&){ throw std::runtime_error("I'm broke."); }
    virtual void operator()(TestMessage&){ throw std::runtime_error("I'm broke."); }
    virtual void operator()(TestMessage2&){ throw std::runtime_error("I'm broke."); }
};
