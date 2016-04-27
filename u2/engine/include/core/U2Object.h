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


class Object
{
public:
    Object(const String& type, const String& name = BLANK);
    virtual ~Object();

    const String& getType() const { return m_szType; };
    const String& getName() const { return m_szName; };
    const String& getGuid() const { return m_szGuid; };

	/** You should not call this in any way.
	*/
	void renameForPooling(const String& name) { m_szName = name; };

protected:
    String                  m_szType;       //< 子类类型
    String                  m_szName;       //< 对象名称
    String                  m_szGuid;       //< 唯一标识对象，每个对象递增
};


class ObjectFactory : public FactoryObj< Object >
{
public:
    virtual ~ObjectFactory() {}

    virtual const String& getType() const override = 0;

    virtual Object* createInstance(const String& name) override = 0;

    virtual void destroyInstance(Object* ptr) override = 0;

};


template <class T>
class TemplateObjectFactory : public ObjectFactory
{
public:
    virtual ~TemplateObjectFactory() {}

    virtual const String& getType() const override;

    virtual Object* createInstance(const String& name) override;

    virtual void destroyInstance(Object* ptr) override;

};


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
const String& TemplateObjectFactory<T>::getType(void) const
{
    static String name = GET_OBJECT_TYPE(T);
    return name;
}
//-----------------------------------------------------------------------
template <class T>
void TemplateObjectFactory<T>::destroyInstance(Object* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}
//-----------------------------------------------------------------------
template <class T>
Object* TemplateObjectFactory<T>::createInstance(const String& name)
{
    return new T(getType(), name);
}


class ReusableObject : public Object
{
public:
    ReusableObject(const String& type);
    virtual ~ReusableObject();

	virtual void preRecycleByPool() { m_bUsing = false; };
    virtual void postReuseFromPool() { m_bUsing = true; };

protected:
	bool m_bUsing;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Object__) */
