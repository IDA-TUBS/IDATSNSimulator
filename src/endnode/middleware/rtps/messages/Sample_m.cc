//
// Generated file, do not edit! Created by nedtool 5.5 from src/endnode/middleware/rtps/messages/Sample.msg.
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
#include "Sample_m.h"

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

Register_Class(Sample)

Sample::Sample(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->size = 0;
    this->sequenceNumber = 0;
    this->globalStreamId = 0;
    this->generationTime = 0;
    this->writerArrivalTime = 0;
    this->writerFirstInjectionTime = 0;
    this->subscriberReceiveTime = 0;
    this->readerEntityId = 0;
    this->readerParticipantEntityID = 0;
    this->writerEntityId = 0;
    this->writerParticipantEntityID = 0;
    this->deadlineViolation = false;
    this->lifespanExpired = false;
    this->historySizeExeeded = false;
}

Sample::Sample(const Sample& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Sample::~Sample()
{
}

Sample& Sample::operator=(const Sample& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Sample::copy(const Sample& other)
{
    this->size = other.size;
    this->sequenceNumber = other.sequenceNumber;
    this->globalStreamId = other.globalStreamId;
    this->generationTime = other.generationTime;
    this->writerArrivalTime = other.writerArrivalTime;
    this->writerFirstInjectionTime = other.writerFirstInjectionTime;
    this->subscriberReceiveTime = other.subscriberReceiveTime;
    this->readerEntityId = other.readerEntityId;
    this->readerParticipantEntityID = other.readerParticipantEntityID;
    this->writerEntityId = other.writerEntityId;
    this->writerParticipantEntityID = other.writerParticipantEntityID;
    this->deadlineViolation = other.deadlineViolation;
    this->lifespanExpired = other.lifespanExpired;
    this->historySizeExeeded = other.historySizeExeeded;
}

void Sample::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->size);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->globalStreamId);
    doParsimPacking(b,this->generationTime);
    doParsimPacking(b,this->writerArrivalTime);
    doParsimPacking(b,this->writerFirstInjectionTime);
    doParsimPacking(b,this->subscriberReceiveTime);
    doParsimPacking(b,this->readerEntityId);
    doParsimPacking(b,this->readerParticipantEntityID);
    doParsimPacking(b,this->writerEntityId);
    doParsimPacking(b,this->writerParticipantEntityID);
    doParsimPacking(b,this->deadlineViolation);
    doParsimPacking(b,this->lifespanExpired);
    doParsimPacking(b,this->historySizeExeeded);
}

void Sample::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->size);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->globalStreamId);
    doParsimUnpacking(b,this->generationTime);
    doParsimUnpacking(b,this->writerArrivalTime);
    doParsimUnpacking(b,this->writerFirstInjectionTime);
    doParsimUnpacking(b,this->subscriberReceiveTime);
    doParsimUnpacking(b,this->readerEntityId);
    doParsimUnpacking(b,this->readerParticipantEntityID);
    doParsimUnpacking(b,this->writerEntityId);
    doParsimUnpacking(b,this->writerParticipantEntityID);
    doParsimUnpacking(b,this->deadlineViolation);
    doParsimUnpacking(b,this->lifespanExpired);
    doParsimUnpacking(b,this->historySizeExeeded);
}

int Sample::getSize() const
{
    return this->size;
}

void Sample::setSize(int size)
{
    this->size = size;
}

int Sample::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void Sample::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

int Sample::getGlobalStreamId() const
{
    return this->globalStreamId;
}

void Sample::setGlobalStreamId(int globalStreamId)
{
    this->globalStreamId = globalStreamId;
}

::omnetpp::simtime_t Sample::getGenerationTime() const
{
    return this->generationTime;
}

void Sample::setGenerationTime(::omnetpp::simtime_t generationTime)
{
    this->generationTime = generationTime;
}

::omnetpp::simtime_t Sample::getWriterArrivalTime() const
{
    return this->writerArrivalTime;
}

void Sample::setWriterArrivalTime(::omnetpp::simtime_t writerArrivalTime)
{
    this->writerArrivalTime = writerArrivalTime;
}

::omnetpp::simtime_t Sample::getWriterFirstInjectionTime() const
{
    return this->writerFirstInjectionTime;
}

void Sample::setWriterFirstInjectionTime(::omnetpp::simtime_t writerFirstInjectionTime)
{
    this->writerFirstInjectionTime = writerFirstInjectionTime;
}

::omnetpp::simtime_t Sample::getSubscriberReceiveTime() const
{
    return this->subscriberReceiveTime;
}

void Sample::setSubscriberReceiveTime(::omnetpp::simtime_t subscriberReceiveTime)
{
    this->subscriberReceiveTime = subscriberReceiveTime;
}

int Sample::getReaderEntityId() const
{
    return this->readerEntityId;
}

void Sample::setReaderEntityId(int readerEntityId)
{
    this->readerEntityId = readerEntityId;
}

int Sample::getReaderParticipantEntityID() const
{
    return this->readerParticipantEntityID;
}

void Sample::setReaderParticipantEntityID(int readerParticipantEntityID)
{
    this->readerParticipantEntityID = readerParticipantEntityID;
}

int Sample::getWriterEntityId() const
{
    return this->writerEntityId;
}

void Sample::setWriterEntityId(int writerEntityId)
{
    this->writerEntityId = writerEntityId;
}

int Sample::getWriterParticipantEntityID() const
{
    return this->writerParticipantEntityID;
}

void Sample::setWriterParticipantEntityID(int writerParticipantEntityID)
{
    this->writerParticipantEntityID = writerParticipantEntityID;
}

bool Sample::getDeadlineViolation() const
{
    return this->deadlineViolation;
}

void Sample::setDeadlineViolation(bool deadlineViolation)
{
    this->deadlineViolation = deadlineViolation;
}

bool Sample::getLifespanExpired() const
{
    return this->lifespanExpired;
}

void Sample::setLifespanExpired(bool lifespanExpired)
{
    this->lifespanExpired = lifespanExpired;
}

bool Sample::getHistorySizeExeeded() const
{
    return this->historySizeExeeded;
}

void Sample::setHistorySizeExeeded(bool historySizeExeeded)
{
    this->historySizeExeeded = historySizeExeeded;
}

class SampleDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SampleDescriptor();
    virtual ~SampleDescriptor();

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

Register_ClassDescriptor(SampleDescriptor)

SampleDescriptor::SampleDescriptor() : omnetpp::cClassDescriptor("Sample", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

SampleDescriptor::~SampleDescriptor()
{
    delete[] propertynames;
}

bool SampleDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Sample *>(obj)!=nullptr;
}

const char **SampleDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SampleDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SampleDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 14+basedesc->getFieldCount() : 14;
}

unsigned int SampleDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<14) ? fieldTypeFlags[field] : 0;
}

const char *SampleDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "size",
        "sequenceNumber",
        "globalStreamId",
        "generationTime",
        "writerArrivalTime",
        "writerFirstInjectionTime",
        "subscriberReceiveTime",
        "readerEntityId",
        "readerParticipantEntityID",
        "writerEntityId",
        "writerParticipantEntityID",
        "deadlineViolation",
        "lifespanExpired",
        "historySizeExeeded",
    };
    return (field>=0 && field<14) ? fieldNames[field] : nullptr;
}

int SampleDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+1;
    if (fieldName[0]=='g' && strcmp(fieldName, "globalStreamId")==0) return base+2;
    if (fieldName[0]=='g' && strcmp(fieldName, "generationTime")==0) return base+3;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerArrivalTime")==0) return base+4;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerFirstInjectionTime")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "subscriberReceiveTime")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "readerEntityId")==0) return base+7;
    if (fieldName[0]=='r' && strcmp(fieldName, "readerParticipantEntityID")==0) return base+8;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerEntityId")==0) return base+9;
    if (fieldName[0]=='w' && strcmp(fieldName, "writerParticipantEntityID")==0) return base+10;
    if (fieldName[0]=='d' && strcmp(fieldName, "deadlineViolation")==0) return base+11;
    if (fieldName[0]=='l' && strcmp(fieldName, "lifespanExpired")==0) return base+12;
    if (fieldName[0]=='h' && strcmp(fieldName, "historySizeExeeded")==0) return base+13;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SampleDescriptor::getFieldTypeString(int field) const
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
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "int",
        "int",
        "int",
        "int",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<14) ? fieldTypeStrings[field] : nullptr;
}

const char **SampleDescriptor::getFieldPropertyNames(int field) const
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

const char *SampleDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int SampleDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Sample *pp = (Sample *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *SampleDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Sample *pp = (Sample *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SampleDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Sample *pp = (Sample *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSize());
        case 1: return long2string(pp->getSequenceNumber());
        case 2: return long2string(pp->getGlobalStreamId());
        case 3: return simtime2string(pp->getGenerationTime());
        case 4: return simtime2string(pp->getWriterArrivalTime());
        case 5: return simtime2string(pp->getWriterFirstInjectionTime());
        case 6: return simtime2string(pp->getSubscriberReceiveTime());
        case 7: return long2string(pp->getReaderEntityId());
        case 8: return long2string(pp->getReaderParticipantEntityID());
        case 9: return long2string(pp->getWriterEntityId());
        case 10: return long2string(pp->getWriterParticipantEntityID());
        case 11: return bool2string(pp->getDeadlineViolation());
        case 12: return bool2string(pp->getLifespanExpired());
        case 13: return bool2string(pp->getHistorySizeExeeded());
        default: return "";
    }
}

bool SampleDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Sample *pp = (Sample *)object; (void)pp;
    switch (field) {
        case 0: pp->setSize(string2long(value)); return true;
        case 1: pp->setSequenceNumber(string2long(value)); return true;
        case 2: pp->setGlobalStreamId(string2long(value)); return true;
        case 3: pp->setGenerationTime(string2simtime(value)); return true;
        case 4: pp->setWriterArrivalTime(string2simtime(value)); return true;
        case 5: pp->setWriterFirstInjectionTime(string2simtime(value)); return true;
        case 6: pp->setSubscriberReceiveTime(string2simtime(value)); return true;
        case 7: pp->setReaderEntityId(string2long(value)); return true;
        case 8: pp->setReaderParticipantEntityID(string2long(value)); return true;
        case 9: pp->setWriterEntityId(string2long(value)); return true;
        case 10: pp->setWriterParticipantEntityID(string2long(value)); return true;
        case 11: pp->setDeadlineViolation(string2bool(value)); return true;
        case 12: pp->setLifespanExpired(string2bool(value)); return true;
        case 13: pp->setHistorySizeExeeded(string2bool(value)); return true;
        default: return false;
    }
}

const char *SampleDescriptor::getFieldStructName(int field) const
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

void *SampleDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Sample *pp = (Sample *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


