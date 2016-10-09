#pragma once
#include <cstddef>
#include <stdexcept>

#include "./Traits.hpp"

namespace test_batch {
    
    class TestMessage;
    class BatchMessage;

    class ProcessingFunctorInterface
    {
    public:
        using Message = Traits::Message;
        
        ProcessingFunctorInterface()
        {
        }

        virtual ~ProcessingFunctorInterface(){}
        
        virtual void operator()(Message& msg) = 0;
        virtual void operator()(TestMessage& msg) = 0;
        virtual void operator()(BatchMessage& msg) = 0;
    };

    // Demonstrate how to process batches of messages within current
    // processing functor with non-virtual function calls.
    class BatchProcessingFunctor : public ProcessingFunctorInterface
    {
    public:
        BatchProcessingFunctor()
            : message1Count(0)
            , message2Count(0)
        {
        }
        
        void operator()(Message&) override
        {
        }
        
        void operator()(TestMessage&) override
        {
            message1Count++;
        }
        
        void operator()(BatchMessage& msg) override
        {
            message2Count++;
            
            auto& messages = msg.getMessages();
            std::for_each(begin(messages), end(messages), [this](TestMessage* testMessage)
            {
                BatchProcessingFunctor::operator()(*testMessage);
            });
        }
        
        std::size_t message1Count;
        std::size_t message2Count;
    };
}
