//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Object__
#define __U2Object__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2FactoryObj.h"


U2EG_NAMESPACE_BEGIN


#define GET_OBJECT_TYPE(T)          typeid(T).name()


class _U2Export Object
{
public:
    Object(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~Object();

    const String& getType() const;
    const String& getName() const;
    const String& getGuid() const;

    virtual void serialize();
    virtual void deserialize();
    bool isDeserializeSucceed();
    bool isSerializeSucceed();
    void setData(const vector<u2char>::type& data);
    const vector<u2char>::type& getData() const;
    vector<u2char>::type& getData();
    size_t getDataSize();

protected:
    String                  m_szType;       //< 子类类型
    String                  m_szName;       //< 对象名称
    String                  m_szGuid;       //< 唯一标识对象，每个对象递增
    vector<u2char>::type    m_Data;
    bool                    m_bDeserializeSucceed;
    bool                    m_bSerializeSucceed;
};


class _U2Export ObjectFactory : public FactoryObj< Object >, public GeneralAllocatedObject
{
public:
    virtual ~ObjectFactory() {}

    virtual const String& getType() const override = 0;

    virtual Object* createInstance(const String& name, const String& guid) override = 0;

    virtual void destroyInstance(Object* ptr) override = 0;

};


template <class T>
class _U2Export TemplateObjectFactory : public ObjectFactory
{
public:
    TemplateObjectFactory(const String& type = BLANK);
    virtual ~TemplateObjectFactory() {}

    virtual const String& getType() const override;

    virtual Object* createInstance(const String& name, const String& guid) override;

    virtual void destroyInstance(Object* ptr) override;

protected:
    String     m_szType;
};


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
const String& TemplateObjectFactory<T>::getType(void) const
{
    //static String name = GET_OBJECT_TYPE(T);
    return m_szType;
}
//-----------------------------------------------------------------------
template <class T>
TemplateObjectFactory<T>::TemplateObjectFactory(const String& type)
{
    if (type == BLANK)
    {
        m_szType = GET_OBJECT_TYPE(T);
    }
    else
    {
        m_szType = type;
    }
}
//-----------------------------------------------------------------------
template <class T>
void TemplateObjectFactory<T>::destroyInstance(Object* obj)
{
    if (obj != nullptr)
    {
        U2_DELETE obj;
    }
}
//-----------------------------------------------------------------------
template <class T>
Object* TemplateObjectFactory<T>::createInstance(const String& name, const String& guid)
{
    T* pObj = U2_NEW T(getType(), name, guid);
    return pObj;
}


class _U2Export ReusableObject : public Object
{
public:
    ReusableObject(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~ReusableObject();

    /** You should not call this in any way.
    */
    void renameAsName(const String& name);

    /** You should not call this in any way.
    */
    void renameAsGuid(const String& guid);

	virtual void preRecycleByPool() { m_bUsing = false; };
    virtual void postReuseFromPool() { m_bUsing = true; };

protected:
	bool m_bUsing;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Object__) */
