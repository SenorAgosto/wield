# Color Scheduling Policy 

This scheduling policy implements the Color scheduling algorithm, described in ??. It requires a dispatcher to enqueue the stage name any time an event is dispatched to a stage. 

This implementation currently uses a non-blocking queue for the work queue, there is a TODO to implement a back-off policy.

We provide the scheduling policy and dispatcher in Color.hpp and Dispatcher.hpp respectively, under the `wield/schedulers/color` folder.
