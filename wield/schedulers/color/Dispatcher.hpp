#pragma once 
#include <wield/DispatcherBase.hpp>

namespace wield { namespace schedulers { namespace color {

    template<class StageEnumType, class Stage, class StageNameQueue>
    class Dispatcher : public wield::DispatcherBase<StageEnumType, Stage>
    {
    public:
        using base_t = wield::DispatcherBase<StageEnumType, Stage>;
        using Queue = StageNameQueue;

        Dispatcher(Queue& queue)
            : queue_(queue)
        {
        }

        // send a message to a stage.
        inline void dispatch(StageEnumType stageName, typename Stage::MessageType& message)
        {
            base_t::dispatch(stageName, message);
            queue_.push(stageName);
        }

        // send a copy of a message to a stage.
        template<class ConcreteMessageType>
        inline void dispatch(StageEnumType stageName, ConcreteMessageType& message, CloneMessageTagType cloneTag)
        {
            base_t::dispatch(stageName, message, cloneTag);
            queue_.push(stageName);
        }

    private:
        Queue& queue_;
    };
}}}
