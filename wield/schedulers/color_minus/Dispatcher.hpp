#pragma once 
#include <wield/DispatcherBase.hpp>
#include <wield/schedulers/utils/MessageCount.hpp>

namespace wield { namespace schedulers { namespace color_minus {

    // This dispatcher is for use with the Color- scheduling policy.
    // On each dispatch of an event, a counter is incremented.
    template<class StageEnumType, class Stage>
    class Dispatcher : public wield::DispatcherBase<StageEnumType, Stage>
    {
    public:
        using base_t = wield::DispatcherBase<StageEnumType, Stage>;
        using MessageCount = utils::MessageCount<StageEnumType>;

        Dispatcher(MessageCount& stats)
            : stats_(stats)
        {
        }

        // send a message to a stage.
        inline void dispatch(StageEnumType stageName, typename Stage::MessageType& message)
        {
            base_t::dispatch(stageName, message);
            stats_.increment(stageName);
        }

        // send a copy of a message to a stage.
        template<class ConcreteMessageType>
        inline void dispatch(StageEnumType stageName, ConcreteMessageType& message, CloneMessageTagType cloneTag)
        {
            base_t::dispatch(stageName, message, cloneTag);
            stats_.increment(stageName);
        }

    private:
        MessageCount& stats_;
    };
}}}
