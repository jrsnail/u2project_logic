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


class Context : public u2::Object, public GeneralAllocatedObject
{
protected:
    typedef std::map<String, Context*>      ContextMap;

public:
    Context(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~Context();

    void initialize(const String& facadeName
        , const String& viewCompClass, const String& viewCompName
        , const String& scriptName);

    inline String getFacadeName() const { return m_szFacadeName; };
    inline String getViewCompClass() const { return m_szViewCompClass; };
    inline String getViewCompName() const { return m_szViewCompName; };
    inline String getScriptName() const { return m_szScriptName; };

    Context* createChild(const String& type, const String& name, const String& guid
        , const String& facadeName
        , const String& viewCompClass, const String& viewCompName 
        , const String& uiName);

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
    String          m_szFacadeName;
    String          m_szViewCompClass;
    String          m_szViewCompName;
    String          m_szScriptName;

    ContextMap           m_children;
    Context*             m_pParent;
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

    Context* createObject(const String& type, const String& name, const String& guid
        , const String& facadeName
        , const String& viewCompClass, const String& viewCompName
        , const String& scriptName);

protected:
    virtual Context* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

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
    static ContextManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Context__) */
