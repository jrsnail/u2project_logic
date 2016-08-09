//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2ContextQueue__
#define __U2ContextQueue__


#include "U2Core.h"


U2EG_NAMESPACE_BEGIN


class Context;
class ContextProxy;


class ContextQueue : public Object
{
protected:
    typedef std::list<u2::Context*>         Queue;

public:
    enum class eCapacity
    {
        Cap_Infinite = 0,
        Cap_1 = 1,
        Cap_2 = 2,
        Cap_3 = 3,
        Cap_4 = 4,
        Cap_5 = 5,
    };

    enum class eBackKeyPriority
    {
        Pri_0_Shade = 0,
        Pri_10 = 10,
        Pri_20 = 20,
        Pri_30 = 30,
        Pri_40 = 40,
        Pri_50 = 50,
        Pri_60 = 60,
        Pri_70 = 70,
        Pri_80 = 80,
        Pri_90 = 90,
        Pri_100_Dialog = 100,
        Pri_110 = 110,
        Pri_120 = 120,
        Pri_130 = 130,
        Pri_140 = 140,
        Pri_150 = 150,
        Pri_160 = 160,
        Pri_170 = 170,
        Pri_180 = 180,
        Pri_190 = 190,
        Pri_200_Scene = 200,
        Pri_210 = 210,
    };

    enum class eTransType
    {
        TT_None,
        TT_Overlay,
        TT_OneByOne,
        TT_Cross,
        TT_Overlay_Child,
        TT_OneByOne_Child,
        TT_Cross_Child
    };

public:
    ContextQueue(const String& type, const String& name, const String& guid = BLANK);
    virtual ~ContextQueue();

    inline ContextQueue::eBackKeyPriority getPriority() const { return m_ePriority; };
    u2::Context* top();

    virtual void initialize(eTransType defaultTransType, eBackKeyPriority priority, ContextProxy* contextProxy) = 0;
    virtual void pushBack(u2::Context* context, eTransType transType = eTransType::TT_None) = 0;
    virtual void pushFront(u2::Context* context, eTransType transType = eTransType::TT_None) = 0;
    virtual void pop(eTransType transType = eTransType::TT_None) = 0;
    virtual void replace(u2::Context* from, u2::Context* to, eTransType transType = eTransType::TT_None) = 0;

    bool hasContext(const String& name);
    bool hasContext(const u2::Context* context);
    void erase(const String& name);
    void erase(const u2::Context* context);

protected:
    void _switch(u2::Context* from, eTransType transType, u2::Context* to);

protected:
    eTransType          m_eDefaultTransType;
    eCapacity           m_eCapacity;
    eBackKeyPriority    m_ePriority;
    Queue               m_queue;
    ContextProxy*       m_pContextProxy;
};


class SingleContextQueue : public ContextQueue
{
public:
    SingleContextQueue(const String& type, const String& name, const String& guid = BLANK);
    virtual ~SingleContextQueue();

    virtual void initialize(eTransType defaultTransType, eBackKeyPriority priority, ContextProxy* contextProxy) override;
    virtual void pushBack(u2::Context* context, eTransType transType = eTransType::TT_None) override;
    virtual void pushFront(u2::Context* context, eTransType transType = eTransType::TT_None) override;
    virtual void pop(eTransType transType = eTransType::TT_None) override;
    virtual void replace(u2::Context* from, u2::Context* to, eTransType transType = eTransType::TT_None) override;

};


class InfiniteContextQueue : public ContextQueue
{
public:
    InfiniteContextQueue(const String& type, const String& name, const String& guid = BLANK);
    virtual ~InfiniteContextQueue();

    virtual void initialize(eTransType defaultTransType, eBackKeyPriority priority, ContextProxy* contextProxy) override;
    virtual void pushBack(u2::Context* context, eTransType transType = eTransType::TT_None) override;
    virtual void pushFront(u2::Context* context, eTransType transType = eTransType::TT_None) override;
    virtual void pop(eTransType transType = eTransType::TT_None) override;
    virtual void replace(u2::Context* from, u2::Context* to, eTransType transType = eTransType::TT_None) override;

};


U2EG_NAMESPACE_END

#endif /* defined(__U2ContextQueue__) */
