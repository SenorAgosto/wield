#pragma once
#include <wield/MessageBase.h>

class ProcessingFunctorInterface;
using Message = wield::MessageBase<ProcessingFunctorInterface>;

class TestMessage : public Message
{
public:
    void processWith(ProcessingFunctorInterface& process) override;
};

class TestMessage2 : public Message
{
public:
    void processWith(ProcessingFunctorInterface& process) override;
};

class TestMessage3 : public Message
{
public:
    void processWith(ProcessingFunctorInterface& process) override;
};
