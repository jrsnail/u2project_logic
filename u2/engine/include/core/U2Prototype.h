#ifndef __U2Prototype__
#define __U2Prototype__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN



template <typename T>
class Prototype : public Object
{
public:
    Prototype(const String& type, const String& name)
        : Object(type, name)
        , m_pPrototype(nullptr)
    {

    }

    virtual ~Prototype() {}

    void addInstance(T* instance)
    {
        assert(instance != nullptr);
        if (isPrototype())
        {
            InstanceMap::iterator it = m_InstanceMap.find(instance->getGuid());
            if (it == m_InstanceMap.end())
            {
                m_InstanceMap.insert(make_pair(instance->getGuid(), instance));
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            m_pPrototype->addInstance(instance);
        }
    }

    void removeInstance(T* instance)
    {
        assert(instance != nullptr);
        if (isPrototype())
        {
            InstanceMap::iterator it = m_InstanceMap.find(instance->getGuid());
            if (it == m_InstanceMap.end())
            {
                assert(0);
            }
            else
            {
                m_InstanceMap.erase(it);
            }
        }
        else
        {
            m_pPrototype->removeInstance(instance);
        }
    }

    void resetAllInstances()
    {
        if (isPrototype())
        {
            for (InstanceMap::iterator it = m_InstanceMap.begin();
            it != m_InstanceMap.end(); it++)
            {
                it->second->resetFromPrototype();
            }
        }
        else
        {
            m_pPrototype->resetAllInstances();
        }
    }

    virtual bool isPrototype()
    {
        return m_pPrototype == nullptr;
    }

    virtual T* retrievePrototype()
    {
        return (m_pPrototype == nullptr) ? dynamic_cast<T*>(this) : m_pPrototype;
    }

    virtual T* cloneFromPrototype(const String& name = BLANK) = 0;
    virtual T* cloneFromInstance(const String& name = BLANK) = 0;
    virtual void resetFromPrototype() = 0;
    virtual void applyToPrototype() = 0;


protected:
    // <guid, Instance*>
    typedef std::map<String, T*> InstanceMap;
    InstanceMap m_InstanceMap;

    T*   m_pPrototype;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Prototype__) */
