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

#include "ForwardingLogic.h"

/**
 * default constructor
 */
ForwardingLogic::ForwardingLogic()
{
}

/**
 * default destructor
 */
ForwardingLogic::~ForwardingLogic()
{
}


void ForwardingLogic::initialize()
{
    //get the parent module of the forwarding logic (parent = switch)
    this->parentSwitch = getParentModule();

    //get the MAC address of the module
    this->switchMAC = (int) parentSwitch->par("mac");

    //get the MAC table set by user
    configVector allMACTableEntries = getMACTable();

    this->macTable = getOnlyEntriesForThisSwitch(allMACTableEntries);

    // no configuration is made for this switch
    if(this->macTable.size() == 0)
    {
        printSimulationOutputs(MAC_TABLE_CONFIGURATION_INFO);
    }

    //print initialization information while running the simulation
    printSimulationOutputs(INIT_OUTPUT);
}


void ForwardingLogic::handleMessage(cMessage* message)
{
    //cast to Ethernet frame
    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame *>(message);
    if(ethernetFrame == 0)
    {
        errorHandling(INCORRECT_MESSAGE_TYPE);
    }

    bool foundStreamID;

    //forwarding message by global streamID
    foundStreamID = this->forwardingByStreamID(ethernetFrame);

    //no entry with given streamID in macTable
    if(foundStreamID == false)
    {
        errorHandling(UNKNOWN_STREAM_ID);
    }

    delete (message);
}


configVector ForwardingLogic::getMACTable()
{
    // get the .config file set by user
    std::string tableFile = this->parentSwitch->par("macTable");
    this->macTableFile = tableFile;

    // Singleton ConfigurationReader -> read out the .config file and store it
    configVector allMacTableEntries = ConfigurationReader::getInstance()->getMACTableEntriesFromInputFile(tableFile);

    return allMacTableEntries;
}


configVector ForwardingLogic::getOnlyEntriesForThisSwitch(configVector macTableEntries)
{
    configVector switchMACTable;
    // iterate over all entries in macTable and get the entries which affects this switch
    for(int iterator = 0; iterator < macTableEntries.size(); iterator++)
    {
        // get the switch MAC address in MAC Table
        int switchMACAddress = macTableEntries[iterator][SWITCH_MAC];

        // MAC Table entry is equal to the MAC address of this switch
        if(this->switchMAC == switchMACAddress)
        {
            // yes -> get and store this entries
            std::vector<int> tmpIntVector = macTableEntries[iterator];
            switchMACTable.push_back(tmpIntVector);
        }
    }
    return switchMACTable;
}


bool ForwardingLogic::forwardingByStreamID(EthernetFrame* ethernetFrame)
{
    // get the information if frame is transmitted to several end nodes
    bool multicast = ethernetFrame->getIsMulticast();

    // variable for debugging
    bool foundStreamID;

    if(multicast)
    {
        return foundStreamID = dealWithMulticast(ethernetFrame);
    }
    else
    {
        return foundStreamID = dealWithUnicast(ethernetFrame);
    }

    if(!foundStreamID)
    {
        errorHandling(UNKNOWN_STREAM_ID);
    }
}


bool ForwardingLogic::dealWithUnicast(EthernetFrame* ethernetFrame)
{
    bool foundStreamID = false;

    this->streamID = ethernetFrame->getStreamID();
    int sizeMACTable = this->macTable.size();

    for(int macTableEntry = 0; macTableEntry != sizeMACTable; macTableEntry++)
    {
        isValidNumberOfEntries(macTableEntry);

        int streamIDEntry = this->macTable[macTableEntry][STREAM_ID];

        if(streamIDEntry != this->streamID)
        {
            // streamID in MAC table unequal to streamID of Ethernet frame
            // skip the entry
            continue;
        }

        int hopMAC = this->macTable[macTableEntry][NEXT_HOP_MAC];

        EthernetFrame* ethernetFrameDuplicate = ethernetFrame->dup();

        int portID = getPortIDFromNextHOP(hopMAC);

        if(portIDInvalid(portID))
        {
            errorHandling(INVALID_PORT_ID);
        }

        send(ethernetFrameDuplicate, "out", portID);

        return foundStreamID = true;
    }
    return foundStreamID;
}


bool ForwardingLogic::dealWithMulticast(EthernetFrame* ethernetFrame)
{
    bool foundStreamID = false;

    this->streamID = ethernetFrame->getStreamID();
    int sizeMACTable = this->macTable.size();

    for(int macTableEntry = 0; macTableEntry != sizeMACTable; macTableEntry++)
    {
        // check if only three entries are in MAC Table row
        isValidNumberOfEntries(macTableEntry);

        int streamIDEntry = this->macTable[macTableEntry][STREAM_ID];

        if(streamIDEntry != this->streamID)
        {
            // streamID in MAC table unequal to streamID of Ethernet frame
            // skip the entry
            continue;
        }

        int hopMAC = this->macTable[macTableEntry][NEXT_HOP_MAC];

        EthernetFrame* ethernetFrameDuplicate = ethernetFrame->dup();

        // get the port ID of next connected Module (hop)
        int portID = getPortIDFromNextHOP(hopMAC);

        if(portIDInvalid(portID))
        {
            errorHandling(INVALID_PORT_ID);
        }

        send(ethernetFrameDuplicate, "out", portID);
        foundStreamID = true;
    }

    return foundStreamID;
}


void ForwardingLogic::isValidNumberOfEntries(int macTableRow)
{
    if(this->macTable[macTableRow].size() != 3)
    {
        errorHandling(INVALID_NUMBER_MAC_TABLE_ENTRIES);
    }
}


bool ForwardingLogic::portIDInvalid(int portID)
{
    bool invalid = false;

    if(portID < 0 || portID >= this->parentSwitch->gateSize("ethernetPorts"))
    {
        invalid =  true;
    }
    return invalid;
}

int ForwardingLogic::getPortIDFromNextHOP(int hopMACAddress)
{
    //for each gate index (there should be only one)
    for (int gateIndex = 0; gateIndex < this->parentSwitch->gateSize("ethernetPorts"); gateIndex++)
    {
        cGate* ethernetOutputPort = this->parentSwitch->gate("ethernetPorts$o", gateIndex);
        //get the pointer of the gate from the connected Module
        cGate* nextConnectedModuleGate = ethernetOutputPort->getNextGate();

        //get MAC address of the module to which the sending gate belongs

        bool isEndNode = isConnectedModuleEndNode(nextConnectedModuleGate);
        cModule* connectedModule = nextConnectedModuleGate->getOwnerModule();
        bool foundEntry = false;

        if(isEndNode == true)
        {
            //foundEntry = isHopMACAddressEqualSwitchMAC(hopMACAddress, connectedModule);
            foundEntry = isHopMACAddressEqualEndNodeMAC(gateIndex,hopMACAddress,connectedModule);
        }
        else
        {
            foundEntry = isHopMACAddressEqualSwitchMAC(hopMACAddress, connectedModule);
        }

        if(foundEntry)
        {
            return gateIndex;
        }
    }
    // no neighbor found, no gate index identified
    return -1;
}


bool ForwardingLogic::isConnectedModuleEndNode(cGate* nextConnectedModuleGate)
{
    //connected module on the other side of the channel
    cModule* connectedModule = nextConnectedModuleGate->getOwnerModule();
    const char* moduleClassName = connectedModule->getModuleType()->getName();

    //check if moduleClassName is equal to EndNode module
    if(strcmp(moduleClassName, "EndNode") == 0)
    {
        return true;
    }

    return false;
}


bool ForwardingLogic::isHopMACAddressEqualEndNodeMAC(int gateNumber, int hopMACAddress, cModule* connectedModule)
{
    for(int indexIterator = 0; indexIterator < connectedModule->gateSize("ethernetPorts"); indexIterator++)
    {
        // get the submodule of EndNode
        cModule* ingressPort = connectedModule->getSubmodule("ingressPort", indexIterator);
        // get the port MAC address
        int portMAC = ingressPort->par("mac");

        // check if portMAC is equal
        if(hopMACAddress == portMAC)
        {
            // get the gate from EndNode IngressPort
            cGate* ingressGate = connectedModule->gate("ethernetPorts$i",indexIterator);

            // get the other side of the channel
            cGate* previousGate = ingressGate->getPreviousGate();

            // get the index of these gate
            int gateIndex = previousGate->getIndex();

            if (gateNumber == gateIndex)
            {
                return true;
            }

        }
    }
    return false;
}


bool ForwardingLogic::isHopMACAddressEqualSwitchMAC(int hopMACAddress, cModule* connectedModule)
{
    int connectedMAC = connectedModule->par("mac");

    if (hopMACAddress == connectedMAC)
    {
        //found a neighbor with the given MAC address
        return true;
    }

    return false;
}


void ForwardingLogic::printSimulationOutputs(simulationOutputType type)
{
    const char* moduleName = getFullName();

    switch(type)
    {
        case INIT_OUTPUT:
            EV <<"[" << moduleName << "] MAC table loaded in module: " << this->parentSwitch->getFullName()
            << " with MAC address [" << this->switchMAC <<"] \n";
            EV << "[" << moduleName << "] Elements loaded: " << this->macTable.size() << "\n";
            break;
        case MAC_TABLE_CONFIGURATION_INFO:
            EV<<"**************************************************************************************************\n";
            EV<< "No Configurations are set for switch with MAC address [" << this->switchMAC<< "] in >>"
                    << macTableFile << "<< \n";
            EV<<"**************************************************************************************************\n";
    }
}

void ForwardingLogic::errorHandling(errorType type)
{
    const char* moduleName = getFullName();

    switch(type)
    {
        case INCORRECT_MESSAGE_TYPE:
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";
            EV << "\t [ERROR] in module -" << moduleName << "-. Could not cast message to Ethernet frame.";
            EV << "\n";
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";
            throw cRuntimeError("[ForwardingLogic] ERROR: Could not cast message to Ethernet frame");
            break;

        case UNKNOWN_STREAM_ID:
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";
            EV << "\t [ERROR] in module -" << moduleName << "-. No entry with given global streamID in ["
                    << this->macTableFile << "] found. Expected streamID is [" << this->streamID << "]";
            EV << "\n";
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";
            throw cRuntimeError("[ForwardingLogic] ERROR: No entry with given streamID in macTable");
            break;

        case INVALID_NUMBER_MAC_TABLE_ENTRIES:
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";  
            EV << "\t [ERROR] in module -" << moduleName << "-. Invalid number of entries in macTable ["
                    << this->macTableFile <<"]. Forwarding by streamID requires only 3 entries.";
            EV << "\n";
            EV << "\t 1st SWITCH MAC: MAC address of the current switch [" << this->switchMAC << "] \n";
            EV << "\t 2nd GLOBAL STREAM ID: The unique stream ID of the frame [" << this->streamID << "] \n";
            EV << "\t 3rd HOP MAC: MAC address of the next HOP in the path to the destination. Set by User.";
            EV << "\n";
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";
            throw cRuntimeError("[ForwardingLogic] ERROR: Forwarding by streamID requires 3 entries");
            break;
        
        case INVALID_PORT_ID:
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";  
            EV << "\t [ERROR] in module -" << moduleName << "-. Cannot identify the next portID in Ethernet.";
            EV << "\n";
            EV << ">>>>>>>>>>>>>>>>>>>>>> \n";
            throw cRuntimeError("[ForwardingLogic] ERROR: Could not identify portID in Ethernet forwarding logic!");
            break;
    }
}
