#include "./platform/UnitTestSupport.hpp"
#include "./platform/ConcurrentQueue.hpp"

#include "./test_adapter/Traits.hpp"
#include "./test_adapter/Message.hpp"
#include "./test_adapter/ProcessingFunctor.hpp"
#include "./test_adapter/Stages.hpp"

#include <wield/adapters/polymorphic/MultipleInputQueueAdapter.hpp>
#include <cstddef>

namespace {

    using namespace test_adapter;
    using Dispatcher = typename Traits::Dispatcher;
    using MessagePtr = typename Traits::MessagePtr;
    using Stage = typename Traits::Stage;

    TEST(verifyInstantiationOfMultipleInputQueueAdapter)
    {
        Dispatcher d;
        ProcessingFunctor dummyFunctor;

        wield::adapters::polymorphic::MultipleInputQueueAdapter<
              Traits
            , Concurrency::concurrent_queue<MessagePtr>
            , Stages
            , Stages::Stage1
            , Stages::Stage3>
        q(d, dummyFunctor);
    }

    TEST(verifyMultipleInputsQueueAdapter)
    {
        Dispatcher d;
        ProcessingFunctor dummyFunctor;

        // queues for Stage1 & Stage2 are handled by Stage3.
        wield::adapters::polymorphic::MultipleInputQueueAdapter<
              Traits
            , Concurrency::concurrent_queue<MessagePtr>
            , Stages
            , Stages::Stage1
            , Stages::Stage3>
        q(d, dummyFunctor);
        CHECK_EQUAL(0, q.unsafe_size());

        ProcessingFunctor f;
        Stage stage(Stages::Stage3, d, q, f);

        // create some messages and dispatch them to Stage1 & Stage2.
        Message::smartptr m = new TestMessage();
        d.dispatch(Stages::Stage1, *m);
        d.dispatch(Stages::Stage2, *m);

        CHECK_EQUAL(2, q.unsafe_size());

        stage.process();    // process the first message from Stage1 queue
        stage.process();    // process the second message from Stage2 queue
        stage.process();    // this shouldn't do anything...

        CHECK_EQUAL(2, f.message1CallCount_);
    }

    TEST(verifyAllQueuesArePolled)
    {
        Dispatcher d;
        ProcessingFunctor dummyFunctor;

        // queues for Stage1, Stage2, & Stage3 are handled by Stage4
        wield::adapters::polymorphic::MultipleInputQueueAdapter<
              Traits
            , Concurrency::concurrent_queue<MessagePtr>
            , Stages
            , Stages::Stage1
            , Stages::Stage4>
        q(d, dummyFunctor);

        ProcessingFunctor f;
        Stage stage(Stages::Stage4, d, q, f);

        // put one message in the 3rd queue.
        Message::smartptr m = new TestMessage();
        d.dispatch(Stages::Stage3, *m);

        stage.process();
        CHECK_EQUAL(1, f.message1CallCount_);
    }
}