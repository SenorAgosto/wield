#include "./platform/UnitTestSupport.hpp"
#include <wield/schedulers/SRPT.hpp>
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include "./test/Stages.hpp"
#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"

namespace {

    struct SRPTFixture
    {
        SRPTFixture()
            : schedulingPolicy(dispatcher)
        {
        }

        using Dispatcher = test::Traits::Dispatcher;
        using Stage = test::Traits::Stage;
        using PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<test::Stages>;
        using SchedulingPolicy = wield::schedulers::SRPT<test::Stages, Dispatcher, Stage, PollingPolicy>;
        using Queue = test::Traits::Queue;

        Dispatcher dispatcher;
        SchedulingPolicy schedulingPolicy;
    };

    TEST_FIXTURE(SRPTFixture, verifyInstantiationOfRoundRobin)
    {
    }

    TEST_FIXTURE(SRPTFixture, verifyShortestRemainingProcessingTimeVisitationOrderWithNoWorkInStages)
    {
        test::ProcessingFunctor pf;
        Queue q;

        Stage s1(test::Stages::Stage1, dispatcher, q, pf);
        Stage s2(test::Stages::Stage2, dispatcher, q, pf);
        Stage s3(test::Stages::Stage3, dispatcher, q, pf);
        
        static const bool noMessages = false;
        
        PollingPolicy::PollingInformation pollingInfo(0, test::Stages::NumberOfEntries);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(0));
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(0));
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s1, &schedulingPolicy.nextStage(0));
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(0));
    }
    
    TEST_FIXTURE(SRPTFixture, verifySRPTWithWorkInStages)
    {
        test::ProcessingFunctor pf;
        Queue q;
        
        Stage s1(test::Stages::Stage1, dispatcher, q, pf);
        Stage s2(test::Stages::Stage2, dispatcher, q, pf);
        Stage s3(test::Stages::Stage3, dispatcher, q, pf);

        static const bool noMessages = false;
        static const bool hadMessages = true;
        static const std::size_t thread0 = 0;
        
        PollingPolicy::PollingInformation pollingInfo(thread0, test::Stages::NumberOfEntries);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage3);
        pollingInfo.incrementMessageCount(hadMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage2);
        pollingInfo.incrementMessageCount(hadMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage3);
        pollingInfo.incrementMessageCount(hadMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage2);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s1, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage1);
        pollingInfo.incrementMessageCount(hadMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(0));
    }
    
    TEST_FIXTURE(SRPTFixture, verifySRPTOrderWithTwoThreadsAndNoWork)
    {
        test::ProcessingFunctor pf;
        Queue q;
        
        Stage s1(test::Stages::Stage1, dispatcher, q, pf);
        Stage s2(test::Stages::Stage2, dispatcher, q, pf);
        Stage s3(test::Stages::Stage3, dispatcher, q, pf);

        static const bool noMessages = false;
        static const bool hadMessages = true;
        
        static const std::size_t thread0 = 0;
        static const std::size_t thread1 = 1;
        
        PollingPolicy::PollingInformation pollingInfo(thread0, test::Stages::NumberOfEntries);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread1, test::Stages::NumberOfEntries);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread1)); // thread0 is on Stage3, goto Stage2
        
        pollingInfo = PollingPolicy::PollingInformation(thread1, test::Stages::Stage2);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s1, &schedulingPolicy.nextStage(thread1)); // no work Stage2, goto Stage1
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage3);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread0));  // no work Stage3, goto Stage2
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage2);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(thread0)); // thread1 on Stage1, goto Stage3
        
        pollingInfo = PollingPolicy::PollingInformation(thread1, test::Stages::Stage1);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread1)); // thread0 on Stage3, goto Stage2
    }
    
    TEST_FIXTURE(SRPTFixture, verifySRPTOrderWithTwoThreadsAndWork)
    {
        test::ProcessingFunctor pf;
        Queue q;
        
        Stage s1(test::Stages::Stage1, dispatcher, q, pf);
        Stage s2(test::Stages::Stage2, dispatcher, q, pf);
        Stage s3(test::Stages::Stage3, dispatcher, q, pf);

        static const bool noMessages = false;
        static const bool hadMessages = true;
        
        static const std::size_t thread0 = 0;
        static const std::size_t thread1 = 1;
        
        PollingPolicy::PollingInformation pollingInfo(thread0, test::Stages::NumberOfEntries);
        pollingInfo.incrementMessageCount(noMessages);

        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(thread0));
        
        pollingInfo = PollingPolicy::PollingInformation(thread1, test::Stages::NumberOfEntries);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread1)); // thread0 on Stage3, goto Stage2
        
        pollingInfo = PollingPolicy::PollingInformation(thread1, test::Stages::Stage2);
        pollingInfo.incrementMessageCount(hadMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s2, &schedulingPolicy.nextStage(thread1)); // thread0 on Stage3, goes back to Stage2
        
        pollingInfo = PollingPolicy::PollingInformation(thread0, test::Stages::Stage3);
        pollingInfo.incrementMessageCount(hadMessages);
        
        schedulingPolicy.batchEnd(pollingInfo);
        CHECK_EQUAL(&s1, &schedulingPolicy.nextStage(thread0));  // thread1 on Stage2, go to Stage1
        
        pollingInfo = PollingPolicy::PollingInformation(thread1, test::Stages::Stage2);
        pollingInfo.incrementMessageCount(noMessages);
        
        schedulingPolicy.batchEnd(pollingInfo); // thread0 on Stage1, goto Stage3
        CHECK_EQUAL(&s3, &schedulingPolicy.nextStage(thread1));
    }
}
