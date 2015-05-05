#pragma once
#include "./Traits.hpp"

namespace test_color_minus {
    
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
