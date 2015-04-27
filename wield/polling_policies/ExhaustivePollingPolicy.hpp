#pragma once
#include <cstddef>

namespace wield { namespace polling_policies {
    
    // NOTE: this polling policy implements exhaustive polling of queues. I.e.,
    // a thread will stay at a stage until the queue is empty.
    // The polling policy logic is on this class, and the data is stored
    // on the PollingInformation class, which should be used as the
    // SchedulingPolicy::PollingInformation implementation when using
    // this polling policy.
    template<typename StageEnum>
    class ExhaustivePollingPolicy
    {
    public:
        using StageEnumType = StageEnum;
        class PollingInformation;           // or you could make PollingInformation a
                                            // template parameter and do 'using PollingInformation = PollingInfo'
        
        inline bool continueProcessing(PollingInformation& pollingInfo)
        {
            // ExhaustivePollingPolicy is implementing exhaustive polling
            // of the stage queue, the only thing we need to track was whether
            // or not the queue was empty on our previous check.
            return pollingInfo.hadMessage();
        }
        
        inline void batchStart(PollingInformation&){}
        inline void batchEnd(PollingInformation&){}
    };

    template<typename StageEnum>
    class ExhaustivePollingPolicy<StageEnum>::PollingInformation
    {
    public:
        // this constructor signature and incrementMessageCount are required.
        inline PollingInformation(const std::size_t /*thread_id*/, const StageEnumType /*stageName*/)
            : hadMessage_(true)
        {
        }
        
        inline void incrementMessageCount(bool hadMessage)
        {
            hadMessage_ = hadMessage;
        }

        // non-required interface.
        inline bool hadMessage(void) const { return hadMessage_; }
        
    private:
        bool hadMessage_;
    };
}}

