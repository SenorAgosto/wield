#include "./platform/UnitTestSupport.hpp"

#include "./test_color_minus/ProcessingFunctor.hpp"
#include "./test_color_minus/Message.hpp"
#include "./test_color_minus/Traits.hpp"

#include <wield/schedulers/color_minus/ColorMinus.hpp>
#include <wield/schedulers/color_minus/Dispatcher.hpp>
#include <wield/schedulers/utils/MessageCount.hpp>

namespace {

    TEST(verifyColorMinusSchedulingPolicyCanBeInstantiated)
    {
        using namespace wield::schedulers::color_minus;

        using Dispatcher = typename test_color_minus::Traits::Dispatcher;
        using Stages = typename test_color_minus::Traits::StageEnumType;
        using Stage = typename test_color_minus::Traits::Stage;
        using SchedulingPolicy = ColorMinus<Stages, Dispatcher, Stage>;
        using MaxConcurrencyContainer = typename SchedulingPolicy::MaxConcurrencyContainer;
        using MessageCount = wield::schedulers::utils::MessageCount<Stages>;

        MessageCount stats;
        Dispatcher d(stats);

        MaxConcurrencyContainer concurrency = {{1}};
        SchedulingPolicy color(d, stats, concurrency);
    }

    TEST(verifyColorMinusAssignsFirstNextStageRequestToStage1)
    {
        using namespace wield::schedulers::color_minus;

        using Dispatcher = typename test_color_minus::Traits::Dispatcher;
        using Message = typename test_color_minus::Traits::Message;
        using Stages = typename test_color_minus::Traits::StageEnumType;
        using Stage = typename test_color_minus::Traits::Stage;
        using SchedulingPolicy = ColorMinus<Stages, Dispatcher, Stage>;
        using MaxConcurrencyContainer = typename SchedulingPolicy::MaxConcurrencyContainer;
        using Queue = typename test_color_minus::Traits::Queue;
        using MessageCount = wield::schedulers::utils::MessageCount<Stages>;

        MessageCount stats;
        Dispatcher d(stats);

        test_color_minus::ProcessingFunctor pf;
        Queue stageQueue;
        Stage stage1(Stages::Stage1, d, stageQueue, pf);
        Stage stage2(Stages::Stage2, d, stageQueue, pf);
        Stage stage3(Stages::Stage3, d, stageQueue, pf);

        MaxConcurrencyContainer concurrency = {{1, 1, 1}};
        SchedulingPolicy color(d, stats, concurrency);

        // needed to ensure the UT stops, otherwise its continues polling until
        // we have work.
        Message::smartptr m = new test_color_minus::TestMessage();
        d.dispatch(Stages::Stage1, *m);
        d.dispatch(Stages::Stage2, *m);
        d.dispatch(Stages::Stage3, *m);

        CHECK_EQUAL(&stage1, & color.nextStage(0));
        CHECK_EQUAL(&stage2, & color.nextStage(1));
        CHECK_EQUAL(&stage3, & color.nextStage(2));

        // cleanup the memory in the queues...
        stage1.process();
        stage2.process();
        stage3.process();
    }

    TEST(verifyColorMinusAssignsStageWithMostWork)
    {
        using namespace wield::schedulers::color_minus;

        using Dispatcher = typename test_color_minus::Traits::Dispatcher;
        using Message = typename test_color_minus::Traits::Message;
        using Stages = typename test_color_minus::Traits::StageEnumType;
        using Stage = typename test_color_minus::Traits::Stage;
        using SchedulingPolicy = ColorMinus<Stages, Dispatcher, Stage>;
        using MaxConcurrencyContainer = typename SchedulingPolicy::MaxConcurrencyContainer;
        using Queue = typename test_color_minus::Traits::Queue;
        using MessageCount = wield::schedulers::utils::MessageCount<Stages>;

        MessageCount stats;
        Dispatcher d(stats);

        test_color_minus::ProcessingFunctor pf;
        Queue stageQueue;
        Stage stage1(Stages::Stage1, d, stageQueue, pf);
        Stage stage2(Stages::Stage2, d, stageQueue, pf);
        Stage stage3(Stages::Stage3, d, stageQueue, pf);

        MaxConcurrencyContainer concurrency = {{1, 1, 1}};
        SchedulingPolicy color(d, stats, concurrency);

        // needed to ensure the UT stops, otherwise its continues polling until
        // we have work.
        Message::smartptr m = new test_color_minus::TestMessage();
        d.dispatch(Stages::Stage3, *m);
        d.dispatch(Stages::Stage3, *m);
        d.dispatch(Stages::Stage3, *m);

        CHECK_EQUAL(&stage3, & color.nextStage(0));
        CHECK_EQUAL(&stage1, & color.nextStage(1));

        // cleanup the memory in the queues...
        stage3.process();
        stage3.process();
        stage3.process();
    }
}

