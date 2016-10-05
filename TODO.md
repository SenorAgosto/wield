# Wield TODO

- implement a compile-time FanoutProcessingFunctor. 

- scheduling policies are templated on Stage and StageEnum and Dispatcher, both the Stage and StageEnum are available from the Dispatcher. Eliminate the need for Stage & StageEnum in the scheduling policy. 

- add IdlePolicy to schedulers which gets invoked in the event there is no stages available to visit that have work. 

- affinity & priority 

    - add a thread affinity policy to SchedulerBase, have it default to a null implementation.
    - functionality for setting process and thread priority
    - functionality to set NUMA affinity (http://msdn.microsoft.com/en-us/library/windows/desktop/aa363804(v=vs.85).aspx)

- implement an example of using a poison pill to shutdown an application. This is for dataflow applications that are done after consuming all inputs.

- implement schedulers:

    - Cohort 
    - DBR (Drum Buffer Rope)
    - MG1? 

- change interface of queue so push returns a bool? This would allow ring buffers to be used as the queue implementation if desired. 

- improve robustness of processing threads in event of exceptions? Is there a UT that tries to throw and verify a log message (I think that might not be returning), otherwise just add a do {} while(!done); around what's already there.

- benchmark branches with 'push optimization' and 'cache-line padded reference count' to see what performance benefits of each is. Also benchmark the combination of the two.

- add 'noexcept' to everything it is applicable to. 

- (not sure about this) change QueueBase template arguments to take queue_type as a template template parameter

Performance:

- In SchedulerBase, compare whether using done_.load(std::memory_order_acquire)/done_.store(true, std::memory_order_release) is better or whether just doing done_ = true; and reading done_ is better.
- Test different compiler options
- Test whether the changes in OptimizePush branch are worth it. Using move semantics into queues to avoid cache-line ping-pong caused by incrementing/decrementing messages as they go into queues. 
- cache-line pad RandomVisit scheduling policy.

## Discussion of Scheduling Policies
### Dynamic MG1

In the original treatment in the literature, MG1 used a preallocated polling table to determine what order to visit stages in. Here, we implement MG1 without the polling table. 

In the MG1 policy, stages are visited proportional to their load, with the highest loaded stages being visited the most frequently. Load is calculated as:

    double load = arrivalRate / serviceRate 

We can eliminate the need to calculate rates relative to time through algerbraic manipulation:

     load = (numMsg / period) / (numServiced / period)
          = (numMsg / period) * (period / numServiced)
          =  numMsg / numServiced

Two arrays are kept in the scheduling policy, one tracks the number of messages dispatched to a stage (numMsg) and the other tracks the number of messages processed by a stage (numServiced). For performance reasons, these values are only ever incremented, and as they are typically going to track fairly close to each other, we don't worry about rollover (1M msg/second it will take 500 years on a uint64_t type)

### DBR Scheduling 
DBR policy prefers stages in ascending order of their service rates so slower stages are visited frequently. We can achieve this policy by calculating the service rate in a cheap-time call and accumulating the service times. The next stage to visit is always the stage with the highest accumulated service time. DBR restarts polling at the slowest stage after any stage has been successfully visited. 
