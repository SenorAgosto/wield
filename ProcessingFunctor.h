#pragma once

namespace wield {
	class Message;
}

namespace wield {

	class ProcessingFunctor
	{
	public:
		ProcessingFunctor();
		virtual ~ProcessingFunctor();
	
		virtual void operator()(Message& msg) = 0;
	};
}