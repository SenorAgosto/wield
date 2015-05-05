#pragma once
#include <mutex>
#include <queue>

#include "./Traits.hpp"

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
                message = std::move(queue_.front());
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

