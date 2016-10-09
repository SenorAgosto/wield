#pragma once
#include "./Traits.hpp"
#include <deque>

namespace test_batch {
    
    class ProcessingFunctorInterface;
    using Message = Traits::Message;

    class TestMessage : public Message
    {
    public:
        void processWith(ProcessingFunctorInterface& process) override;
    };

    class BatchMessage : public Message
    {
    public:
        ~BatchMessage();
        
        void processWith(ProcessingFunctorInterface& process) override;
        
        void addMessage(TestMessage* m);
        std::deque<TestMessage*>& getMessages() { return messages_; }
        
    private:
        std::deque<TestMessage*> messages_;
    };
}
