#pragma once
#include <wield/MessageBase.h>

namespace wield {

    template<class ProcessingFunctor, class QueueType>
    class QueueBase final
    {
    public:
        typedef typename QueueType::size_type size_type;

        QueueBase()
        {
        }

        ~QueueBase()
        {
        }

        inline void push(const typename MessageBase<ProcessingFunctor>::smartptr& value)
        {
            queue_.push(value);
        }

        inline bool try_pop(typename MessageBase<ProcessingFunctor>::smartptr& value)
        {
            return queue_.try_pop(value);
        }
        
        inline size_type unsafe_size(void) const
        {
            return queue_.unsafe_size();
        }

    private:
        QueueType queue_;
    };
}
