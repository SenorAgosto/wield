#include "./platform/UnitTestSupport.h"

#include "./test_raw/Traits.h"
#include "./test_raw/Message.h"
#include "./test_raw/ProcessingFunctor.h"
#include "./test_raw/Scheduler.h"
#include "./test_raw/Stages.h"

namespace {

    using namespace test_raw;

    using Dispatcher = Traits::Dispatcher;
    using Message = Traits::Message;
    using Scheduler = Traits::Scheduler;
    using Stage = Traits::Stage;
    using Queue = Traits::Queue;

    TEST(exampleOfPuttingRawPointersIntoQueues)
    {
        Dispatcher d;

        Queue q;
        ProcessingFunctorWithDispatcherAndNextStage<Dispatcher> pf(d, Stages::Stage2);
        Stage s1(Stages::Stage1, d, q, pf);

        Queue q2;
        ProcessingFunctorWithDispatcherAndNextStage<Dispatcher> pf2(d, Stages::Stage3);
        Stage s2(Stages::Stage2, d, q2, pf2);

        Queue q3;
        GarbageCollector garbageCollection;
        Stage s3(Stages::Stage3, d, q3, garbageCollection);

        Message::smartptr m = new TestMessage();    // message allocated...
        CHECK(m);

        d.dispatch(Stages::Stage1, *m);             // inserted into s1 queue
        s1.process();                               // moved to s2 queue
        s2.process();                               // moved to s3 queue
        s3.process();                               // garbage collected
    }
}