#pragma once
#include <queue_stress/Message.hpp>

namespace queue_stress { namespace message {
    class TestMessage;
}}

namespace queue_stress {

    class ProcessingFunctorBase 
    {
    public:
        virtual ~ProcessingFunctorBase(){}

        virtual void operator()(Message&){}
        virtual void operator()(message::TestMessage&){}
    };
}

