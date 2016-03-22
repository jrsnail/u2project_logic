//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Facade.h"

#include "U2Controller.h"
#include "U2Model.h"
#include "U2View.h"


U2EG_NAMESPACE_USING


Facade::Facade(const String& type, const String& name)
    : Object(type, name)
    , _controller(nullptr)
	, _model(nullptr)
	, _view(nullptr)
{
	initializeNotifier(name);
	
    CREATE_FACTORY(Controller);
    CREATE_FACTORY(Model);
    CREATE_FACTORY(View);
}

void Facade::initializeFacade(void)
{
	initializeModel();
	initializeView();
	initializeController();
}

inline void Facade::initializeController(void)
{
	if (_controller != nullptr)
	{
		return;
	}

	initializeController<Controller>(m_szName);
}

void Facade::initializeModel(void)
{
	if (_model != nullptr)
	{
		return;
	}
	
	initializeModel<Model>(m_szName);
}

void Facade::initializeView(void)
{
	if (_view != nullptr)
	{
		return;
	}
	
	initializeView<View>(m_szName);
}

inline void Facade::registerCommand(const String& notification_name, Command* command)
{
	if (_controller == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot register command [%s]. Controller is null.", notification_name.c_str());
	}
	
	_controller->registerCommand(notification_name, command);
}

inline Command const& Facade::retrieveCommand(const String& notification_name) const
{
	if (_controller == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot retrieve command [%s]. Controller is null.", notification_name.c_str());
	}
	
	return _controller->retrieveCommand(notification_name);
}

inline Command& Facade::retrieveCommand(const String& notification_name)
{
	if (_controller == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot retrieve command [%s]. Controller is null.", notification_name.c_str());
	}
	
	return _controller->retrieveCommand(notification_name);
}

inline Command* Facade::removeCommand(const String& notification_name)
{
	if (_controller == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot remove command [%s]. Controller is null.", notification_name.c_str());
	}
	
	return _controller->removeCommand(notification_name);
}

inline bool Facade::hasCommand(const String& notification_name) const
{
	if (_controller == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot find command [%s]. Controller is null.", notification_name.c_str());
	}
	
	return _controller->hasCommand(notification_name);
}

inline void Facade::registerProxy(Proxy* proxy)
{
	if (_model == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot register proxy [%s]. Model is null.", proxy == nullptr ? "nullptr" : proxy->getProxyName().c_str());
	}
	
	_model->registerProxy(proxy);
}

inline Proxy const& Facade::retrieveProxy(const String& proxy_name) const
{
	if (_model == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot retrieve proxy [%s]. Model is null.", proxy_name.c_str());
	}
	
	return _model->retrieveProxy(proxy_name);
}

inline Proxy& Facade::retrieveProxy(const String& proxy_name)
{
	if (_model == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot retrieve proxy [%s]. Model is null.", proxy_name.c_str());
	}
	
	return _model->retrieveProxy(proxy_name);
}

inline Proxy* Facade::removeProxy(const String& proxy_name)
{
	if (_model == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot remove proxy [%s]. Model is null.", proxy_name.c_str());
	}
	
	return _model->removeProxy(proxy_name);
}

inline bool Facade::hasProxy(const String& proxy_name) const
{
	if (_model == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot find proxy [%s]. Model is null.", proxy_name.c_str());
	}
	
	return _model->hasProxy(proxy_name);
}

inline void Facade::registerMediator(Mediator* mediator)
{
	if (_view == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot register mediator [%s]. View is null.",
		//mediator == nullptr ? "nullptr" : mediator->getMediatorName().c_str());
	}
	
	_view->registerMediator(mediator);
}

inline Mediator const& Facade::retrieveMediator(const String& mediator_name) const
{
	if (_view == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot retrieve mediator [%s]. View is null.", mediator_name.c_str());
	}
	
	return _view->retrieveMediator(mediator_name);
}

inline Mediator& Facade::retrieveMediator(const String& mediator_name)
{
	if (_view == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot retrieve mediator [%s]. View is null.", mediator_name.c_str());
	}
	
	return _view->retrieveMediator(mediator_name);
}

inline Mediator* Facade::removeMediator(const String& mediator_name)
{
	if (_view == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot remove mediator [%s]. View is null.", mediator_name.c_str());
	}

	return _view->removeMediator(mediator_name);
}

inline bool Facade::hasMediator(const String& mediator_name) const
{
	if (_view == nullptr)
	{
		assert(0);
		//throwException<std::runtime_error>("Cannot find mediator [%s]. View is null.", mediator_name.c_str());
	}
	
	return _view->hasMediator(mediator_name);
}

inline void Facade::sendNotification(const String& notification_name, const void* data)
{
	Notification notification(notification_name, data);
	notifyObservers(notification);
}

inline void Facade::notifyObservers(const Notification& notification)
{
	if (_view != nullptr)
		_view->notifyObservers(notification);
}

bool Facade::hasCore(const String& name)
{
    return FacadeManager::getSingleton().retrieveObjectByName(name) != nullptr;
}

void Facade::removeCore(const String& name)
{
    Facade* pObj = FacadeManager::getSingleton().retrieveObjectByName(name);
    if (pObj == nullptr)
    {
        return;
    }
    Model::removeModel(name);
    Controller::removeController(name);
    View::removeView(name);
    FacadeManager::getSingleton().destoryObject(pObj);
}

void Facade::broadcastNotification(const Notification& notification)
{
    FacadeManager::ObjectMapIterator mapIterator = FacadeManager::getSingleton().retrieveAllObjects();
    while (mapIterator.hasMoreElements())
    {
        Facade* pFacade = dynamic_cast<Facade*>(mapIterator.getNext());
        pFacade->notifyObservers(notification);
    }
}

void Facade::broadcastNotification(const String& notification_name, const void* data)
{
    Notification notification(notification_name, data);
	broadcastNotification(notification);
}

Facade::~Facade(void)
{
    Model::removeModel(m_szName);
    Controller::removeController(m_szName);
    View::removeView(m_szName);

	_view = nullptr;
	_model = nullptr;
	_controller = nullptr;
	//puremvc_facade_instance_map.remove(_multiton_key);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> FacadeManager* Singleton<FacadeManager>::msSingleton = 0;
FacadeManager* FacadeManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new FacadeManager;
	}
	return msSingleton;
}
FacadeManager& FacadeManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
FacadeManager::FacadeManager()
{
}
//-----------------------------------------------------------------------
FacadeManager::~FacadeManager()
{
}