#include "./platform/UnitTestSupport.hpp"
#include <wield/schedulers/RoundRobin.hpp>
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include "./test/Stages.hpp"
#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"

#include <cstddef> 

namespace {

    // NOTE: both scheduling policies can't currently require a specialized
    // dispatcher, one has to be able to use a general dispatcher.
    struct MultipleSchedulersFixture
    {
        MultipleSchedulersFixture()
            : policy1StageMask{{0, 0, 1}}    // only schedule Stage3
            , policy2StageMask{{1, 1, 0}}    // only schedule Stage1 & Stage2
            , policy1(dispatcher, policy1StageMask)
            , policy2(dispatcher, policy2StageMask)
            , s1(test::Stages::Stage1, dispatcher, queue, f)
            , s2(test::Stages::Stage2, dispatcher, queue, f)
            , s3(test::Stages::Stage3, dispatcher, queue, f)
        {
        }
    
        using Dispatcher = test::Traits::Dispatcher;
        using Stage = test::Traits::Stage;
        using PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<test::Stages>;
        using SchedulingPolicy = wield::schedulers::RoundRobin<test::Stages, Dispatcher, Stage, PollingPolicy>;
        using Queue = test::Traits::Queue;
        using Message = test::Traits::Message;
        
        using RoundRobinMask = SchedulingPolicy::MaxConcurrencyContainer;
        RoundRobinMask policy1StageMask;
        RoundRobinMask policy2StageMask;
        
        Dispatcher dispatcher;
        SchedulingPolicy policy1;
        SchedulingPolicy policy2;   // I'm using Round Robin twice because it's easy to
                                    // demonstrate movement between stages without a lot of setup.
        
        Queue queue;
        test::ProcessingFunctor f;

        Stage s1;
        Stage s2;
        Stage s3;
    };
    
    // Here we just demonstrate the policies work correctly, but
    // in an application, you'd instance N-SchedulerBase's, one with
    // each scheduling policy you want and call .start() on each,
    // .stop() on each, and .join() on each.
    //
    // NOTE: it is highly recommend there be no overlap in the scheduling masks.
    // whether or not overlap is an issue is application specific.
    TEST_FIXTURE(MultipleSchedulersFixture, verifySchedulingPoliciesAdhereToStageMasks)
    {
        static const std::size_t thread0 = 0;
        static const std::size_t thread1 = 1;
        
        // policy1 always schedules Stage3
        CHECK_EQUAL(&s3, &policy1.nextStage(thread0));
        CHECK_EQUAL(&s3, &policy1.nextStage(thread0));
        CHECK_EQUAL(&s3, &policy1.nextStage(thread0));
        
        CHECK_EQUAL(&s1, &policy2.nextStage(thread1));
        CHECK_EQUAL(&s2, &policy2.nextStage(thread1));
        CHECK_EQUAL(&s1, &policy2.nextStage(thread1));
        CHECK_EQUAL(&s2, &policy2.nextStage(thread1));
    }
}