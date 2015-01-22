#pragma once
#include <wield/adapters/polymorphic/QueueInterface.hpp>

namespace wield { namespace adapters { namespace polymorphic {
  
    // <QueueAdaptor> allows you to instantiate stages with different queue types.
    //
    // So for example stage1 could use a disruptor and stage2 could use a non-locking
    // linked list.
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