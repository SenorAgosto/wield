#pragma once
#include <wield/MessageBase.h>

namespace wield {

    template<class ProcessingFunctor, class QueueType>
    class QueueBase
    {
    public:
        typedef typename QueueType::size_type size_type;

        QueueBase()
        {
        }

        void push(const typename MessageBase<ProcessingFunctor>::smartptr& value)
        {
            queue_.push(value);
        }

        bool try_pop(typename MessageBase<ProcessingFunctor>::smartptr& value)
        {
            if(!queue_.empty())
            {
                value = queue_.front();
                queue_.pop();

                return true;
            }

            return false;
        }
        
        size_type unsafe_size(void) const
        {
            return queue_.size();
        }

    private:
        //queue_type<typename MessageBase<ProcessingFunctor>::smartptr, std::allocator<typename MessageBase<ProcessingFunctor> > > queue_;
        QueueType queue_;
    };
}
