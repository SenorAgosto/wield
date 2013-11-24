#pragma once
#include <mutex>
#include <queue>

#include "Traits.h"

namespace test_adapter {
    
    // A basic concurrent queue implementation using mutex to protect std::queue.
    class SimpleConcurrentQueue
    {
    public:
        using MessagePtr = Traits::MessagePtr;
        
        void push(const MessagePtr& message)
        {
            std::lock_guard<std::mutex> lock(lock_);
            queue_.push(message);
        }
        
        bool try_pop(MessagePtr& message)
        {
            std::lock_guard<std::mutex> lock(lock_);
            
            if(!queue_.empty())
            {
                // front returns &, make a copy to increment ref count before pop().
                MessagePtr m = queue_.front();
                message = std::move(m);
                
                queue_.pop();
                return true;
            }
            
            return false;
        }
        
        std::size_t unsafe_size(void) const
        {
            std::lock_guard<std::mutex> lock(lock_);
            return queue_.size();
        }
        
    private:
        mutable std::mutex lock_;
        std::queue<MessagePtr> queue_;
    };
}