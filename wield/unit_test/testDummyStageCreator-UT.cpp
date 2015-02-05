#include "./platform/UnitTestSupport.hpp"
#include "./platform/ConcurrentQueue.hpp"

#include <wield/details/DummyStageCreator.hpp>
#include <wield/details/QueueCreator.hpp>
#include <vector>

#include "./test/Traits.hpp"
#include "./test/Message.hpp"
#include "./test/ProcessingFunctor.hpp"

namespace {

    using namespace test;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Dispatcher = Traits::Dispatcher;

    TEST(verifyDummyStageCreatorCreatesTheCorrectNumberOfStages)
    {
        Dispatcher dispatcher;
        ProcessingFunctor dummyFunctor;

        std::deque<Queue> queues;
        wield::details::QueueCreator<Queue, 2> queueCreator(queues);

        std::deque<Stage> stages;
        CHECK_EQUAL(0, stages.size());

        wield::details::DummyStageCreator<Traits, Stages, Stages::Stage1, Stages::Stage3> stageCreator(dispatcher, dummyFunctor, queues, stages);
        CHECK_EQUAL(2, stages.size());
    }
}
