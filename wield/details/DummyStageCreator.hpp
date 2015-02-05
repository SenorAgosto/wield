#pragma once
#include <wield/details/DummyStageCreatorImpl.hpp>
#include <wield/DispatcherInterface.hpp>

#include <cstddef>
#include <deque>

namespace wield { namespace details {

    // A helper class that creates the number of dummy stages we need,
    // adding them to a deque<StageType>.
    //
    // The nasty template meta programming is hidden in the implementation header.
    template<class Traits, class StageEnumType, StageEnumType StartStage, StageEnumType EndStage>
    class DummyStageCreator
    {
    public:
        static_assert( static_cast<std::size_t>(EndStage) >= static_cast<std::size_t>(StartStage), "End must be larger than Start.");

        using ProcessingFunctor = typename Traits::ProcessingFunctor;
        using Stage = typename Traits::Stage;

        template<class ConcreteQueue>
        DummyStageCreator(DispatcherInterface<StageEnumType, Stage>& dispatcher, ProcessingFunctor& processingFunctor, std::deque<ConcreteQueue>& queues, std::deque<Stage>& stages)
        {
            static const std::size_t numberOfStages = static_cast<std::size_t>(EndStage) - static_cast<std::size_t>(StartStage);

            DummyStageCreatorImpl<Traits, ConcreteQueue, static_cast<std::size_t>(StartStage), numberOfStages>
            creator(dispatcher, processingFunctor, queues, stages);
        }
    };
}}
