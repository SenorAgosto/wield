#pragma once 
#include <wield/DispatcherBase.hpp>

namespace wield { namespace schedulers { namespace color {

    template<class StageEnumType, class Stage, class StageNameQueue>
    class Dispatcher : public wield::DispatcherBase<StageEnumType, Stage>
    {
    public:
        using base_t = wield::DispatcherBase<StageEnumType, Stage>;
        using Queue = StageNameQueue;

        Dispatcher(Queue& queue);

        // send a message to a stage.
        void dispatch(StageEnumType stageName, typename Stage::MessageType& message);

        // send a copy of a message to a stage.
        template<class ConcreteMessageType>
        void dispatch(StageEnumType stageName, ConcreteMessageType& message, CloneMessageTagType cloneTag);

    private:
        Queue& queue_;
    };
    
    
    template<class StageEnumType, class Stage, class StageNameQueue>
    Dispatcher<StageEnumType, Stage, StageNameQueue>::Dispatcher(Queue& queue)
        : queue_(queue)
    {
    }

    template<class StageEnumType, class Stage, class StageNameQueue>
    inline
    void Dispatcher<StageEnumType, Stage, StageNameQueue>::dispatch(StageEnumType stageName, typename Stage::MessageType& message)
    {
        base_t::dispatch(stageName, message);
        queue_.push(stageName);
    }

    template<class StageEnumType, class Stage, class StageNameQueue>
    template<class ConcreteMessageType>
    inline
    void Dispatcher<StageEnumType, Stage, StageNameQueue>::dispatch(StageEnumType stageName, ConcreteMessageType& message, CloneMessageTagType cloneTag)
    {
        base_t::dispatch(stageName, message, cloneTag);
        queue_.push(stageName);
    }
    
}}}
