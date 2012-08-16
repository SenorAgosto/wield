#pragma once

namespace wield
{
    template<typename StageEnum, typename Stage>
    class DispatcherBase final
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

        void dispatch(StageEnum stageName, typename Stage::Message& message)
        {
            stages[static_cast<size_t>(stageName)]->push( typename Stage::Message::smartptr(&message) );
        }

    private:
        Stage* stages[StageEnum::NumberOfStages];
    };
}
