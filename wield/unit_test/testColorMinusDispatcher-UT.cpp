#include "./platform/UnitTestSupport.hpp"

#include <wield/schedulers/color_minus/Dispatcher.hpp>
#include <wield/schedulers/utils/MessageCount.hpp>

#include "./test_color_minus/Message.hpp"
#include "./test_color_minus/ProcessingFunctor.hpp"
#include "./test_color_minus/Traits.hpp"

namespace {

    TEST(verifyColorDispatcherInstantiation)
    {
        using namespace wield::schedulers::color_minus;

        using Dispatcher = typename test_color_minus::Traits::Dispatcher;
        using Stages = typename test_color_minus::Traits::StageEnumType;
        using Stage = typename test_color_minus::Traits::Stage;
        using MessageCount = wield::schedulers::utils::MessageCount<Stages>;

        MessageCount stats;
        Dispatcher dispatcher(stats);
    }

    TEST(verifyAddsStageNameWhenMessageDispatched)
    {
        using namespace wield::schedulers::color_minus;

        using Dispatcher = typename test_color_minus::Traits::Dispatcher;
        using Message = typename test_color_minus::Traits::Message;
        using Stages = typename test_color_minus::Traits::StageEnumType;
        using Stage = typename test_color_minus::Traits::Stage;
        using Queue = typename test_color_minus::Traits::Queue;
        using MessageCount = wield::schedulers::utils::MessageCount<Stages>;

        MessageCount stats;
        Dispatcher dispatcher(stats);
        Queue q;
        test_color_minus::ProcessingFunctor pf;

        Stage stage1(Stages::Stage1, dispatcher, q, pf);
        Stage stage2(Stages::Stage2, dispatcher, q, pf);
        Stage stage3(Stages::Stage3, dispatcher, q, pf);

        Message::smartptr m = new test_color_minus::TestMessage();

        dispatcher.dispatch(Stages::Stage1, *m);
        dispatcher.dispatch(Stages::Stage1, *m);
        dispatcher.dispatch(Stages::Stage3, *m);
        dispatcher.dispatch(Stages::Stage2, *m);
        dispatcher.dispatch(Stages::Stage1, *m);

        CHECK_EQUAL(3, stats.estimatedDepth(Stages::Stage1));
        CHECK_EQUAL(1, stats.estimatedDepth(Stages::Stage2));
        CHECK_EQUAL(1, stats.estimatedDepth(Stages::Stage3));
    }
}