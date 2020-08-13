//
// Generated file, do not edit! Created by nedtool 5.5 from src/messagetype/BufferControlMessage.msg.
//

#ifndef __BUFFERCONTROLMESSAGE_M_H
#define __BUFFERCONTROLMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
//using omnetpp::cLikedList;
using omnetpp::cQueue;
#include "EthernetHeaderInformation.h";
typedef std::vector<ethernetHeaderInformation*> typeDefVector;
// }}

/**
 * Enum generated from <tt>src/messagetype/BufferControlMessage.msg:28</tt> by nedtool.
 * <pre>
 * enum messageInformationConstant
 * {
 *     BUFFER_LEVEL_REQUEST = 0;
 *     BUFFER_LEVEL_RESPONSE = 1;
 *     BUFFER_LEVEL_UPDATE = 2;
 *     SEND_AUTHORISATION = 3;
 * }
 * </pre>
 */
enum messageInformationConstant {
    BUFFER_LEVEL_REQUEST = 0,
    BUFFER_LEVEL_RESPONSE = 1,
    BUFFER_LEVEL_UPDATE = 2,
    SEND_AUTHORISATION = 3
};

/**
 * Class generated from <tt>src/messagetype/BufferControlMessage.msg:36</tt> by nedtool.
 * <pre>
 * message BufferControlMessage
 * {
 *     int messageInformation \@enum(messageInformationConstant);
 *     typeDefVector informationPerBuffer;
 *     int authorizedBuffer;
 * }
 * </pre>
 */
class BufferControlMessage : public ::omnetpp::cMessage
{
  protected:
    int messageInformation;
    typeDefVector informationPerBuffer;
    int authorizedBuffer;

  private:
    void copy(const BufferControlMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BufferControlMessage&);

  public:
    BufferControlMessage(const char *name=nullptr, short kind=0);
    BufferControlMessage(const BufferControlMessage& other);
    virtual ~BufferControlMessage();
    BufferControlMessage& operator=(const BufferControlMessage& other);
    virtual BufferControlMessage *dup() const override {return new BufferControlMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getMessageInformation() const;
    virtual void setMessageInformation(int messageInformation);
    virtual typeDefVector& getInformationPerBuffer();
    virtual const typeDefVector& getInformationPerBuffer() const {return const_cast<BufferControlMessage*>(this)->getInformationPerBuffer();}
    virtual void setInformationPerBuffer(const typeDefVector& informationPerBuffer);
    virtual int getAuthorizedBuffer() const;
    virtual void setAuthorizedBuffer(int authorizedBuffer);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BufferControlMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BufferControlMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __BUFFERCONTROLMESSAGE_M_H

