#pragma once

#include <wield/adapters/polymorphic/QueueAdapter.hpp>
#include <wield/adapters/polymorphic/QueueInterface.hpp>
#include <wield/DispatcherInterface.hpp>
#include <wield/Exceptions.hpp>

#include <wield/details/QueueCreator.hpp>
#include <wield/details/DummyStageCreator.hpp>

#include <vector>

namespace wield { namespace adapters { namespace polymorphic {

    // This is a queue adapter that enables a single stage
    // to read from multiple input queues.
    //
    // If you're obsessed with Martin Thompson's advice, and
    // believe the Single Writer Principle to be the best
    // thing since sliced bread - this adapter is for you.
    //
    // This version assumes that only some of the stages in
    // the stage graph need multiple inputs, and hence forces
    // the QueueType to be:
    //      wield::adapters::polymorphic::QueueInterface<MessagePtr>.
    //
    // See also: wield::adpaters::MultiInputQueueAdapter,
    // which will mirror this implementation but assume all
    // stages in the stage graph use multiple inputs... hence
    // avoid virtual calls.
    //
    // This class creates dummy stages and queues for them,
    // registering the stages with the dispatcher. Messages will
    // be dispatched to the stages handled by the
    // MultipleInputQueueAdapter. Finally, there is another
    // StageEnumType value and another stage this adapter is
    // passed to. That stage is the one you should schedule
    // and it will check the input queues in a round-robin
    // fashion.
    template<class Traits, class ConcreteQueue, class StageEnumType, StageEnumType Start, StageEnumType End>
    class MultipleInputQueueAdapter : public Traits::Queue
    {
    public:
        using MessagePtr = typename Traits::MessagePtr;
        using AdaptedQueue = QueueAdapter<MessagePtr, ConcreteQueue>;

        using ProcessingFunctor = typename Traits::ProcessingFunctor;
        using Stage = typename Traits::Stage;

        static const std::size_t numberOfStages = static_cast<std::size_t>(End) - static_cast<std::size_t>(Start);

        // We need the dispatcher so the dummy stages
        // can be register with the desired labels.
        // We need the dummy processingFunctor so
        // dummy stages can be constructed.
        MultipleInputQueueAdapter(DispatcherInterface<StageEnumType, Stage>& dispatcher, ProcessingFunctor& dummyFunctor)
            : which_(0)
        {
            details::QueueCreator<AdaptedQueue, numberOfStages> queueCreator(queues_);
            details::DummyStageCreator<Traits, StageEnumType, Start, End> stageCreator(dispatcher, dummyFunctor, queues_, dummyStages_);
        }

        // Calling push is an error, we throw to aid in debugging.
        void push(const MessagePtr& message) override
        {
            throw IllegallyPushedMessageOntoQueueAdapter();
        }

        // Get a message, checking the queues in a round-robin fashion.
        bool try_pop(MessagePtr& message) override
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
        std::size_t unsafe_size(void) const override
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
        bool allQueuesPolled(const std::size_t queuesPolled)
        {
            return queuesPolled >= numberOfStages;
        }

        // roll which queue we're polling (round robin).
        void updateNextQueue()
        {
            if(++which_ >= numberOfStages)
            {
                which_ = 0;
            }
        }

    private:
        // the adapted concrete queues which messages get dispatched to through the dummy stages.
        std::deque<AdaptedQueue> queues_;
        std::deque<Stage> dummyStages_;

        // the variable for determining which queue to poll next (round-robin).
        std::size_t which_;
    };

}}}