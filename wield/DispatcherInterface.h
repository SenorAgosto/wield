#pragma once

namespace wield {

    template<typename StageEnumType, class StageType>
    class DispatcherInterface
    {
    public:
        virtual void registerStage(StageEnumType stageName, StageType* stage) = 0;
    };
}
