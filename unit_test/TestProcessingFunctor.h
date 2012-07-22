#pragma once
#include "Message.h"

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
        : messageBaseCalled(false)
        , message1Called(false)
        , message2Called(false)
    {
    }

    virtual void operator()(Message& msg){ messageBaseCalled = true; }
    virtual void operator()(TestMessage& msg){ message1Called = true; }
    virtual void operator()(TestMessage2& msg) { message2Called = true;}

    bool messageBaseCalled;
    bool message1Called;
    bool message2Called;
};
