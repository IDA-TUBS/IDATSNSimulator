# RTPS Configuration
The following parameters have to be configured
* For a multicast transmission reader entityIds should have the same Id even if they are in different participants!
* * Therefore search an entityId, which is not used in each of the receiving participants

## Overall configuration
* **.middleware.delayUnit[*].duration = normal(xms, yms)     (Distribution of the initial delay of a generated Sample before it is handled  in the RTPS middleware)
* **.middleware.delayUnit[*].durationMin = xms     (The minimum initial delay of a generated Sample before it is handled  in the RTPS middleware)
* **.middleware.delayUnit[*].durationMax = xms     (The maximum initial delay of a generated Sample before it is handled  in the RTPS middleware)
* **.middleware.shaperEnabled = true            (Shaping over all outgoing messages of an RTPS middleware component) 
* **.middleware.shapingDuration = 10us          (Shaping duration over all outgoing messages of an RTPS middleware component)
* **.middleware.writer[*].history = 10          (The history size of a writer entity)
* **.middleware.reader[*].history = 10           (The history size of a reader entity)

## Endpoint specific
* **.Endpoint.middleware.participantId = x (globally unique participant Id)
* **.Endpoint.numMwStreams = x (The number of middleware writers)
* **.Endpoint.numMwSinks = x (The number of middleware readers)

## Stream specific
* **.Endpoint.sampleGenerator[<>].sampleSize = normal(xB,yB) (The size distribution of a sample)
* **.Endpoint.sampleGenerator[<>].sampleSizeMin = xB (The minimum size of a sample)
* **.Endpoint.sampleGenerator[<>].sampleSizeMax = xB (The maximum size of a sample)
* **.Endpoint.sampleGenerator[<>].period = normal(xs,ys) (The period distribution of a sample i.e. the distance between two consecutive sample generations)
* **.Endpoint.sampleGenerator[<>].periodMin = xs (The minimum period)
* **.Endpoint.sampleGenerator[<>].periodMax = xs (The maximum period)
* **.Endpoint.sampleGenerator[<>].globalStreamId = <> (The unique global stream Id)

## Wrtier specific
* **.Endpoint.middleware.writer[<>].globalStreamId = <> (The unique global stream ID of the writer corresponding to the connected generator)
* **.Endpoint.middleware.writer[<>].streamEthernetPriority = <> (The Ethernet priority )
* **.Endpoint.middleware.writer[<>].sourceMac = <> (The own egress port to send the messages out)
* **.Endpoint.middleware.writer[<>].entityId = <>  (The own entity Id which has to be unique for the participant)
* **.Endpoint.middleware.writer[<>].destinationMac = <> (The remote (ingress) port MAC adress to send the messages to)
* **.Endpoint.middleware.writer[<>].destinationEntityId = <> (The remote entity Id -- Note that for multicast all readers have to be configured with this adress)
* **.Endpoint.middleware.writer[<>].fragmentSize = <> (The fragment size)
* **.Endpoint.middleware.writer[<>].shaper.shapingDuration = <> (The frame shaping duration of the writer for its sample transmission)

## Reader specific
* **.Endpoint.middleware.reader[<>].entityId = X  (The readers entityId which has to be unique for the RTPS participant)
* **.Endpoint.middleware.reader[<>].sourceMac = X (The own egress prot to send the messages out -- backchannel for retranmissions)

# Example

