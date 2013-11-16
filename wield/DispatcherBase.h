#pragma once
#include <wield/Exceptions.h>
#include <cstddef>

namespace wield {

    template<typename StageEnum, typename Stage>
    class DispatcherBase final
    {
    public:
        static_assert(std::is_enum<StageEnum>::value, "StageEnum parameter is not an enum type.");
        
        using stage_t = Stage;
        using stage_enum_t = StageEnum;
        
        DispatcherBase()
        {
            memset(stages, 0, sizeof(stages));
        }

        ~DispatcherBase()
        {
        }

        inline void registerStage(StageEnum stageName, Stage* stage)
        {
            if(nullptr != stages[static_cast<std::size_t>(stageName)])
            {
                throw wield::DuplicateStageRegistrationException(); 
            }

            stages[static_cast<std::size_t>(stageName)] = stage;
        }

        inline void dispatch(StageEnum stageName, typename Stage::message_t& message)
        {
            stages[static_cast<std::size_t>(stageName)]->push( typename Stage::message_t::smartptr(&message) );
        }

        inline Stage& operator[](StageEnum stageName)
        {
            return *stages[static_cast<std::size_t>(stageName)];
        }

    private:
        DispatcherBase(const DispatcherBase&) = delete;
        DispatcherBase& operator=(const DispatcherBase&) = delete;
        
    private:
        Stage* stages[static_cast<std::size_t>(StageEnum::NumberOfEntries)];
    };
}
