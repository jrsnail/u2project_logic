//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2TransStep__
#define __U2TransStep__


#include "U2Core.h"
#include "U2ViewComponent.h"


U2EG_NAMESPACE_BEGIN


class TransStep : public Object
{
public:
    class Key
    {
    public:
        Key();
        Key(ViewComponent* viewComp, ViewComponent::ViewCompState state);

        bool operator== (const Key &that) const;

    protected:
        ViewComponent*      m_pViewComp;
        ViewComponent::ViewCompState              m_state;
    };

public:
    TransStep(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~TransStep();

    void _initialize(const Key& key);

    bool isThisStep(const Key& key);

protected:
    Key         m_Key;
};


class VoidStep : public TransStep
{
public:
    typedef std::function<void()>           CallbackFun;

public:
    VoidStep(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~VoidStep();

    void initialize(const TransStep::Key& key, CallbackFun func);
    void call();

protected:
    void _initialize(const Key& key);

protected:
    CallbackFun     m_func;
};


class ParamStep : public TransStep
{
public:
    typedef std::function<void(void*)>      CallbackFun;

public:
    ParamStep(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~ParamStep();

    void initialize(const TransStep::Key& key, CallbackFun func);
    void call(void* param);

protected:
    void _initialize(const Key& key);

protected:
    CallbackFun     m_func;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2TransStep__) */
