#pragma once 

#include <array>
#include <cstddef>

namespace wield { namespace schedulers { namespace utils {

    // This is a helper class to keep track of estimated
    // queue depths. Each time an event is enqueued,
    // call <increment>. When you need an estimate
    // of the current queue depth, call <estimatedDepth>
    // Once you wish to 'observe' the depth, update
    // the previous observed counter value to reset
    // the estimated queue depth to 0 by calling
    // <updatePrevious>.
    template<class StageEnumType>
    class MessageCount
    {
    public:
        MessageCount();

        // update the current count by @count for @stage
        void increment(const StageEnumType stage, const std::size_t count = 1);

        // update the previous observed depth for @stage
        void updatePrevious(const StageEnumType stage);

        // return the current estimated queue depth for @stage
        std::size_t estimatedDepth(const StageEnumType stage);

        // return the stage with the most estimated work.
        StageEnumType highwaterStage();

        // reset all counters to 0.
        void reset();

    private:
        static const std::size_t NumberOfStages = static_cast<std::size_t>(StageEnumType::NumberOfEntries);

        std::array<std::size_t, NumberOfStages> previous_;
        std::array<std::size_t, NumberOfStages> current_;
    };


    template<class StageEnumType>
    MessageCount<StageEnumType>::MessageCount()
    {
        reset();
    }

    template<class StageEnumType>
    void MessageCount<StageEnumType>::increment(const StageEnumType stage, const std::size_t count)
    {
        const std::size_t stageIndex = static_cast<std::size_t>(stage);
        current_[stageIndex] += count;
    }

    template<class StageEnumType>
    void MessageCount<StageEnumType>::updatePrevious(const StageEnumType stage)
    {
        const std::size_t stageIndex = static_cast<std::size_t>(stage);
        previous_[stageIndex] = current_[stageIndex];
    }

    template<class StageEnumType>
    std::size_t MessageCount<StageEnumType>::estimatedDepth(const StageEnumType stage)
    {
        const std::size_t stageIndex = static_cast<std::size_t>(stage);

        auto previousValue = previous_[stageIndex];
        auto currentValue = current_[stageIndex];

        return (previousValue < currentValue) ? currentValue - previousValue : 0;
    }

    template<class StageEnumType>
    StageEnumType MessageCount<StageEnumType>::highwaterStage()
    {
        StageEnumType stage = static_cast<StageEnumType>(0);
        std::size_t highwaterMark = 0;

        for(std::size_t i = 0, end = current_.size(); i < end; ++i)
        {
            const std::size_t stageDepth = estimatedDepth(static_cast<StageEnumType>(i));
            if(stageDepth > highwaterMark)
            {
                stage = static_cast<StageEnumType>(i);
                highwaterMark = stageDepth;
            }
        }

        return stage;
    }

    template<class StageEnumType>
    void MessageCount<StageEnumType>::reset()
    {
        for(auto& p : previous_) { p = 0; }
        for(auto& c : current_) { c = 0; }
    }
}}}
