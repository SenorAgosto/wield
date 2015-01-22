#include "Message.hpp"
#include "ProcessingFunctor.hpp"

namespace test_adapter {
    
    //virtual
    void TestMessage::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }

    //virtual
    void TestMessage2::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }

    //virtual
    void TestMessage3::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }
}