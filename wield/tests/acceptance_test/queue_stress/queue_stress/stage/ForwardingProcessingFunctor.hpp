#pragma once 
#include <queue_stress/Dispatcher.hpp>
#include <queue_stress/ProcessingFunctorBase.hpp>
#include <queue_stress/Stages.hpp>

#include <queue_stress/message/TestMessage.hpp>
#include <queue_stress/details/SequenceArbiter.hpp>

#include <iostream>
#include <limits>
#include <set>

namespace queue_stress { namespace stage {

    class ForwardingProcessingFunctor : public ProcessingFunctorBase
    {
    public:
        ForwardingProcessingFunctor(Dispatcher& dispatcher, Stages next)
            : arbiter_(errorPolicy_)
            , dispatcher_(dispatcher)
            , next_(next)
        {
        }

        void operator()(message::TestMessage& message) override
        {
            if(arbiter_.validate(0, message.sequenceNumber()))
            {
                dispatcher_.dispatch(next_, message);
            }
            else
            {
                std::cerr << "Error: arbiter rejected sequence number: " << message.sequenceNumber() << std::endl;
            }
        }

    private:
        details::ArbiterErrorReportingPolicy errorPolicy_;
        details::SequenceArbiter arbiter_;

        Dispatcher& dispatcher_;
        const Stages next_;
    };
}}

