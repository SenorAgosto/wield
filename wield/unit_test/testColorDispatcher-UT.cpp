#include "./platform/UnitTestSupport.hpp"

#include "./test_color/ProcessingFunctor.hpp"
#include "./test_color/Message.hpp"
#include "./test_color/Traits.hpp"

#include <wield/schedulers/color/Color.hpp>
#include <wield/schedulers/color/Dispatcher.hpp>

#include <array>

namespace {

    TEST(verifyColorDispatcherInstantiation)
    {
        using namespace wield::schedulers::color;

        using Stages = typename test_color::Traits::StageEnumType;
        using Stage = typename test_color::Traits::Stage;
        using Queue = Concurrency::concurrent_queue<Stages>;

        Queue q;
        Dispatcher<Stages, Stage, Queue> dispatcher(q);
    }

    TEST(verifyAddsStageNameWhenMessageDispatched)
    {
        using namespace wield::schedulers::color;

        using Message = typename test_color::Traits::Message;
        using Stages = typename test_color::Traits::StageEnumType;
        using Stage = typename test_color::Traits::Stage;
        using ColorQueue = Concurrency::concurrent_queue<Stages>;
        using Queue = typename test_color::Traits::Queue;

        Message::smartptr m = new test_color::TestMessage();
        ColorQueue colorQueue;
        Dispatcher<Stages, Stage, ColorQueue> dispatcher(colorQueue);

        Queue q;
        test_color::ProcessingFunctor pf;

        Stage stage1(Stages::Stage1, dispatcher, q, pf);
        Stage stage2(Stages::Stage2, dispatcher, q, pf);
        Stage stage3(Stages::Stage3, dispatcher, q, pf);

        dispatcher.dispatch(Stages::Stage1, *m);
        dispatcher.dispatch(Stages::Stage1, *m);
        dispatcher.dispatch(Stages::Stage3, *m);
        dispatcher.dispatch(Stages::Stage2, *m);
        dispatcher.dispatch(Stages::Stage1, *m);

        CHECK_EQUAL(5, q.unsafe_size());

        std::array<Stages, 5> stages = {{Stages::NumberOfEntries}};
        for(auto& s : stages)
        {
            colorQueue.try_pop(s);
        }

        CHECK_EQUAL(Stages::Stage1, stages[0]);
        CHECK_EQUAL(Stages::Stage1, stages[1]);
        CHECK_EQUAL(Stages::Stage3, stages[2]);
        CHECK_EQUAL(Stages::Stage2, stages[3]);
        CHECK_EQUAL(Stages::Stage1, stages[4]);
    }
}