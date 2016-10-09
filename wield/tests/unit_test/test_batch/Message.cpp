#include "./Message.hpp"
#include "./ProcessingFunctor.hpp"

#include <algorithm>

namespace test_batch {
    
    //virtual
    void TestMessage::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }

    //virtual
    void BatchMessage::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }

    //-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_

    BatchMessage::~BatchMessage()
    {
        std::for_each(begin(messages_), end(messages_), [](TestMessage* m)
        {
            delete m;
        });
    }
    
    void BatchMessage::addMessage(TestMessage* m)
    {
        messages_.push_back(m);
    }
}


