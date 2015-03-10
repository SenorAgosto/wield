# Wield TODO

- benchmark branches with 'push optimization' and 'cache-line padded reference count' to see what performance benefits of each is. Also benchmark the combination of the two.

- add 'noexcept' to everything it is applicable to. 
- input stages. Would it be possible to redefine bool process() ? 
  rather than pumping the queue have it generate events using a callback/functor.
  probably want to move guts of process() into process_impl() and then either use 
  enable_if to create/suppress implementation based on stage type (normal vs. input).
  Another possible approach is to forward process() to process_impl() and implement
  process_impl() in a derived class. 
		CommonStageBase <- EventSourceStage 
		CommonStageBase <- ProcessingStageBase
  
  An EventSourceStage may not want a queue or processing functor. As they wouldn't be necessary
  In that case, dispaching messages to the stage has no effect. Another scenario, we may want
  to dispatch messages to an "input" stage. Think through the best way to support both scenarios.
  

- functionality for setting processor affinity
- functionality for setting process and thread priority
- functionality to set NUMA affinity (http://msdn.microsoft.com/en-us/library/windows/desktop/aa363804(v=vs.85).aspx)
- (not sure about this) change QueueBase template arguments to take queue_type as a template template parameter

Performance:
- In SchedulerBase, compare whether using done_.load(std::memory_order_acquire)/done_.store(true, std::memory_order_release) is better or whether just doing done_ = true; and reading done_ is better.
- Test different compiler options
- Test whether the changes in OptimizePush branch are worth it. Using move semantics into queues to avoid cache-line ping-pong caused by incrementing/decrementing messages as they go into queues. 

