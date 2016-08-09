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
    Prototype(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : Object(type, name, guid)
        , m_pPrototype(nullptr)
    {

    }

    virtual ~Prototype() {}

    void addInstance(T* instance)
    {
        assert(instance != nullptr);
        assert(this->getType() == instance->getType());
        assert(!instance->hasPrototype());
        if (hasPrototype())
        {
            if (isPrototype())
            {
                InstanceMap::iterator it = m_InstanceMap.find(instance->getGuid());
                if (it == m_InstanceMap.end())
                {
                    m_InstanceMap.insert(make_pair(instance->getGuid(), instance));
                    instance->setPrototype(dynamic_cast<T*>(this));
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
        else
        {
            assert(0);
        }
    }

    void removeInstance(T* instance)
    {
        assert(instance != nullptr);
        assert(this->getType() == instance->getType());
        assert(!instance->hasPrototype());
        if (hasPrototype())
        {
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
                    instance->clearPrototype();
                }
            }
            else
            {
                m_pPrototype->removeInstance(instance);
            }
        }
        else
        {
            assert(0);
        }
    }

    void resetAllInstances()
    {
        if (hasPrototype())
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
        else
        {
            assert(0);
        }
    }

    void setPrototype(T* prototype)
    {
        assert(prototype != nullptr);
        assert(this->getType() == prototype->getType());
        assert(this == prototype || prototype->isPrototype());
        if (hasPrototype())
        {
            assert(0);
        }
        else
        {
            m_pPrototype = prototype;
        }
    }

    void clearPrototype()
    {
        m_pPrototype = nullptr;
    }

    bool hasPrototype() const
    {
        return m_pPrototype != nullptr;
    }

    bool isPrototype() const
    {
        return m_pPrototype == this;
    }

    T* retrievePrototype()
    {
        return hasPrototype() ? m_pPrototype : nullptr;
    }

    virtual T* cloneFromPrototype(const String& name = BLANK, const String& guid = BLANK) = 0;
    virtual T* cloneFromInstance(const String& name = BLANK, const String& guid = BLANK) = 0;
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
