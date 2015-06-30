#include <atomic>
#include <chrono>
#include <thread>

#include <queue_stress/Traits.hpp>

#include <queue_stress/stage/ForwardingProcessingFunctor.hpp>
#include <queue_stress/stage/StatsProcessingFunctor.hpp>

void createMessage(queue_stress::Traits::Dispatcher& dispatcher, const std::size_t sequenceNumber);
void tryCreateMessage(queue_stress::Traits::Dispatcher& dispatcher, const std::size_t sequenceNumber);

int main() 
{
    using namespace queue_stress;

    using Dispatcher = Traits::Dispatcher;
    using Message = Traits::Message;
    using Scheduler = Traits::Scheduler;
    using Stage = Traits::Stage;
    using Queue = Traits::Queue;

    Dispatcher dispatcher;

    Queue q1;
    stage::ForwardingProcessingFunctor f1(dispatcher, Stages::Stage2);
    Stage s1(Stages::Stage1, dispatcher, q1, f1);

    Queue q2;
    stage::ForwardingProcessingFunctor f2(dispatcher, Stages::Stage3);
    Stage s2(Stages::Stage2, dispatcher, q2, f2);

    Queue q3;
    stage::ForwardingProcessingFunctor f3(dispatcher, Stages::Stage4);
    Stage s3(Stages::Stage3, dispatcher, q3, f3);

    Queue q4;
    stage::StatsProcessingFunctor stats;
    Stage s4(Stages::Stage4, dispatcher, q4, stats);

    Scheduler scheduler(dispatcher);

    std::atomic<bool> done(false);
    std::size_t sequenceNumber = 0;

    scheduler.start();

    while(!done)
    {
        tryCreateMessage(dispatcher, sequenceNumber++);
    }

    scheduler.join();
    return 0;
}

void createMessage(queue_stress::Traits::Dispatcher& dispatcher, const std::size_t sequenceNumber)
{
    using namespace queue_stress;

    message::TestMessage::smartptr message(new message::TestMessage(sequenceNumber));
    dispatcher.dispatch(Stages::Stage1, *message);
}

void tryCreateMessage(queue_stress::Traits::Dispatcher& dispatcher, const std::size_t sequenceNumber)
{
    try 
    {
        createMessage(dispatcher, sequenceNumber);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
