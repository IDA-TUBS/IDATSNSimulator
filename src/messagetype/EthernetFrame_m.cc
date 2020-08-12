//
// Generated file, do not edit! Created by nedtool 5.5 from src/messagetype/EthernetFrame.msg.
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
#include "EthernetFrame_m.h"

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

Register_Class(EthernetFrame)

EthernetFrame::EthernetFrame(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->frameSize = 0;
    this->payloadSize = 0;
    this->packetSize = 0;
    this->MACsource = 0;
    this->MACdest = 0;
    this->priority = 0;
    this->streamID = 0;
    this->isMulticast = false;
    this->isBroadcast = false;
    this->frameSequenceID = 0;
    this->injectionTime = 0;
}

EthernetFrame::EthernetFrame(const EthernetFrame& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

EthernetFrame::~EthernetFrame()
{
}

EthernetFrame& EthernetFrame::operator=(const EthernetFrame& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void EthernetFrame::copy(const EthernetFrame& other)
{
    this->frameSize = other.frameSize;
    this->payloadSize = other.payloadSize;
    this->packetSize = other.packetSize;
    this->MACsource = other.MACsource;
    this->MACdest = other.MACdest;
    this->priority = other.priority;
    this->streamID = other.streamID;
    this->isMulticast = other.isMulticast;
    this->isBroadcast = other.isBroadcast;
    this->frameSequenceID = other.frameSequenceID;
    this->injectionTime = other.injectionTime;
}

void EthernetFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->frameSize);
    doParsimPacking(b,this->payloadSize);
    doParsimPacking(b,this->packetSize);
    doParsimPacking(b,this->MACsource);
    doParsimPacking(b,this->MACdest);
    doParsimPacking(b,this->priority);
    doParsimPacking(b,this->streamID);
    doParsimPacking(b,this->isMulticast);
    doParsimPacking(b,this->isBroadcast);
    doParsimPacking(b,this->frameSequenceID);
    doParsimPacking(b,this->injectionTime);
}

void EthernetFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->frameSize);
    doParsimUnpacking(b,this->payloadSize);
    doParsimUnpacking(b,this->packetSize);
    doParsimUnpacking(b,this->MACsource);
    doParsimUnpacking(b,this->MACdest);
    doParsimUnpacking(b,this->priority);
    doParsimUnpacking(b,this->streamID);
    doParsimUnpacking(b,this->isMulticast);
    doParsimUnpacking(b,this->isBroadcast);
    doParsimUnpacking(b,this->frameSequenceID);
    doParsimUnpacking(b,this->injectionTime);
}

int EthernetFrame::getFrameSize() const
{
    return this->frameSize;
}

void EthernetFrame::setFrameSize(int frameSize)
{
    this->frameSize = frameSize;
}

int EthernetFrame::getPayloadSize() const
{
    return this->payloadSize;
}

void EthernetFrame::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

int EthernetFrame::getPacketSize() const
{
    return this->packetSize;
}

void EthernetFrame::setPacketSize(int packetSize)
{
    this->packetSize = packetSize;
}

int EthernetFrame::getMACsource() const
{
    return this->MACsource;
}

void EthernetFrame::setMACsource(int MACsource)
{
    this->MACsource = MACsource;
}

int EthernetFrame::getMACdest() const
{
    return this->MACdest;
}

void EthernetFrame::setMACdest(int MACdest)
{
    this->MACdest = MACdest;
}

int EthernetFrame::getPriority() const
{
    return this->priority;
}

void EthernetFrame::setPriority(int priority)
{
    this->priority = priority;
}

int EthernetFrame::getStreamID() const
{
    return this->streamID;
}

void EthernetFrame::setStreamID(int streamID)
{
    this->streamID = streamID;
}

bool EthernetFrame::getIsMulticast() const
{
    return this->isMulticast;
}

void EthernetFrame::setIsMulticast(bool isMulticast)
{
    this->isMulticast = isMulticast;
}

bool EthernetFrame::getIsBroadcast() const
{
    return this->isBroadcast;
}

void EthernetFrame::setIsBroadcast(bool isBroadcast)
{
    this->isBroadcast = isBroadcast;
}

int64_t EthernetFrame::getFrameSequenceID() const
{
    return this->frameSequenceID;
}

void EthernetFrame::setFrameSequenceID(int64_t frameSequenceID)
{
    this->frameSequenceID = frameSequenceID;
}

::omnetpp::simtime_t EthernetFrame::getInjectionTime() const
{
    return this->injectionTime;
}

void EthernetFrame::setInjectionTime(::omnetpp::simtime_t injectionTime)
{
    this->injectionTime = injectionTime;
}

class EthernetFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    EthernetFrameDescriptor();
    virtual ~EthernetFrameDescriptor();

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

Register_ClassDescriptor(EthernetFrameDescriptor)

EthernetFrameDescriptor::EthernetFrameDescriptor() : omnetpp::cClassDescriptor("EthernetFrame", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

EthernetFrameDescriptor::~EthernetFrameDescriptor()
{
    delete[] propertynames;
}

bool EthernetFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<EthernetFrame *>(obj)!=nullptr;
}

const char **EthernetFrameDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *EthernetFrameDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int EthernetFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 11+basedesc->getFieldCount() : 11;
}

unsigned int EthernetFrameDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<11) ? fieldTypeFlags[field] : 0;
}

const char *EthernetFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "frameSize",
        "payloadSize",
        "packetSize",
        "MACsource",
        "MACdest",
        "priority",
        "streamID",
        "isMulticast",
        "isBroadcast",
        "frameSequenceID",
        "injectionTime",
    };
    return (field>=0 && field<11) ? fieldNames[field] : nullptr;
}

int EthernetFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "frameSize")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "payloadSize")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetSize")==0) return base+2;
    if (fieldName[0]=='M' && strcmp(fieldName, "MACsource")==0) return base+3;
    if (fieldName[0]=='M' && strcmp(fieldName, "MACdest")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "priority")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "streamID")==0) return base+6;
    if (fieldName[0]=='i' && strcmp(fieldName, "isMulticast")==0) return base+7;
    if (fieldName[0]=='i' && strcmp(fieldName, "isBroadcast")==0) return base+8;
    if (fieldName[0]=='f' && strcmp(fieldName, "frameSequenceID")==0) return base+9;
    if (fieldName[0]=='i' && strcmp(fieldName, "injectionTime")==0) return base+10;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *EthernetFrameDescriptor::getFieldTypeString(int field) const
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
        "int",
        "int",
        "bool",
        "bool",
        "int64_t",
        "simtime_t",
    };
    return (field>=0 && field<11) ? fieldTypeStrings[field] : nullptr;
}

const char **EthernetFrameDescriptor::getFieldPropertyNames(int field) const
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

const char *EthernetFrameDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int EthernetFrameDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    EthernetFrame *pp = (EthernetFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *EthernetFrameDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    EthernetFrame *pp = (EthernetFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string EthernetFrameDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    EthernetFrame *pp = (EthernetFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFrameSize());
        case 1: return long2string(pp->getPayloadSize());
        case 2: return long2string(pp->getPacketSize());
        case 3: return long2string(pp->getMACsource());
        case 4: return long2string(pp->getMACdest());
        case 5: return long2string(pp->getPriority());
        case 6: return long2string(pp->getStreamID());
        case 7: return bool2string(pp->getIsMulticast());
        case 8: return bool2string(pp->getIsBroadcast());
        case 9: return int642string(pp->getFrameSequenceID());
        case 10: return simtime2string(pp->getInjectionTime());
        default: return "";
    }
}

bool EthernetFrameDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    EthernetFrame *pp = (EthernetFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setFrameSize(string2long(value)); return true;
        case 1: pp->setPayloadSize(string2long(value)); return true;
        case 2: pp->setPacketSize(string2long(value)); return true;
        case 3: pp->setMACsource(string2long(value)); return true;
        case 4: pp->setMACdest(string2long(value)); return true;
        case 5: pp->setPriority(string2long(value)); return true;
        case 6: pp->setStreamID(string2long(value)); return true;
        case 7: pp->setIsMulticast(string2bool(value)); return true;
        case 8: pp->setIsBroadcast(string2bool(value)); return true;
        case 9: pp->setFrameSequenceID(string2int64(value)); return true;
        case 10: pp->setInjectionTime(string2simtime(value)); return true;
        default: return false;
    }
}

const char *EthernetFrameDescriptor::getFieldStructName(int field) const
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

void *EthernetFrameDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    EthernetFrame *pp = (EthernetFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


