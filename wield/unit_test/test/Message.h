#pragma once
#include <wield/MessageBase.h>

class ProcessingFunctor;
typedef wield::MessageBase<ProcessingFunctor> Message;

class TestMessage : public Message
{
public:
    virtual void ProcessWith(ProcessingFunctor& process) override;
};

class TestMessage2 : public Message
{
public:
    virtual void ProcessWith(ProcessingFunctor& process) override;
};

class TestMessage3 : public Message
{
public:
    virtual void ProcessWith(ProcessingFunctor& process) override;
};
