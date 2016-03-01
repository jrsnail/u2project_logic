//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Context__
#define __U2Context__


#include "U2Core.h"


U2EG_NAMESPACE_BEGIN


class Context : public Object
{
protected:
    typedef std::map<u2::String, Context*>      ContextMap;

public:
    Context(const String& type, const String& name);
    virtual ~Context();

    void initialize(const u2::String& facadeName
        , const u2::String& mediatorClass, const u2::String& mediatorName
        , const u2::String& viewCompClass, const u2::String& viewCompName);

    inline u2::String getFacadeName() const { return m_szFacadeName; };
    inline u2::String getMediatorClass() const { return m_szMediatorClass; };
    inline u2::String getMediatorName() const { return m_szMediatorName; };
    inline u2::String getViewCompClass() const { return m_szViewCompClass; };
    inline u2::String getViewCompName() const { return m_szViewCompName; };

    Context* createChild(const String& type, const String& name
        , const u2::String& facadeName
        , const u2::String& mediatorClass, const u2::String& mediatorName
        , const u2::String& viewCompClass, const u2::String& viewCompName);

    void destroyChild(Context* child);

    void addChild(Context* child);

    void removeChild(Context* child);

    inline const Context* getParent() const { return m_pParent; };

    inline Context* getParent() { return m_pParent; };

    inline void setParent(Context* parent);

    typedef ConstMapIterator<ContextMap>        ConstContextMapIterator;
    typedef MapIterator<ContextMap>             ContextMapIterator;

    /** Get an iterator over the children of this Context. */
    ConstContextMapIterator getChildIterator() const;
    ContextMapIterator getChildIterator();


protected:
    u2::String          m_szFacadeName;
    u2::String          m_szMediatorClass;
    u2::String          m_szMediatorName;
    u2::String          m_szViewCompClass;
    u2::String          m_szViewCompName;

    ContextMap          m_children;
    Context*            m_pParent;
};



class ContextManager : public SimpleObjectManager<Context>, public Singleton < ContextManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ContextManager();

public:
    /** Default destructor.
    */
    virtual ~ContextManager();

    Context* createObject(const String& type, const String& name
        , const u2::String& facadeName
        , const u2::String& mediatorClass, const u2::String& mediatorName
        , const u2::String& viewCompClass, const u2::String& viewCompName);

protected:
    virtual Context* createObject(const String& type, const String& name);

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static ContextManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static ContextManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Context__) */
