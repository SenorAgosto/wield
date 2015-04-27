#pragma once 

namespace wield { namespace details {

    // this tag type indicates the scheduler owns
    // the scheduling policy (composition).
    struct PolicyIsInternalToScheduler {};

    // this tag type indicates the scheduler expects
    // to get a reference to the scheduling policy
    // (dependency injection).
    struct PolicyIsExternalToScheduler {};


    template<class SchedulingPolicy>
    struct PolicyIsOwnedByScheduler
    {
        template<typename... Args>
        PolicyIsOwnedByScheduler(Args&&... args)
            : schedulingPolicy_(std::forward<Args>(args)...)
        {
        }

        SchedulingPolicy schedulingPolicy_;
    };

    template<class SchedulingPolicy>
    struct SchedulerHasReferenceToPolicy
    {
        template<typename... Args>
        SchedulerHasReferenceToPolicy(SchedulingPolicy& policy, Args&&...)
            : schedulingPolicy_(policy)
        {
        }

        SchedulingPolicy& schedulingPolicy_;
    };

    // primary SchedulerPolicyHolder class template, defaults to owning
    // the scheduling policy.
    template<class SchedulingPolicy, class SchedulingPolicyHolderIndicator>
    struct SchedulerPolicyHolder : public PolicyIsOwnedByScheduler<SchedulingPolicy>
    {
        template<typename... Args>
        SchedulerPolicyHolder(Args&&... args)
            : PolicyIsOwnedByScheduler<SchedulingPolicy>(std::forward<Args>(args)...)
        {
        }
    };

    // a specialization of SchedulerPolicyHolder which holds the scheduling policy
    // by reference. Used in the case the scheduling policy needs to be owned
    // outside the scheduler because the application needs to interact with it
    // directly.
    template<class SchedulingPolicy>
    struct SchedulerPolicyHolder<SchedulingPolicy, PolicyIsExternalToScheduler>
        : public SchedulerHasReferenceToPolicy<SchedulingPolicy>
    {
        template<typename... Args>
        SchedulerPolicyHolder(Args&&... args)
            : SchedulerHasReferenceToPolicy<SchedulingPolicy>(std::forward<Args>(args)...)
        {
        }
    };

}}

