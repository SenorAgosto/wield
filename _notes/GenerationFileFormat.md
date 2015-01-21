# Wield Generator File Format
## Austin Gilbert, Version 0.1

This document defines the format of a wield_gen file, the file used to scaffold a wield application. 

    namespace('Organization', 'Application')

Defines the c++ namespace to wrap all generated code into. This would generate the following:

    namespace Organization { namespace Application {
        // stuff ...
    }}
    
To define stages and processing functors with the same name use the _stage_ clause.

    stage('Stage1')

To generate a processing functor with a given name, and possibly reuse the processing functor in multiple stages, use the following stage clause:

    stage('Stage', 'ProcessingFunctor')
    stage('Stage2', 'ProcessingFunctor')

Here two stages would be created, both take separate instances of ProcessingFunctor processing functor.

To define message types, use the _message_ clause. 

    message('Message1')

This generates a file and class called Message1 which inherits from the base message type _Traits::Message_.

To define a message which inherits from a message which is not the base message type, pass a second argument with the base class name. 

    message('Message2', 'Message1')

Here Message2 would inherit from Message1 rather than Traits::Message. 

To define the stage graph, use the _edge_ clause. The following implies there are two stages and Stage1 needs a reference to the dispatcher. It further implies that all message types are dispatched to Stage2.

    edge('Stage1', 'Stage2')

If we know different message types are to go to different stages for processing, we can add the message name to express that in the stage graph. 

    edge('Stage1', 'Stage2', 'Message1')
    edge('Stage1', 'Stage3', 'Message2')

To force a stage's logic to be executed inline instead of pushing the message onto a queue, use the _inline_ clause. Note: this forces us to use polymorphic::QueueAdapter as the queue type which introduces a virtual call before every enqueue call. 
    
    inline('Stage2')

Setting a stage's affinity to a NUMA node or specific CPU core is accomplished through the _affinity_ clause. 

    affinity('Stage1', 'NUMA_0')
    affinity('Stage1', 'CPU_ID')

Attempting to set the affinity of an inlined stage will generate a warning. The actual affinity will match that of the calling stage.

    % WARNING: inlined stages can't have affinity.
    inline('Stage2')
    affinity('Stage2', 'CPU_ID')

You will most likely want to keep all the _affinity_ and _inline_ clauses in separate files than _stage_, _message_, _edge_, and _namespace_. It is imagined we could supply different affinity profiles for different hardware, etc. 