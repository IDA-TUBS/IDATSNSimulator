//
// Generated file, do not edit! Created by nedtool 5.5 from src/endnode/middleware/rtps/messages/RTPSEthMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "RTPSEthMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(RTPSEthMessage)

RTPSEthMessage::RTPSEthMessage(const char *name, short kind) : ::EthernetFrame(name,kind)
{
    this->payloadSize = 0;
    this->protocolOverhead = 0;
    this->rtpsMessageSize = 0;
    this->sourceMac = 0;
    this->destinationMac = 0;
    this->generationTime = 0;
    this->writerArrivalTime = 0;
    this->writerFirstInjectionTime = 0;
    this->subscriberReceiveTime = 0;
    this->globalStreamId = 0;
    this->rtpsMsgSequenceId = 0;
    this->transportPriority = 0;
    this->ethernetPriority = 0;
    this->generalFragmentSize = 0;
    this->sourceNodeId = 0;
    this->sourceParticipantId = 0;
    this->ackNackIsSet = false;
    this->readerId = 0;
    this->writerId = 0;
    this->readerSNStateBase = 0;
    this->readerSNStateNbrBits = 0;
    for (unsigned int i=0; i<256; i++)
        this->readerSNStateBitmap[i] = false;
    this->dataIsSet = false;
    this->writerSN = 0;
    this->sampleSize = 0;
    this->dataFragIsSet = false;
    this->fragmentStartingNum = 0;
    this->fragmentsInSubmessage = 0;
    this->fragmentSize = 0;
    this->heartBeatIsSet = false;
    this->firstSN = 0;
    this->lastSN = 0;
    this->heartBeatFragIsSet = false;
    this->infoDestinationIsSet = false;
    this->destinationNodeId = 0;
    this->destinationParticipantId = 0;
    this->infoTimestampIsSet = false;
    this->timestamp = 0;
    this->nackFragIsSet = false;
    this->fragmentNumberStateBase = 0;
    this->fragmentNumberStateNbrBits = 0;
    for (unsigned int i=0; i<256; i++)
        this->fragmentNumberBitmap[i] = false;
}

RTPSEthMessage::RTPSEthMessage(const RTPSEthMessage& other) : ::EthernetFrame(other)
{
    copy(other);
}

RTPSEthMessage::~RTPSEthMessage()
{
}

RTPSEthMessage& RTPSEthMessage::operator=(const RTPSEthMessage& other)
{
    if (this==&other) return *this;
    ::EthernetFrame::operator=(other);
    copy(other);
    return *this;
}

void RTPSEthMessage::copy(const RTPSEthMessage& other)
{
    this->payloadSize = other.payloadSize;
    this->protocolOverhead = other.protocolOverhead;
    this->rtpsMessageSize = other.rtpsMessageSize;
    this->sourceMac = other.sourceMac;
    this->destinationMac = other.destinationMac;
    this->generationTime = other.generationTime;
    this->writerArrivalTime = other.writerArrivalTime;
    this->writerFirstInjectionTime = other.writerFirstInjectionTime;
    this->subscriberReceiveTime = other.subscriberReceiveTime;
    this->globalStreamId = other.globalStreamId;
    this->rtpsMsgSequenceId = other.rtpsMsgSequenceId;
    this->transportPriority = other.transportPriority;
    this->ethernetPriority = other.ethernetPriority;
    this->generalFragmentSize = other.generalFragmentSize;
    this->sourceNodeId = other.sourceNodeId;
    this->sourceParticipantId = other.sourceParticipantId;
    this->ackNackIsSet = other.ackNackIsSet;
    this->readerId = other.readerId;
    this->writerId = other.writerId;
    this->readerSNStateBase = other.readerSNStateBase;
    this->readerSNStateNbrBits = other.readerSNStateNbrBits;
    for (unsigned int i=0; i<256; i++)
        this->readerSNStateBitmap[i] = other.readerSNStateBitmap[i];
    this->dataIsSet = other.dataIsSet;
    this->writerSN = other.writerSN;
    this->sampleSize = other.sampleSize;
    this->dataFragIsSet = other.dataFragIsSet;
    this->fragmentStartingNum = other.fragmentStartingNum;
    this->fragmentsInSubmessage = other.fragmentsInSubmessage;
    this->fragmentSize = other.fragmentSize;
    this->heartBeatIsSet = other.heartBeatIsSet;
    this->firstSN = other.firstSN;
    this->lastSN = other.lastSN;
    this->heartBeatFragIsSet = other.heartBeatFragIsSet;
    this->infoDestinationIsSet = other.infoDestinationIsSet;
    this->destinationNodeId = other.destinationNodeId;
    this->destinationParticipantId = other.destinationParticipantId;
    this->infoTimestampIsSet = other.infoTimestampIsSet;
    this->timestamp = other.timestamp;
    this->nackFragIsSet = other.nackFragIsSet;
    this->fragmentNumberStateBase = other.fragmentNumberStateBase;
    this->fragmentNumberStateNbrBits = other.fragmentNumberStateNbrBits;
    for (unsigned int i=0; i<256; i++)
        this->fragmentNumberBitmap[i] = other.fragmentNumberBitmap[i];
}

void RTPSEthMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::EthernetFrame::parsimPack(b);
    doParsimPacking(b,this->payloadSize);
    doParsimPacking(b,this->protocolOverhead);
    doParsimPacking(b,this->rtpsMessageSize);
    doParsimPacking(b,this->sourceMac);
    doParsimPacking(b,this->destinationMac);
    doParsimPacking(b,this->generationTime);
    doParsimPacking(b,this->writerArrivalTime);
    doParsimPacking(b,this->writerFirstInjectionTime);
    doParsimPacking(b,this->subscriberReceiveTime);
    doParsimPacking(b,this->globalStreamId);
    doParsimPacking(b,this->rtpsMsgSequenceId);
    doParsimPacking(b,this->transportPriority);
    doParsimPacking(b,this->ethernetPriority);
    doParsimPacking(b,this->generalFragmentSize);
    doParsimPacking(b,this->sourceNodeId);
    doParsimPacking(b,this->sourceParticipantId);
    doParsimPacking(b,this->ackNackIsSet);
    doParsimPacking(b,this->readerId);
    doParsimPacking(b,this->writerId);
    doParsimPacking(b,this->readerSNStateBase);
    doParsimPacking(b,this->readerSNStateNbrBits);
    doParsimArrayPacking(b,this->readerSNStateBitmap,256);
    doParsimPacking(b,this->dataIsSet);
    doParsimPacking(b,this->writerSN);
    doParsimPacking(b,this->sampleSize);
    doParsimPacking(b,this->dataFragIsSet);
    doParsimPacking(b,this->fragmentStartingNum);
    doParsimPacking(b,this->fragmentsInSubmessage);
    doParsimPacking(b,this->fragmentSize);
    doParsimPacking(b,this->heartBeatIsSet);
    doParsimPacking(b,this->firstSN);
    doParsimPacking(b,this->lastSN);
    doParsimPacking(b,this->heartBeatFragIsSet);
    doParsimPacking(b,this->infoDestinationIsSet);
    doParsimPacking(b,this->destinationNodeId);
    doParsimPacking(b,this->destinationParticipantId);
    doParsimPacking(b,this->infoTimestampIsSet);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->nackFragIsSet);
    doParsimPacking(b,this->fragmentNumberStateBase);
    doParsimPacking(b,this->fragmentNumberStateNbrBits);
    doParsimArrayPacking(b,this->fragmentNumberBitmap,256);
}

void RTPSEthMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::EthernetFrame::parsimUnpack(b);
    doParsimUnpacking(b,this->payloadSize);
    doParsimUnpacking(b,this->protocolOverhead);
    doParsimUnpacking(b,this->rtpsMessageSize);
    doParsimUnpacking(b,this->sourceMac);
    doParsimUnpacking(b,this->destinationMac);
    doParsimUnpacking(b,this->generationTime);
    doParsimUnpacking(b,this->writerArrivalTime);
    doParsimUnpacking(b,this->writerFirstInjectionTime);
    doParsimUnpacking(b,this->subscriberReceiveTime);
    doParsimUnpacking(b,this->globalStreamId);
    doParsimUnpacking(b,this->rtpsMsgSequenceId);
    doParsimUnpacking(b,this->transportPriority);
    doParsimUnpacking(b,this->ethernetPriority);
    doParsimUnpacking(b,this->generalFragmentSize);
    doParsimUnpacking(b,this->sourceNodeId);
    doParsimUnpacking(b,this->sourceParticipantId);
    doParsimUnpacking(b,this->ackNackIsSet);
    doParsimUnpacking(b,this->readerId);
    doParsimUnpacking(b,this->writerId);
    doParsimUnpacking(b,this->readerSNStateBase);
    doParsimUnpacking(b,this->readerSNStateNbrBits);
    doParsimArrayUnpacking(b,this->readerSNStateBitmap,256);
    doParsimUnpacking(b,this->dataIsSet);
    doParsimUnpacking(b,this->writerSN);
    doParsimUnpacking(b,this->sampleSize);
    doParsimUnpacking(b,this->dataFragIsSet);
    doParsimUnpacking(b,this->fragmentStartingNum);
    doParsimUnpacking(b,this->fragmentsInSubmessage);
    doParsimUnpacking(b,this->fragmentSize);
    doParsimUnpacking(b,this->heartBeatIsSet);
    doParsimUnpacking(b,this->firstSN);
    doParsimUnpacking(b,this->lastSN);
    doParsimUnpacking(b,this->heartBeatFragIsSet);
    doParsimUnpacking(b,this->infoDestinationIsSet);
    doParsimUnpacking(b,this->destinationNodeId);
    doParsimUnpacking(b,this->destinationParticipantId);
    doParsimUnpacking(b,this->infoTimestampIsSet);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->nackFragIsSet);
    doParsimUnpacking(b,this->fragmentNumberStateBase);
    doParsimUnpacking(b,this->fragmentNumberStateNbrBits);
    doParsimArrayUnpacking(b,this->fragmentNumberBitmap,256);
}

int RTPSEthMessage::getPayloadSize() const
{
    return this->payloadSize;
}

void RTPSEthMessage::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

int RTPSEthMessage::getProtocolOverhead() const
{
    return this->protocolOverhead;
}

void RTPSEthMessage::setProtocolOverhead(int protocolOverhead)
{
    this->protocolOverhead = protocolOverhead;
}

int RTPSEthMessage::getRtpsMessageSize() const
{
    return this->rtpsMessageSize;
}

void RTPSEthMessage::setRtpsMessageSize(int rtpsMessageSize)
{
    this->rtpsMessageSize = rtpsMessageSize;
}

int RTPSEthMessage::getSourceMac() const
{
    return this->sourceMac;
}

void RTPSEthMessage::setSourceMac(int sourceMac)
{
    this->sourceMac = sourceMac;
}

int RTPSEthMessage::getDestinationMac() const
{
    return this->destinationMac;
}

void RTPSEthMessage::setDestinationMac(int destinationMac)
{
    this->destinationMac = destinationMac;
}

::omnetpp::simtime_t RTPSEthMessage::getGenerationTime() const
{
    return this->generationTime;
}

void RTPSEthMessage::setGenerationTime(::omnetpp::simtime_t generationTime)
{
    this->generationTime = generationTime;
}

::omnetpp::simtime_t RTPSEthMessage::getWriterArrivalTime() const
{
    return this->writerArrivalTime;
}

void RTPSEthMessage::setWriterArrivalTime(::omnetpp::simtime_t writerArrivalTime)
{
    this->writerArrivalTime = writerArrivalTime;
}

::omnetpp::simtime_t RTPSEthMessage::getWriterFirstInjectionTime() const
{
    return this->writerFirstInjectionTime;
}

void RTPSEthMessage::setWriterFirstInjectionTime(::omnetpp::simtime_t writerFirstInjectionTime)
{
    this->writerFirstInjectionTime = writerFirstInjectionTime;
}

::omnetpp::simtime_t RTPSEthMessage::getSubscriberReceiveTime() const
{
    return this->subscriberReceiveTime;
}

void RTPSEthMessage::setSubscriberReceiveTime(::omnetpp::simtime_t subscriberReceiveTime)
{
    this->subscriberReceiveTime = subscriberReceiveTime;
}

int RTPSEthMessage::getGlobalStreamId() const
{
    return this->globalStreamId;
}

void RTPSEthMessage::setGlobalStreamId(int globalStreamId)
{
    this->globalStreamId = globalStreamId;
}

int64_t RTPSEthMessage::getRtpsMsgSequenceId() const
{
    return this->rtpsMsgSequenceId;
}

void RTPSEthMessage::setRtpsMsgSequenceId(int64_t rtpsMsgSequenceId)
{
    this->rtpsMsgSequenceId = rtpsMsgSequenceId;
}

int RTPSEthMessage::getTransportPriority() const
{
    return this->transportPriority;
}

void RTPSEthMessage::setTransportPriority(int transportPriority)
{
    this->transportPriority = transportPriority;
}

int RTPSEthMessage::getEthernetPriority() const
{
    return this->ethernetPriority;
}

void RTPSEthMessage::setEthernetPriority(int ethernetPriority)
{
    this->ethernetPriority = ethernetPriority;
}

int RTPSEthMessage::getGeneralFragmentSize() const
{
    return this->generalFragmentSize;
}

void RTPSEthMessage::setGeneralFragmentSize(int generalFragmentSize)
{
    this->generalFragmentSize = generalFragmentSize;
}

int RTPSEthMessage::getSourceNodeId() const
{
    return this->sourceNodeId;
}

void RTPSEthMessage::setSourceNodeId(int sourceNodeId)
{
    this->sourceNodeId = sourceNodeId;
}

int RTPSEthMessage::getSourceParticipantId() const
{
    return this->sourceParticipantId;
}

void RTPSEthMessage::setSourceParticipantId(int sourceParticipantId)
{
    this->sourceParticipantId = sourceParticipantId;
}

bool RTPSEthMessage::getAckNackIsSet() const
{
    return this->ackNackIsSet;
}

void RTPSEthMessage::setAckNackIsSet(bool ackNackIsSet)
{
    this->ackNackIsSet = ackNackIsSet;
}

int RTPSEthMessage::getReaderId() const
{
    return this->readerId;
}

void RTPSEthMessage::setReaderId(int readerId)
{
    this->readerId = readerId;
}

int RTPSEthMessage::getWriterId() const
{
    return this->writerId;
}

void RTPSEthMessage::setWriterId(int writerId)
{
    this->writerId = writerId;
}

int RTPSEthMessage::getReaderSNStateBase() const
{
    return this->readerSNStateBase;
}

void RTPSEthMessage::setReaderSNStateBase(int readerSNStateBase)
{
    this->readerSNStateBase = readerSNStateBase;
}

int RTPSEthMessage::getReaderSNStateNbrBits() const
{
    return this->readerSNStateNbrBits;
}

void RTPSEthMessage::setReaderSNStateNbrBits(int readerSNStateNbrBits)
{
    this->readerSNStateNbrBits = readerSNStateNbrBits;
}

unsigned int RTPSEthMessage::getReaderSNStateBitmapArraySize() const
{
    return 256;
}

bool RTPSEthMessage::getReaderSNStateBitmap(unsigned int k) const
{
    if (k>=256) throw omnetpp::cRuntimeError("Array of size 256 indexed by %lu", (unsigned long)k);
    return this->readerSNStateBitmap[k];
}

void RTPSEthMessage::setReaderSNStateBitmap(unsigned int k, bool readerSNStateBitmap)
{
    if (k>=256) throw omnetpp::cRuntimeError("Array of size 256 indexed by %lu", (unsigned long)k);
    this->readerSNStateBitmap[k] = readerSNStateBitmap;
}

bool RTPSEthMessage::getDataIsSet() const
{
    return this->dataIsSet;
}

void RTPSEthMessage::setDataIsSet(bool dataIsSet)
{
    this->dataIsSet = dataIsSet;
}

int RTPSEthMessage::getWriterSN() const
{
    return this->writerSN;
}

void RTPSEthMessage::setWriterSN(int writerSN)
{
    this->writerSN = writerSN;
}

int RTPSEthMessage::getSampleSize() const
{
    return this->sampleSize;
}

void RTPSEthMessage::setSampleSize(int sampleSize)
{
    this->sampleSize = sampleSize;
}

bool RTPSEthMessage::getDataFragIsSet() const
{
    return this->dataFragIsSet;
}

void RTPSEthMessage::setDataFragIsSet(bool dataFragIsSet)
{
    this->dataFragIsSet = dataFragIsSet;
}

int RTPSEthMessage::getFragmentStartingNum() const
{
    return this->fragmentStartingNum;
}

void RTPSEthMessage::setFragmentStartingNum(int fragmentStartingNum)
{
    this->fragmentStartingNum = fragmentStartingNum;
}

int RTPSEthMessage::getFragmentsInSubmessage() const
{
    return this->fragmentsInSubmessage;
}

void RTPSEthMessage::setFragmentsInSubmessage(int fragmentsInSubmessage)
{
    this->fragmentsInSubmessage = fragmentsInSubmessage;
}

int RTPSEthMessage::getFragmentSize() const
{
    return this->fragmentSize;
}

void RTPSEthMessage::setFragmentSize(int fragmentSize)
{
    this->fragmentSize = fragmentSize;
}

bool RTPSEthMessage::getHeartBeatIsSet() const
{
    return this->heartBeatIsSet;
}

void RTPSEthMessage::setHeartBeatIsSet(bool heartBeatIsSet)
{
    this->heartBeatIsSet = heartBeatIsSet;
}

int RTPSEthMessage::getFirstSN() const
{
    return this->firstSN;
}

void RTPSEthMessage::setFirstSN(int firstSN)
{
    this->firstSN = firstSN;
}

int RTPSEthMessage::getLastSN() const
{
    return this->lastSN;
}

void RTPSEthMessage::setLastSN(int lastSN)
{
    this->lastSN = lastSN;
}

bool RTPSEthMessage::getHeartBeatFragIsSet() const
{
    return this->heartBeatFragIsSet;
}

void RTPSEthMessage::setHeartBeatFragIsSet(bool heartBeatFragIsSet)
{
    this->heartBeatFragIsSet = heartBeatFragIsSet;
}

bool RTPSEthMessage::getInfoDestinationIsSet() const
{
    return this->infoDestinationIsSet;
}

void RTPSEthMessage::setInfoDestinationIsSet(bool infoDestinationIsSet)
{
    this->infoDestinationIsSet = infoDestinationIsSet;
}

int RTPSEthMessage::getDestinationNodeId() const
{
    return this->destinationNodeId;
}

void RTPSEthMessage::setDestinationNodeId(int destinationNodeId)
{
    this->destinationNodeId = destinationNodeId;
}

int RTPSEthMessage::getDestinationParticipantId() const
{
    return this->destinationParticipantId;
}

void RTPSEthMessage::setDestinationParticipantId(int destinationParticipantId)
{
    this->destinationParticipantId = destinationParticipantId;
}

bool RTPSEthMessage::getInfoTimestampIsSet() const
{
    return this->infoTimestampIsSet;
}

void RTPSEthMessage::setInfoTimestampIsSet(bool infoTimestampIsSet)
{
    this->infoTimestampIsSet = infoTimestampIsSet;
}

::omnetpp::simtime_t RTPSEthMessage::getTimestamp() const
{
    return this->timestamp;
}

void RTPSEthMessage::setTimestamp(::omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

bool RTPSEthMessage::getNackFragIsSet() const
{
    return this->nackFragIsSet;
}

void RTPSEthMessage::setNackFragIsSet(bool nackFragIsSet)
{
    this->nackFragIsSet = nackFragIsSet;
}

int RTPSEthMessage::getFragmentNumberStateBase() const
{
    return this->fragmentNumberStateBase;
}

void RTPSEthMessage::setFragmentNumberStateBase(int fragmentNumberStateBase)
{
    this->fragmentNumberStateBase = fragmentNumberStateBase;
}

int RTPSEthMessage::getFragmentNumberStateNbrBits() const
{
    return this->fragmentNumberStateNbrBits;
}

void RTPSEthMessage::setFragmentNumberStateNbrBits(int fragmentNumberStateNbrBits)
{
    this->fragmentNumberStateNbrBits = fragmentNumberStateNbrBits;
}

unsigned int RTPSEthMessage::getFragmentNumberBitmapArraySize() const
{
    return 256;
}

bool RTPSEthMessage::getFragmentNumberBitmap(unsigned int k) const
{
    if (k>=256) throw omnetpp::cRuntimeError("Array of size 256 indexed by %lu", (unsigned long)k);
    return this->fragmentNumberBitmap[k];
}

void RTPSEthMessage::setFragmentNumberBitmap(unsigned int k, bool fragmentNumberBitmap)
{
    if (k>=256) throw omnetpp::cRuntimeError("Array of size 256 indexed by %lu", (unsigned long)k);
    this->fragmentNumberBitmap[k] = fragmentNumberBitmap;
}

class RTPSEthMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    RTPSEthMessageDescriptor();
    virtual ~RTPSEthMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(RTPSEthMessageDescriptor)

RTPSEthMessageDescriptor::RTPSEthMessageDescriptor() : omnetpp::cClassDescriptor("RTPSEthMessage", "EthernetFrame")
{
    propertynames = nullptr;
}

RTPSEthMessageDescriptor::~RTPSEthMessageDescriptor()
{
    delete[] propertynames;
}

bool RTPSEthMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RTPSEthMessage *>(obj)!=nullptr;
}

const char **RTPSEthMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *RTPSEthMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int RTPSEthMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 42+basedesc->getFieldCount() : 42;
}

unsigned int RTPSEthMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<42) ? fieldTypeFlags[field] : 0;
}

const char *RTPSEthMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "payloadSize",
        "protocolOverhead",
        "rtpsMessageSize",
        "sourceMac",
        "destinationMac",
        "generationTime",
        "writerArrivalTime",
        "writerFirstInjectionTime",
        "subscriberReceiveTime",
        "globalStreamId",
        "rtpsMsgSequenceId",
        "transportPriority",
        "ethernetPriority",
        "generalFragmentSize",
        "sourceNodeId",
        "sourceParticipantId",
        "ackNackIsSet",
        "readerId",
        "writerId",
        "readerSNStateBase",
        "readerSNStateNbrBits",
        "readerSNStateBitmap",
        "dataIsSet",
        "writerSN",
        "sampleSize",
        "dataFragIsSet",
        "fragmentStartingNum",
        "fragmentsInSubmessage",
        "fragmentSize",
        "heartBeatIsSet",
        "firstSN",
        "lastSN",
        "heartBeatFragIsSet",
        "infoDestinationIsSet",
        "destinationNodeId",
        "destinationParticipantId",
        "infoTimestampIsSet",
        "timestamp",
        "nackFragIsSet",
        "fragmentNumberStateBase",
        "fragmentNumberStateNbrBits",
        "fragmentNumberBitmap",
    };
    return (field>=0 && field<42) ? fieldNames[field] : nullptr;
}

int RTPSEthMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "payloadSize")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "protocolOverhead")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "rtpsMessageSize")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceMac")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationMac")==0) return base+4;
    if (fieldName[0]=='g' && strcmp(fieldName, "generationTime")==0) return base+5;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerArrivalTime")==0) return base+6;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerFirstInjectionTime")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "subscriberReceiveTime")==0) return base+8;
    if (fieldName[0]=='g' && strcmp(fieldName, "globalStreamId")==0) return base+9;
    if (fieldName[0]=='r' && strcmp(fieldName, "rtpsMsgSequenceId")==0) return base+10;
    if (fieldName[0]=='t' && strcmp(fieldName, "transportPriority")==0) return base+11;
    if (fieldName[0]=='e' && strcmp(fieldName, "ethernetPriority")==0) return base+12;
    if (fieldName[0]=='g' && strcmp(fieldName, "generalFragmentSize")==0) return base+13;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceNodeId")==0) return base+14;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceParticipantId")==0) return base+15;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackNackIsSet")==0) return base+16;
    if (fieldName[0]=='r' && strcmp(fieldName, "readerId")==0) return base+17;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerId")==0) return base+18;
    if (fieldName[0]=='r' && strcmp(fieldName, "readerSNStateBase")==0) return base+19;
    if (fieldName[0]=='r' && strcmp(fieldName, "readerSNStateNbrBits")==0) return base+20;
    if (fieldName[0]=='r' && strcmp(fieldName, "readerSNStateBitmap")==0) return base+21;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataIsSet")==0) return base+22;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerSN")==0) return base+23;
    if (fieldName[0]=='s' && strcmp(fieldName, "sampleSize")==0) return base+24;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataFragIsSet")==0) return base+25;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentStartingNum")==0) return base+26;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentsInSubmessage")==0) return base+27;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentSize")==0) return base+28;
    if (fieldName[0]=='h' && strcmp(fieldName, "heartBeatIsSet")==0) return base+29;
    if (fieldName[0]=='f' && strcmp(fieldName, "firstSN")==0) return base+30;
    if (fieldName[0]=='l' && strcmp(fieldName, "lastSN")==0) return base+31;
    if (fieldName[0]=='h' && strcmp(fieldName, "heartBeatFragIsSet")==0) return base+32;
    if (fieldName[0]=='i' && strcmp(fieldName, "infoDestinationIsSet")==0) return base+33;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationNodeId")==0) return base+34;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationParticipantId")==0) return base+35;
    if (fieldName[0]=='i' && strcmp(fieldName, "infoTimestampIsSet")==0) return base+36;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+37;
    if (fieldName[0]=='n' && strcmp(fieldName, "nackFragIsSet")==0) return base+38;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentNumberStateBase")==0) return base+39;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentNumberStateNbrBits")==0) return base+40;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentNumberBitmap")==0) return base+41;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *RTPSEthMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "int",
        "int64_t",
        "int",
        "int",
        "int",
        "int",
        "int",
        "bool",
        "int",
        "int",
        "int",
        "int",
        "bool",
        "bool",
        "int",
        "int",
        "bool",
        "int",
        "int",
        "int",
        "bool",
        "int",
        "int",
        "bool",
        "bool",
        "int",
        "int",
        "bool",
        "simtime_t",
        "bool",
        "int",
        "int",
        "bool",
    };
    return (field>=0 && field<42) ? fieldTypeStrings[field] : nullptr;
}

const char **RTPSEthMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RTPSEthMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RTPSEthMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    RTPSEthMessage *pp = (RTPSEthMessage *)object; (void)pp;
    switch (field) {
        case 21: return 256;
        case 41: return 256;
        default: return 0;
    }
}

const char *RTPSEthMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RTPSEthMessage *pp = (RTPSEthMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RTPSEthMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RTPSEthMessage *pp = (RTPSEthMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPayloadSize());
        case 1: return long2string(pp->getProtocolOverhead());
        case 2: return long2string(pp->getRtpsMessageSize());
        case 3: return long2string(pp->getSourceMac());
        case 4: return long2string(pp->getDestinationMac());
        case 5: return simtime2string(pp->getGenerationTime());
        case 6: return simtime2string(pp->getWriterArrivalTime());
        case 7: return simtime2string(pp->getWriterFirstInjectionTime());
        case 8: return simtime2string(pp->getSubscriberReceiveTime());
        case 9: return long2string(pp->getGlobalStreamId());
        case 10: return int642string(pp->getRtpsMsgSequenceId());
        case 11: return long2string(pp->getTransportPriority());
        case 12: return long2string(pp->getEthernetPriority());
        case 13: return long2string(pp->getGeneralFragmentSize());
        case 14: return long2string(pp->getSourceNodeId());
        case 15: return long2string(pp->getSourceParticipantId());
        case 16: return bool2string(pp->getAckNackIsSet());
        case 17: return long2string(pp->getReaderId());
        case 18: return long2string(pp->getWriterId());
        case 19: return long2string(pp->getReaderSNStateBase());
        case 20: return long2string(pp->getReaderSNStateNbrBits());
        case 21: return bool2string(pp->getReaderSNStateBitmap(i));
        case 22: return bool2string(pp->getDataIsSet());
        case 23: return long2string(pp->getWriterSN());
        case 24: return long2string(pp->getSampleSize());
        case 25: return bool2string(pp->getDataFragIsSet());
        case 26: return long2string(pp->getFragmentStartingNum());
        case 27: return long2string(pp->getFragmentsInSubmessage());
        case 28: return long2string(pp->getFragmentSize());
        case 29: return bool2string(pp->getHeartBeatIsSet());
        case 30: return long2string(pp->getFirstSN());
        case 31: return long2string(pp->getLastSN());
        case 32: return bool2string(pp->getHeartBeatFragIsSet());
        case 33: return bool2string(pp->getInfoDestinationIsSet());
        case 34: return long2string(pp->getDestinationNodeId());
        case 35: return long2string(pp->getDestinationParticipantId());
        case 36: return bool2string(pp->getInfoTimestampIsSet());
        case 37: return simtime2string(pp->getTimestamp());
        case 38: return bool2string(pp->getNackFragIsSet());
        case 39: return long2string(pp->getFragmentNumberStateBase());
        case 40: return long2string(pp->getFragmentNumberStateNbrBits());
        case 41: return bool2string(pp->getFragmentNumberBitmap(i));
        default: return "";
    }
}

bool RTPSEthMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    RTPSEthMessage *pp = (RTPSEthMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setPayloadSize(string2long(value)); return true;
        case 1: pp->setProtocolOverhead(string2long(value)); return true;
        case 2: pp->setRtpsMessageSize(string2long(value)); return true;
        case 3: pp->setSourceMac(string2long(value)); return true;
        case 4: pp->setDestinationMac(string2long(value)); return true;
        case 5: pp->setGenerationTime(string2simtime(value)); return true;
        case 6: pp->setWriterArrivalTime(string2simtime(value)); return true;
        case 7: pp->setWriterFirstInjectionTime(string2simtime(value)); return true;
        case 8: pp->setSubscriberReceiveTime(string2simtime(value)); return true;
        case 9: pp->setGlobalStreamId(string2long(value)); return true;
        case 10: pp->setRtpsMsgSequenceId(string2int64(value)); return true;
        case 11: pp->setTransportPriority(string2long(value)); return true;
        case 12: pp->setEthernetPriority(string2long(value)); return true;
        case 13: pp->setGeneralFragmentSize(string2long(value)); return true;
        case 14: pp->setSourceNodeId(string2long(value)); return true;
        case 15: pp->setSourceParticipantId(string2long(value)); return true;
        case 16: pp->setAckNackIsSet(string2bool(value)); return true;
        case 17: pp->setReaderId(string2long(value)); return true;
        case 18: pp->setWriterId(string2long(value)); return true;
        case 19: pp->setReaderSNStateBase(string2long(value)); return true;
        case 20: pp->setReaderSNStateNbrBits(string2long(value)); return true;
        case 21: pp->setReaderSNStateBitmap(i,string2bool(value)); return true;
        case 22: pp->setDataIsSet(string2bool(value)); return true;
        case 23: pp->setWriterSN(string2long(value)); return true;
        case 24: pp->setSampleSize(string2long(value)); return true;
        case 25: pp->setDataFragIsSet(string2bool(value)); return true;
        case 26: pp->setFragmentStartingNum(string2long(value)); return true;
        case 27: pp->setFragmentsInSubmessage(string2long(value)); return true;
        case 28: pp->setFragmentSize(string2long(value)); return true;
        case 29: pp->setHeartBeatIsSet(string2bool(value)); return true;
        case 30: pp->setFirstSN(string2long(value)); return true;
        case 31: pp->setLastSN(string2long(value)); return true;
        case 32: pp->setHeartBeatFragIsSet(string2bool(value)); return true;
        case 33: pp->setInfoDestinationIsSet(string2bool(value)); return true;
        case 34: pp->setDestinationNodeId(string2long(value)); return true;
        case 35: pp->setDestinationParticipantId(string2long(value)); return true;
        case 36: pp->setInfoTimestampIsSet(string2bool(value)); return true;
        case 37: pp->setTimestamp(string2simtime(value)); return true;
        case 38: pp->setNackFragIsSet(string2bool(value)); return true;
        case 39: pp->setFragmentNumberStateBase(string2long(value)); return true;
        case 40: pp->setFragmentNumberStateNbrBits(string2long(value)); return true;
        case 41: pp->setFragmentNumberBitmap(i,string2bool(value)); return true;
        default: return false;
    }
}

const char *RTPSEthMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *RTPSEthMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    RTPSEthMessage *pp = (RTPSEthMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


