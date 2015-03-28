#include "./Message.hpp"
#include "./ProcessingFunctor.hpp"

namespace test_color {
    
    //virtual
    void TestMessage::processWith(ProcessingFunctorInterface& process)
    {
        return process(*this);
    }

    //virtual
    void TestMessage2::processWith(ProcessingFunctorInterface& process)
    {
        return process(*this);
    }

    //virtual
    void TestMessage3::processWith(ProcessingFunctorInterface& process)
    {
        return process(*this);
    }
}
