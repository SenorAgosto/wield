#pragma once
#include <wield/adapters/polymorphic/QueueInterface.h>

namespace wield { namespace adapters { namespace polymorphic {
  
    template<class ProcessingFunctor, class QueueType>
    class QueueAdapter : public QueueInterface<ProcessingFunctor>
    {
    public:
        using MessagePtr = typename MessageBase<ProcessingFunctor>::smartptr;
        
        template<typename... Args>
        QueueAdapter(Args&&... args)
            : queue_(std::forward<Args>(args)...)
        {
        }
        
        void push(const MessagePtr& message) override
        {
            queue_.push(message);
        }
    
        bool try_pop(MessagePtr& message) override
        {
            return queue_.try_pop(message);
        }
        
        std::size_t unsafe_size(void) const override
        {
            return queue_.unsafe_size();
        }

    private:
        QueueAdapter(const QueueAdapter&) = delete;
        QueueAdapter& operator=(const QueueAdapter&) = delete;

    private:
        QueueType queue_;
    };
}}}