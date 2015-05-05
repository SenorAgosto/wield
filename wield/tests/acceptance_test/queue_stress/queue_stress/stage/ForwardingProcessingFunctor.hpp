#pragma once 
#include <queue_stress/Dispatcher.hpp>
#include <queue_stress/ProcessingFunctorBase.hpp>
#include <queue_stress/Stages.hpp>

#include <queue_stress/message/TestMessage.hpp>

#include <iostream>
#include <limits>
#include <set>

namespace queue_stress { namespace stage {

    class ForwardingProcessingFunctor : public ProcessingFunctorBase
    {
    public:
        ForwardingProcessingFunctor(Dispatcher& dispatcher, Stages next)
            : dispatcher_(dispatcher)
            , next_(next)
            , previousSequenceNumber_(std::numeric_limits<std::size_t>::max())
        {
        }

        void operator()(message::TestMessage& message) override
        {
            auto nextExpectedSequenceNumber = previousSequenceNumber_ + 1;
            auto sequenceNumber = message.sequenceNumber();

            if(sequenceNumber != nextExpectedSequenceNumber)
            {
                std::cerr 
                    << "There's a problem with sequence numbers. Expected ("
                    << nextExpectedSequenceNumber 
                    << "), got (" 
                    << sequenceNumber 
                    << ")"
                    << std::endl;
            }

            previousSequenceNumber_ = sequenceNumber;
            dispatcher_.dispatch(next_, message);
        }

    private:
        Dispatcher& dispatcher_;
        Stages next_;

        std::size_t previousSequenceNumber_;
    };
}}

