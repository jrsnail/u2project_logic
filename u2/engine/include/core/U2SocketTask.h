#ifndef __U2SocketTask_H__
#define __U2SocketTask_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"
#include "U2Task.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class SendSocketTask : public Task
{
public:
    SendSocketTask(const String& type, const String& name);
    virtual ~SendSocketTask();

    virtual void run() override;
    void setBinary(bool b) { m_bIsBinary = b; }
    bool isBinary() const { return m_bIsBinary; };
    const vector<u2char>::type* const getData() { return &m_Data; };
    size_t getDataSize() { return m_Data.size(); };
    size_t getIssued() { return m_uIssued; }
    void addIssued(size_t delta) { m_uIssued += delta; }

protected:
    bool m_bIsBinary;
    vector<u2char>::type	m_Data;
    size_t m_uIssued;
};


class RecvSocketTask : public Task
{
public:
    RecvSocketTask(const String& type, const String& name);
    virtual ~RecvSocketTask();

    virtual void run() override;

    void setBinary(bool b);
    bool isBinary() const;

protected:
    bool m_bIsBinary;
};





#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
