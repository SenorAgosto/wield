#pragma once

namespace wield
{
    template<typename StageEnum, typename Stage>
    class DispatcherBase
    {
    public:
        DispatcherBase()
        {
            memset(stages, 0, sizeof(stages));
        }

        ~DispatcherBase()
        {
        }

        void registerStage(StageEnum stageName, Stage* stage)
        {
            stages[static_cast<size_t>(stageName)] = stage;
        }

    private:
        Stage* stages[StageEnum::NumberOfStages];
    };
}