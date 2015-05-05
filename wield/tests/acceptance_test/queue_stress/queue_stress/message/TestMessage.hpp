#pragma once 
#include <queue_stress/Message.hpp>
#include <queue_stress/ProcessingFunctorBase.hpp>

#include <cstddef>

namespace queue_stress { namespace message {

    class TestMessage : public Message 
    {
    public:
        TestMessage(const std::size_t sequenceNumber)
            : sequenceNumber_(sequenceNumber)
        {
        }

        std::size_t sequenceNumber()
        {
            return sequenceNumber_;
        }

        void processWith(ProcessingFunctorBase& pf) override 
        {
            pf(*this);
        }

    private:
        std::size_t sequenceNumber_;
    };
}}
