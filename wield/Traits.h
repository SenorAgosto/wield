#pragma once
#include <wield/DispatcherBase.h>
#include <wield/MessageBase.h>
#include <wield/SchedulerBase.h>
#include <wield/StageBase.h>

namespace wield {
    
    // ClientDefinedTraits contains types the client *must* define.
    // If you wish to override particular classes, like
    // the DispatcherType, you'll need define similar struct
    // with your desired types.
    template<class ClientDefinedTraits>
    struct Traits
    {
        using StageEnumType = typename ClientDefinedTraits::StageEnumType;
        using ProcessingFunctor = typename ClientDefinedTraits::ProcessingFunctor;
        
        using Message = MessageBase<ProcessingFunctor>;
        using MessagePtr = typename Message::smartptr;
        
        using Queue = typename ClientDefinedTraits::template QueueType<MessagePtr>;
        
        // we keep this indirection in-case you want to replace the dispatcher implementation
        // with your own.
        template<typename StageEnum, typename Stage>
        using DispatcherType = wield::DispatcherBase<StageEnum, Stage>;
        
        using Stage = wield::StageBase<StageEnumType, ProcessingFunctor, Message, Queue>;
        using Dispatcher = DispatcherType<StageEnumType, Stage>;
        
        using SchedulingPolicy = typename ClientDefinedTraits::template SchedulingPolicy<Dispatcher>;
        using Scheduler = wield::SchedulerBase<SchedulingPolicy>;
        
        // expose extra fields on ClientDefinedTraits as ApplicationTraits.
        using ApplicationTraits = ClientDefinedTraits;
    };
}
