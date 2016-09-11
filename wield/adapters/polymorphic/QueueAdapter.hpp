#pragma once
#include <wield/adapters/polymorphic/QueueInterface.hpp>

namespace wield { namespace adapters { namespace polymorphic {
  
    // <QueueAdaptor> allows you to instantiate stages with different queue types.
    //
    // So for example stage1 could use a disruptor and stage2 could use a non-locking
    // linked list.
    template<class MessagePtr, class QueueType>
    class QueueAdapter : public QueueInterface<MessagePtr>
    {
    public:

        template<typename... Args>
        QueueAdapter(Args&&... args);
        
        void push(const MessagePtr& message) override;
        bool try_pop(MessagePtr& message) override;
        
        std::size_t unsafe_size(void) const override;

    private:
        QueueAdapter(const QueueAdapter&) = delete;
        QueueAdapter& operator=(const QueueAdapter&) = delete;

    private:
        QueueType queue_;
    };
    

    template<class MessagePtr, class QueueType>
    template<typename... Args>
    QueueAdapter<MessagePtr, QueueType>::QueueAdapter(Args&&... args)
        : queue_(std::forward<Args>(args)...)
    {
    }

    template<class MessagePtr, class QueueType>
    inline
    void QueueAdapter<MessagePtr, QueueType>::push(const MessagePtr& message)
    {
        queue_.push(message);
    }

    template<class MessagePtr, class QueueType>
    inline
    bool QueueAdapter<MessagePtr, QueueType>::try_pop(MessagePtr& message)
    {
        return queue_.try_pop(message);
    }

    template<class MessagePtr, class QueueType>
    inline
    std::size_t QueueAdapter<MessagePtr, QueueType>::unsafe_size(void) const
    {
        return queue_.unsafe_size();
    }
}}}

