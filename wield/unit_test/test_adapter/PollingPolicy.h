#pragma once

// NOTE: store behavior only on the PollingPolicy, but all data
// needed to the decision to keep polling or not on the PollingInformation
// type.
template<typename StageEnum>
class PollingPolicy
{
public:
    using StageEnumType = StageEnum;
    class PollingInformation;           // or you could make PollingInformation a
                                        // template parameter and do 'using PollingInformation = PollingInfo'
    
    inline bool continueProcessing(PollingInformation& pollingInfo)
    {
        // PollingPolicy is implementing exhaustive polling,
        // the only thing we need to track was whether on not the
        // queue was empty on our previous check.
        return pollingInfo.hadMessage();
    }
    
    inline void batchStart(PollingInformation& pollingInfo){}
    inline void batchEnd(PollingInformation& pollingInfo){}
};

template<typename StageEnum>
class PollingPolicy<StageEnum>::PollingInformation
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
