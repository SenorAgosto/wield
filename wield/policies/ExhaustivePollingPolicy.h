#pragma once

namespace wield { namespace policies {
    
    // NOTE: store behavior only on the PollingPolicy, but all data
    // needed to the decision to keep polling or not on the PollingInformation
    // type.
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
        
        inline void batchStart(PollingInformation& pollingInfo){}
        inline void batchEnd(PollingInformation& pollingInfo){}
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

