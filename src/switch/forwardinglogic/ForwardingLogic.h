//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef SRC_SWITCH_FORWARDINGLOGIC_FORWARDINGLOGIC_H_
#define SRC_SWITCH_FORWARDINGLOGIC_FORWARDINGLOGIC_H_

#include <stdio.h>
#include <fstream>
#include <string.h>
#include <omnetpp.h>
#include <map>
#include <vector>

#include "../../common/ConfigurationReader.h"
#include "../../common/GlobalMacros.h"
#include "../../messagetype/EthernetFrame_m.h"

/**
 * Each entry in the configVector macTable is at a certain position.
 *
 * First position the MAC address of a specific switch is entered
 * Second position the global streamID of an Ethernet frame is stored.
 * Third position the MAC address of the next connected module (hop) to
 * which the Ethernet frame will be forwarded is stored.
 */
enum macTableEntriesPositon
{
    SWITCH_MAC = 0,
    STREAM_ID = 1,
    NEXT_HOP_MAC = 2,
};

/**
 * Options for printing outputs during the simulation
 *
 * INIT_OUT: prints characteristics about the switch
 * MAC_TABLE_CONFIGURATION_INFO: only set if no entry in
 *           MAC table is set for a specific switch
 */
enum simulationOutputType
{
    INIT_OUTPUT = 0,
    MAC_TABLE_CONFIGURATION_INFO = 1,
};

/**
 * Options for throw an error for error handling
 *
 * INCORRECT_MESSAGE_TYPE: cMessage cannot cast into an Ethernet frame
 * UNKNOWN_STREAM_ID: Entered stream ID in .config-file is unequal to
 *                    streamID in Ethernet frame (no matching entry in
 *                    .config-file)
 * INVALID_NUMBER_MAC_TABLE_ENTRIES: the number of entries in MAC Table
 *                    and therefore in .config-file is unequal 3
 * INVALID_PORT_ID: No matching portID to connected next module MAC (hop)
 *                    is found.
 */
enum errorType
{
    INCORRECT_MESSAGE_TYPE = 0,
    UNKNOWN_STREAM_ID = 1,
    INVALID_NUMBER_MAC_TABLE_ENTRIES = 2,
    INVALID_PORT_ID = 3,
};

/**
 * Module ForwardingLogic() is part of Switch() module as well as the
 * IngressPort() and EgressPortModule(). The task of the ForwardingLogic() is
 * to get from the MAC table the right gate index to forward an Ethernet
 * frame.
 */
class ForwardingLogic : public cSimpleModule
{
public:
    /**
     * constructor: nothing to do here
     */
    ForwardingLogic();

    /**
     * destructor: nothing to do here
     */
    virtual ~ForwardingLogic();

    /**
     * The initialization of the switch. The MAC address of the switch is
     * set, the MAC Table for this switch is generated and some outputs
     * are made.
     */
    virtual void initialize() override;

    /**
     * Handle an incoming message. Cast it to an Ethernet frame and check
     * if it is forwarded only to one end node (unicast) or several nodes
     * (multicast)
     */
    void handleMessage(cMessage* message) override;

private:
    int switchIndex;
    int switchMAC;
    int streamID;
    cModule* parentSwitch;
    std::string macTableFile;
    configVector macTable;
    EthernetFrame* ethernetFrame;


    /**
     * Get the all MAC Table entries.
     *
     * MAC Table is defined as a .config-file by user. All MAC address of the
     * network are entered in .config.-file.
     *
     * @returns the MAC Table which is defined in the .config file as vector of vectors of integers
     */
    configVector getMACTable();


    /**
     * Get only the MAC Table entries which affects this switch.
     *
     * A given MAC Table configVector is searched iteratively according to entries
     * that are only intended for it.
     *
     * @param macTableEntries a configVector that contains the MACTable which is set by user
     *                        and generated form ConfigurationReader
     * @returns a MACTable which includes only entries intended for a specific switch
     */
    configVector getOnlyEntriesForThisSwitch(configVector macTableEntries);


    /**
     * A incoming Ethernet frame is forwarded by global and unique streamID
     *
     * The global stream ID is obtained from an Ethernet frame. The Port ID of the next module
     * (next Hop) is determined to which the Ethernet frame is forwarded is determined from
     * MAC Table. A distinction is made as to whether the frame is transmitted unicast
     * (only to one end node) or multicast (to several end nodes)
     *
     * @param ethernetFrame pointer to a Ethernet frame containing the information about
     *                      global stream ID, among others
     * @returns true, if the next portID is found to which the Ethernet frame is forwarded
     */
    bool forwardingByStreamID(EthernetFrame* ethernetFrame);


    /**
     * Transmit the Ethernet frame to only one end node (unicast).
     *
     * An Ethernet frame is transmitted only to one end node by global stream ID and
     * connected module MAC (next hop). It is iterated over all MAC table entries and
     * checked if the stream ID entry is equal to stream ID of Ethernet frame. With the
     * same stream ID the port of the next connected Module is determined. Since only
     * one end node receives the Ethernet frame, the MAC Table entry search is aborted
     * after transmitting the frame.
     *
     * @param ethernetFrame pointer to Ethernet Frame containing the information about
     *                      global stream ID, among others
     * @returns true, if the next portID is found to which the Ethernet frame is forwarded
     */
    bool dealWithUnicast(EthernetFrame* ethernetFrame);

    /**
     * Transmit the Ethernet frame to several end nodes (multicast).
     *
     * An Ethernet frame is transmitted to several end nodes by global stream ID and
     * connected module MAC (next hop). It is iterated over all MAC table entries and
     * checked if the streamID is equal to stream ID of the Ethernet frame. With the same
     * stream ID the port of the next connected Module is determined. By choosing mulitcast
     * it is known that several entries with the same stream ID exist. The Ethernet frame
     * is forwarded to all entries with the same stream ID until there are no more entries
     * in the MAC table are left.
     *
     * @param ethernetFrame pointer to Ethernet frame containing the information about
     *                      global stream ID, among others
     * @returns true, if the next portID is found to which the Ethernet frame is forwarded
     */
    bool dealWithMulticast(EthernetFrame* ethernetFrame);


    /**
     * Checks if the number of entries in the MAC table row is valid
     *
     * The number of  in the is 3. First entry corresponds to the switch MAC address,
     * second entry to global stream ID and the third to the MAC address of the next
     * connected module (hop). With an higher number of entries an error is thrown
     * otherwise the procedure continues.
     *
     * @param macTableEntries is a integer value of a specific row entry in the
     *                        configVector
     */
    void isValidNumberOfEntries(int macTableRow);


    /**
     * Get the port ID of the next connected module (hop)
     *
     * Forwarding by stream ID requires the MAC address of the next connected module (hop).
     * The module MAC address is set in the .config-file by user. With this MAC address the
     * Port ID within the switch is determined. All connected gates in the switch and the
     * corresponding MAC address of the connected modules are searched until the hop MAC
     * address of .config-file is found. If no the hop MAC address does not exist, a -1
     * is returned.
     *
     * @param hopMACAddress integer value of the connected module MAC (next hop)
     *                      get from configVector macTable
     * @returns the portID of the next connected module or -1 if the entry is not
     *          found
     */
    int getPortIDFromNextHOP(int hopMACAddress);

    /**
     * Is true if the next connected module is an EndNode otherwise false.
     *
     * It is important to distinguish Switch() and EndNode() modules. The
     * port MAC address of a Switch() is always the same as the switch MAC
     * address. The port MAC address of EndNodes() can differ from the MAC
     * address of the EndNode().
     *
     * @param nextConnectedModuleGate Pointer to cGate of the next connected module
     * @returns true if the next connected module is an EndNode() otherwise false
     */
    bool isConnectedModuleEndNode(cGate* nextConnectedModuleGate);

    /**
     * Returns true if EndNode() MAC address and hop MAC address is equal,
     * otherwise returns false.
     *
     * The MAC addresses from EndNode() module and EndNode ports can be differ.
     * Therefore, all submodules of type IngressPort() from the EndNode() are
     * checked to get the port MAC address. The MAC address is compared to the
     * nextHopMAC address. With the same MAC the gate index from the switch
     * must be checked. The reason for this is the support of multiple
     * EndNode ports. To transmit the Ethernet frame over the correct channel,
     * the gateNumber must be equal to the gateIndex.
     *
     * @param gateNumber Integer value, contains the gate index of switch
     * @param hopMACAddress Integer value of the next hop MAC address
     * @param connectedModule Pointer to cModule of the next connected module
     * @returns true if EndNOde() MAC address and hop MAC address are equal
     */
    bool isHopMACAddressEqualEndNodeMAC(int gateNumber, int hopMACAddress, cModule* connectedModule);

    /**
     * Returns true if the Switch() MAC addres is equal to the next hop MAC
     * address, otherwise false.
     *
     * The MAC address from switch ports and switch module is the same. With
     * connectedModules the MAC address of the connected switch is obtained.
     * These MAC address is compared to the hopMACAddress.
     *
     * @param hopMACAddress Integer value of next hop MAC address
     * @param connectedModule Pointer to cModule of the next connected Module
     * @returns true if Switch() MAC address is equal to hop MAC address
     */
    bool isHopMACAddressEqualSwitchMAC(int hopMACAddress, cModule* connectedModule);


    /**
     * Checks if the portID is valid.
     *
     * If the MAC address of the next connected module (hop) does not exist
     * a -1 is returned. This functions checks if portID is -1 one. If this is the case,
     * an error is thrown.
     *
     * @param portID integer value of the MAC address of the next connected Module
     * @returns true, if the value is lower than 0 are greater than the number of
     *          connected gates in the switch, otherwise false
     */
    bool portIDInvalid(int portID);


    /**
     * A collection of stream outputs which are shown during the simulation.
     *
     * It contains only information for the user.
     * Type 1: INIT_OUTPUT print the MAC address of the initialized switch as
     *         well as the number of loaded elements
     * Type 2: MAC_TABLE_CONFIGURATION_INFO is printed if there is no entry in
     *         MAC table for a switch.
     *
     * @param type value of enum simulationOutputType.
     */
    void printSimulationOutputs(simulationOutputType type);

    /**
     * A collection of output streams to handle (throw) errors.
     *
     * Error 1 INCORRECT_MESSAGE_TYPE: cMessage cannot cast the Ethernet frame
     * Error 2 UNKNOWN_STREAM_ID: stream ID in .config-file does not correspond
     *                            to stream ID of the Ethernet frame
     * Error 3 INVALID_NUMBER_MAC_TABLE_ENTRIES: the number of entries in the mac Table
     *                            set by user is invalid
     * Error 4 INVALID_PORT_ID: There is no Port for given next connected module
     *                            MAC address
     *
     * @param type value of enum errorType.
     */
    void errorHandling(errorType type);

};

Define_Module(ForwardingLogic);

#endif /* SRC_SWITCH_FORWARDINGLOGIC_FORWARDINGLOGIC_H_ */
