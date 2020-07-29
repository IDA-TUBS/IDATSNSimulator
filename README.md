IDA TSN Simulator is an open source library for for simulation of TSN-based automotive networks. The library is designed to run together with the OMNeT++ simulation engine. It is developed by the Institute of Computer and Network Engineering (IDA) of TU Braunschweig.

The provided library for simulation of TSN-based automotive networks consists of the following main elements:
- End Nodes, for example is an ECU (processing Node) connected to the Ethernet Switch
- Switches for simulatioing an Ethernet network
other componets are build as combination of theses ones.

IDA TSN Simulator supports:
- The Simulation of RTPS protocol for the DDS transmissions.
- Parametrization of the end nodes, for instance:
	- traffic shaping per frame
	- activation frequency per frame or burst
	- ToS priorities
	- packe sizes
	- etc.
- Parametrization of the switches:
	- buffer sizes
	- different arbiters
		- IEEE 802.3 standard Ethernet
		- IEEE 802.1Q introducing strict prioritization of individiual traffic classes
		- IEEE 802.1Qav introducing credit-based shaper from AVB standard
		- IEEE 802.1Qbv introducing time-aware shaper from TSN standard
- A collection of statistics for the end-to-end communication via RTPS, i.e. sampling. 
- A collection of statistics for the end-to-end communication per frame and stream. In this context, streams are defined as frames that belongs to the same sender/application.
- A collection for individiual network componets
	- Number of dropped frames
	- Buffer backlog
	- Link utilization

