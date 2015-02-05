#pragma once
#include <wield/DispatcherInterface.hpp>

#include <cstddef>
#include <deque>

namespace wield { namespace details {

    // Helper class that recursively creates stages and adds them to a vector.
    template<class Traits, class ConcreteQueue, std::size_t StageLabel, std::size_t NumberOfStages>
    class DummyStageCreatorImpl
        : public DummyStageCreatorImpl<Traits, ConcreteQueue, StageLabel + 1, NumberOfStages - 1>
    {
    public:
        using ProcessingFunctor = typename Traits::ProcessingFunctor;
        using StageEnumType = typename Traits::StageEnumType;
        using StageType = typename Traits::Stage;
        using QueueType = typename Traits::Queue;
        
        using base = DummyStageCreatorImpl<Traits, ConcreteQueue, StageLabel + 1, NumberOfStages - 1>;

        DummyStageCreatorImpl(DispatcherInterface<StageEnumType, StageType>& dispatcher, ProcessingFunctor& processingFunctor, std::deque<ConcreteQueue>& queues, std::deque<StageType>& stages)
            : base(dispatcher, processingFunctor, queues, stages)
        {
            const StageEnumType stageName = static_cast<StageEnumType>(StageLabel);
            stages.emplace_back(stageName, dispatcher, queues[NumberOfStages - 1], processingFunctor);
        }
    };

    // A specialization to stop recursion.
    template<class Traits, class ConcreteQueue, std::size_t StageLabel>
    class DummyStageCreatorImpl<Traits, ConcreteQueue, StageLabel, 0>
    {
    public:
        using ProcessingFunctor = typename Traits::ProcessingFunctor;
        using StageEnumType = typename Traits::StageEnumType;
        using StageType = typename Traits::Stage;
        using QueueType = typename Traits::Queue;

        DummyStageCreatorImpl(DispatcherInterface<StageEnumType, StageType>& dispatcher, ProcessingFunctor& processingFunctor, std::deque<ConcreteQueue>& queues, std::deque<StageType>& stages)
        {
        }
    };
}}
