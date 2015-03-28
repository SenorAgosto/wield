# Color- Scheduling Policy 

This scheduling policy implements the Color- scheduling algorithm. Color- is a simulation of Color _minus_ the central work queue. 

In Color-, the work queue is replaced with event counters at each stage. The event counters are used to estimate observed queue depths at specific points in time. 

The simulated queue in Color- is therefore a priority queue. In the event two stages have the same estimated qeueue depth, this implementation prefers the first stage in the list - so SRPT can be used to tie break, by ordering the stages from egress stages to ingress stages in the stage enum. 

We provide the scheduling policy and dispatcher in ColorMinus.hpp and Dispatcher.hpp respectively, under `wield/schedulers/color_minus/`
