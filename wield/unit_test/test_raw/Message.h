#pragma once
#include "./Traits.h"

namespace test_raw {
    
    class ProcessingFunctorInterface;
    using Message = Traits::Message;
    
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
}
