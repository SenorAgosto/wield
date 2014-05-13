

## SEDA scheduling ideas

### Inference

Use the dispatcher to track dispatch events and then infer what the most likely state of the system is (where the work is at). Use this information to apportion the threads appropriately. 

[Belief Propagation](http://en.wikipedia.org/wiki/Belief_propagation)

### Reservation Tables

[Latancy Solution Pipeline Reservation Table](http://www.scribd.com/doc/120016881/Latancy-Solution-pipeline-reservation-table)

Investigate the possibility we can use stochastic information (Markov model) about advancement through the pipeline (and cycles if they exist) to predict what the optimum way to visit the stages is. 

Use this for "low latency" scheduler and blending latency and throughput schedulers.