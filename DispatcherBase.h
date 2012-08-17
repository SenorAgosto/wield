#pragma once
#include <exception>

namespace wield
{
    template<typename StageEnum, typename Stage>
    class DispatcherBase final
    {
    public:
        typedef Stage stage_t;
        typedef StageEnum stage_enum_t;

        DispatcherBase()
        {
            memset(stages, 0, sizeof(stages));
        }

        ~DispatcherBase()
        {
        }

        inline void registerStage(StageEnum stageName, Stage* stage)
        {
            if(nullptr != stages[static_cast<size_t>(stageName)])
            {
                throw std::runtime_error("DispatcherBase::registerStage() duplicate registration of stage.");
            }

            stages[static_cast<size_t>(stageName)] = stage;
        }

        inline void dispatch(StageEnum stageName, typename Stage::message_t& message)
        {
            stages[static_cast<size_t>(stageName)]->push( typename Stage::message_t::smartptr(&message) );
        }

        inline Stage& operator[](StageEnum stageName)
        {
            return *stages[static_cast<size_t>(stageName)];
        }

    private:
        Stage* stages[StageEnum::NumberOfStages];
    };
}
