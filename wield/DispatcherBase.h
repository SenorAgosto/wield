#pragma once
#include <wield/Exceptions.h>
#include <cstddef>

namespace wield {

    template<typename StageEnum, class Stage>
    class DispatcherBase
    {
    public:
        static_assert(std::is_enum<StageEnum>::value, "StageEnum parameter is not an enum type.");

        using StageType = Stage;
        using StageEnumType = StageEnum;
        
        DispatcherBase()
        {
            memset(stages, 0, sizeof(stages));
        }

        ~DispatcherBase()
        {
        }

        inline void registerStage(StageEnumType stageName, StageType* stage)
        {
            if(nullptr != stages[static_cast<std::size_t>(stageName)])
            {
                throw wield::DuplicateStageRegistrationException(); 
            }

            stages[static_cast<std::size_t>(stageName)] = stage;
        }

        inline void dispatch(StageEnumType stageName, typename StageType::MessageType& message)
        {
            stages[static_cast<std::size_t>(stageName)]->push( typename StageType::MessageType::smartptr(&message) );
        }

        inline StageType& operator[](StageEnumType stageName)
        {
            return *stages[static_cast<std::size_t>(stageName)];
        }

    private:
        DispatcherBase(const DispatcherBase&) = delete;
        DispatcherBase& operator=(const DispatcherBase&) = delete;
        
    private:
        StageType* stages[static_cast<std::size_t>(StageEnum::NumberOfEntries)];
    };
}
