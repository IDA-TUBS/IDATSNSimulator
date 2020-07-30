![] (./logo/IDA.png)
IDA TSN Simulator is an open source library for the simulation of a TSN-based automotive network. The library is designed to run within the OMNeT++ simulation engine.  The IDA TSN Simulator is developed by the Institute of Computer and Network Engineering (IDA) of TU Braunschweig.

The provided TSN simulation library consists of the following main elements:
- Endnodes, for example is an ECU (processing Node) connected to the Ethernet Switch which produce and receive data streams.
- Switches which are interconnected to build up an Ethernet network other components are build as combination of theses ones.

Features
-------
IDA TSN Simulator supports:
- Parametrization of the Endnodes, for instance:
	- traffic shaping per frame
	- activation frequency per frame or burst
	- IEEE 802.1Q conform priorities
	- package sizes
	- etc.
- Parametrization of the switches:
	- buffer sizes
	- different arbiters
		- IEEE 802.3 Standard Ethernet
		- IEEE 802.1Q introducing strict prioritization of individual traffic classes
		- IEEE 802.1Qav introducing credit-based shaper from AVB standard
		- IEEE 802.1Qbv introducing time-aware shaper from TSN standard
- The Simulation of RTPS protocol for the DDS transmissions.
- A collection of statistics for the end-to-end communication via RTPS, i.e. sampling. 
- A collection of statistics for the end-to-end communication per frame and stream. In this context, streams are defined as frames that belongs to the same sender/application.
- A collection for individual network components (Endnodes, Switches)
	- Number of dropped frames
	- Buffer backlog
	- Link utilization

The authors
-----------
Dr.-Ing. Adam Kostrzewa (kostrzewa@ida.ing.tu-bs.de)
Dominik Stöhrmann, M.Sc. (stoehrmann@ida.ing.tu-bs.de)
Anika Christmann, M.Sc. (christmann@ida.ing.tu-bs.de)
Jonas Peeck, M.Sc. (peeck@ida.ing.tu-bs.de