#pragma once
#include <wield/DispatcherInterface.hpp>
#include <wield/Exceptions.hpp>
#include <wield/adapters/polymorphic/QueueAdapter.hpp>

#include <cstddef>
#include <deque>

namespace wield { namespace adapters { namespace polymorphic {

    // This is a queue adapter that enables a single stage
    // to read from multiple input queues. Accomplishing this
    // requires the Concrete Concurrent Queue implementation
    // be adapted using wield::adapters::polymorphic::QueueAdapter.
    //
    // If you're obsessed with Martin Thompson's advice, and
    // believe the Single Writer Principle to be the best
    // thing since sliced bread - this adapter is for you.
    //
    // This class creates dummy stages and queues for them,
    // registering the stages with the dispatcher. Messages will
    // be dispatched to the stages handled by the
    // MultipleInputQueueAdapter. Finally, there is another
    // StageEnumType value and another stage this adapter is
    // passed to. That stage is the one you should schedule
    // and it will check the input queues in a round-robin
    // fashion.
    template<class Traits, class ConcreteQueue>
    class MultipleInputQueueAdapter : public Traits::Queue
    {
    public:
        using Message = typename Traits::Message;
        using MessagePtr = typename Message::smartptr;
        using AdaptedQueue = wield::adapters::polymorphic::QueueAdapter<MessagePtr, ConcreteQueue>;
        using ProcessingFunctor = typename Traits::ProcessingFunctor;
        using Stage = typename Traits::Stage;
        using StageEnumType = typename Traits::StageEnumType;

        MultipleInputQueueAdapter(DispatcherInterface<StageEnumType, Stage>& dispatcher, ProcessingFunctor& dummyFunctor)
            : dispatcher_(dispatcher)
            , dummyFunctor_(dummyFunctor)
            , which_(0)
            , numberOfQueues_(0)
        {
        }

        template<typename... Args>
        MultipleInputQueueAdapter& addQueue(StageEnumType stageName, Args&&... args)
        {
            queues_.emplace_back(std::forward<Args>(args)...);
            stages_.emplace_back(stageName, dispatcher_, queues_.back(), dummyFunctor_);
            numberOfQueues_++;

            return *this;
        }

        // this should never be called.
        void push(const MessagePtr& )
        {
            throw IllegallyPushedMessageOntoQueueAdapter();
        }

        // Get a message, checking the queues in a round-robin fashion.
        bool try_pop(MessagePtr& message)
        {
            bool hasMessage = false;
            std::size_t queuesPolled = 0;

            while(!hasMessage && !allQueuesPolled(queuesPolled++))
            {
                hasMessage = queues_[which_].try_pop(message);
                updateNextQueue();
            }

            return hasMessage;
        }

        // Get the unsafe size of all contained queues.
        std::size_t unsafe_size(void) const
        {
            std::size_t total = 0;
            for(auto& queue : queues_)
            {
                total += queue.unsafe_size();
            }

            return total;
        }

    private:
        // stopping condition, when we've polled all queues and they're all empty.
        inline bool allQueuesPolled(const std::size_t queuesPolled)
        {
            return queuesPolled >= numberOfQueues_;
        }

        // roll which queue we're polling (round robin).
        inline void updateNextQueue()
        {
            if(++which_ >= numberOfQueues_)
            {
                which_ = 0;
            }
        }

    private:
        std::deque<AdaptedQueue> queues_;
        std::deque<Stage> stages_;
        std::size_t which_;
        std::size_t numberOfQueues_;

        DispatcherInterface<StageEnumType, Stage>& dispatcher_;
        ProcessingFunctor& dummyFunctor_;
    };
}}}
