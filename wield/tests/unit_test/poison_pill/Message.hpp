#pragma once
#include "./Traits.hpp"

#include <atomic>
#include <functional>
#include <string>

namespace poison_pill {
    
    class ProcessingFunctorInterface;
    using Message = Traits::Message;

    class LineMessage : public Message
    {
    public:
        LineMessage(const std::string& line);
        void processWith(ProcessingFunctorInterface& process) override;
        
    private:
        std::string line_;
    };

    class PoisonPill : public Message
    {
    public:
        
        // takes a callable which will stop the application
        PoisonPill(std::function<void()> done)
            : count_(0)
            , done_(done)
        {
        }
        
        void done()
        {
            if(++count_ == static_cast<std::size_t>(Stages::NumberOfEntries))
            {
                done_();
            }
        }
        
        void processWith(ProcessingFunctorInterface& process) override;
    
    private:
        std::atomic<std::size_t> count_;    // in this case, we're going to use atomic because the pipeline forks.
        std::function<void()> done_;
    };
}
