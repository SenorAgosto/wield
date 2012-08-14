#pragma once

#include <atomic>
#include <boost/intrusive_ptr.hpp>

namespace wield {

    template<class ProcessingFunctor>
	class MessageBase
	{
	public:
        typedef boost::intrusive_ptr<MessageBase> smartptr;
        virtual ~MessageBase(){}

		virtual void ProcessWith(ProcessingFunctor& process) = 0;

    protected:
        MessageBase()
            : reference_count_(0)
        {
        }

    private:
        std::atomic<size_t> reference_count_;

        friend void intrusive_ptr_add_ref(MessageBase<ProcessingFunctor>* m)
        {
            ++m->reference_count_;
        }

        friend void intrusive_ptr_release(MessageBase<ProcessingFunctor>* m)
        {
            if( 0 == --m->reference_count_)
            {
                delete m;
            }
        }
    };
}