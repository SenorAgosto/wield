# Things To Try

## Move Semantics

Optimizing movement of reference count smart pointers through the stage graph. Currently, there is a problem because we will be inducing cache-line ping pong incrementing/decrementing the reference count on different cores. 

I've tried a couple of different things. One thing I haven't tried yet. Switch the dispatcher to handle message* types instead of message&. This might help attempt #2 get off the ground.  

## SEDA scheduling ideas

### Inference

Use the dispatcher to track dispatch events and then infer what the most likely state of the system is (where the work is at). Use this information to apportion the threads appropriately. 

[Belief Propagation](http://en.wikipedia.org/wiki/Belief_propagation)

### Reservation Tables

[Latancy Solution Pipeline Reservation Table](http://www.scribd.com/doc/120016881/Latancy-Solution-pipeline-reservation-table)

Investigate the possibility we can use stochastic information (Markov model) about advancement through the pipeline (and cycles if they exist) to predict what the optimum way to visit the stages is. 

Use this for "low latency" scheduler and blending latency and throughput schedulers.
