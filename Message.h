#pragma once
#include <atomic>
#include <boost/intrusive_ptr.hpp>

namespace wield {
	class ProcessingFunctor;
}

namespace wield {

	class Message
	{
	public:
        typedef boost::intrusive_ptr<Message> smartptr;
        Message();
        virtual ~Message();

		virtual void ProcessWith(ProcessingFunctor& process);

    private:
        std::atomic<std::size_t> reference_count_;

        friend void intrusive_ptr_add_ref(Message* m);
        friend void intrusive_ptr_release(Message* m);
	};
}