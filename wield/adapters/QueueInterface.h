#pragma once 
#include <wield/MessageBase.h>
#include <cstddef>

namespace wield { namespace adapters {
    
    // An abstract interface for adapting different
    // queue implementations to wield QueueBase interface.
    template<class ProcessingFunctor>
    class QueueInterface
    {
    public:
        using MessagePtr = typename MessageBase<ProcessingFunctor>::smartptr;
        virtual ~QueueInterface(){}
        
        virtual void push(const MessagePtr& message) = 0;
        virtual bool tryPop(MessagePtr& message) = 0;
        virtual std::size_t unsafeSize(void) const = 0;
    };

}}