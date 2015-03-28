#include "./platform/UnitTestSupport.hpp"

#include "./test_color/ProcessingFunctor.hpp"
#include "./test_color/Message.hpp"
#include "./test_color/Traits.hpp"

#include <wield/schedulers/color/Color.hpp>
#include <wield/schedulers/color/Dispatcher.hpp>

#include <array>

namespace {

    TEST(verifyColorSchedulingPolicyCanBeInstantiated)
    {
        using namespace wield::schedulers::color;

        using Dispatcher = typename test_color::Traits::Dispatcher;
        using Stages = typename test_color::Traits::StageEnumType;
        using Stage = typename test_color::Traits::Stage;
        using ColorQueue = Concurrency::concurrent_queue<Stages>;
        using SchedulingPolicy = Color<Stages, Dispatcher, Stage, ColorQueue>;
        using MaxConcurrencyContainer = typename SchedulingPolicy::MaxConcurrencyContainer;

        ColorQueue q;
        Dispatcher d(q);

        MaxConcurrencyContainer concurrency = {{1}};
        SchedulingPolicy color(d, q, concurrency);
    }

    TEST(verifyColorAssignsFirstNextStageRequestToStage1)
    {
        using namespace wield::schedulers::color;

        using Dispatcher = typename test_color::Traits::Dispatcher;
        using Message = typename test_color::Traits::Message;
        using Stages = typename test_color::Traits::StageEnumType;
        using Stage = typename test_color::Traits::Stage;
        using ColorQueue = Concurrency::concurrent_queue<Stages>;
        using SchedulingPolicy = Color<Stages, Dispatcher, Stage, ColorQueue>;
        using MaxConcurrencyContainer = typename SchedulingPolicy::MaxConcurrencyContainer;
        using Queue = typename test_color::Traits::Queue;

        ColorQueue q;
        Dispatcher d(q);

        test_color::ProcessingFunctor pf;
        Queue stageQueue;
        Stage stage1(Stages::Stage1, d, stageQueue, pf);
        Stage stage2(Stages::Stage2, d, stageQueue, pf);
        Stage stage3(Stages::Stage3, d, stageQueue, pf);

        MaxConcurrencyContainer concurrency = {{1, 1, 1}};
        SchedulingPolicy color(d, q, concurrency);

        // needed to ensure the UT stops, otherwise its continues polling until
        // we have work.
        Message::smartptr m = new test_color::TestMessage();
        d.dispatch(Stages::Stage1, *m);
        d.dispatch(Stages::Stage2, *m);
        d.dispatch(Stages::Stage3, *m);

        CHECK_EQUAL(&stage1, & color.nextStage(0));
        CHECK_EQUAL(&stage2, & color.nextStage(1));
        CHECK_EQUAL(&stage3, & color.nextStage(2));
    }
}