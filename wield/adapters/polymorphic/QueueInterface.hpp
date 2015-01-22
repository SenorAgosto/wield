#pragma once 
#include <wield/MessageBase.hpp>
#include <cstddef>

namespace wield { namespace adapters { namespace polymorphic {
    
    // An abstract interface for adapting different
    // queue implementations to wield QueueBase interface.
    template<class ProcessingFunctor>
    class QueueInterface
    {
    public:
        using MessagePtr = typename MessageBase<ProcessingFunctor>::smartptr;
        virtual ~QueueInterface(){}
        
        virtual void push(const MessagePtr& message) = 0;
        virtual bool try_pop(MessagePtr& message) = 0;
        virtual std::size_t unsafe_size(void) const = 0;
    };

}}}