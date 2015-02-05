#pragma once 
#include <cstddef>
#include <deque>

namespace wield { namespace details {

    // A helper class that creates the number of queues we need, adding
    // them to a vector<QueueType>.
    //
    // @QueueType is the concrete queue type to create.
    template<class QueueType, std::size_t NumberOfQueues>
    class QueueCreator : public QueueCreator<QueueType, NumberOfQueues - 1>
    {
    public:
        using base = QueueCreator<QueueType, NumberOfQueues - 1>;

        QueueCreator(std::deque<QueueType>& queues)
            : base(queues)
        {
            queues.emplace_back();
        }
    };

    // A specialization to stop recursion.
    template<class QueueType>
    class QueueCreator<QueueType, 0>
    {
    public:
        QueueCreator(std::deque<QueueType>& queues)
        {
        }
    };

}}
