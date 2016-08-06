#ifndef __RecvSocketTasks_H__
#define __RecvSocketTasks_H__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameWsCloseRST : public WsCloseRST
{
public:
    GameWsCloseRST(const u2::String& type, const u2::String& name);
    virtual ~GameWsCloseRST();

    virtual void run() override;
};


class GameWsErrorRST : public WsErrorRST
{
public:
    GameWsErrorRST(const u2::String& type, const u2::String& name);
    virtual ~GameWsErrorRST();

    virtual void run() override;
};


class GameWsOpenRST : public WsOpenRST
{
public:
    GameWsOpenRST(const u2::String& type, const u2::String& name);
    virtual ~GameWsOpenRST();

    virtual void run() override;
};


class GameWsHeartBeatSST : public WsHeartBeatSST
{
public:
    GameWsHeartBeatSST(const u2::String& type, const u2::String& name);
    virtual ~GameWsHeartBeatSST();

    virtual void run() override;
};


class SnapshotRST : public RecvSocketTask
{
public:
    SnapshotRST(const String& type, const String& name);

    virtual ~SnapshotRST();

};


#endif /* defined(__RecvSocketTasks_H__) */
