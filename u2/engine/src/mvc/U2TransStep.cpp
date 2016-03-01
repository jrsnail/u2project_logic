//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//


#include "U2TransStep.h"

#include "U2ViewComponent.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TransStep::Key::Key() 
: m_pViewComp(nullptr)
, m_state(ViewComponent::ViewCompState::VCS_Unloaded)
{

}
TransStep::Key::Key(ViewComponent* viewComp, ViewComponent::ViewCompState state)
: m_pViewComp(viewComp)
, m_state(state)
{

}
//-----------------------------------------------------------------------
bool TransStep::Key::operator== (const Key &that) const
{
    return m_pViewComp == that.m_pViewComp && m_state == that.m_state;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TransStep::TransStep(const String& type, const String& name) 
: Object(type, name)
{

}
//-----------------------------------------------------------------------
TransStep::~TransStep()
{

}
//-----------------------------------------------------------------------
void TransStep::_initialize(const Key& key)
{
    m_Key = key;
}
//-----------------------------------------------------------------------
bool TransStep::isThisStep(const Key& key)
{
    return m_Key == key;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
VoidStep::VoidStep(const String& type, const String& name)
    : TransStep(type, name)
{

}
//-----------------------------------------------------------------------
VoidStep::~VoidStep()
{

}
//-----------------------------------------------------------------------
void VoidStep::_initialize(const Key& key)
{
    TransStep::_initialize(key);
}
//-----------------------------------------------------------------------
void VoidStep::initialize(const TransStep::Key& key, CallbackFun func)
{
    _initialize(key);
    m_func = func;
}
//-----------------------------------------------------------------------
void VoidStep::call()
{
    if (m_func != nullptr)
    {
        m_func();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ParamStep::ParamStep(const String& type, const String& name)
    : TransStep(type, name)
{

}
//-----------------------------------------------------------------------
ParamStep::~ParamStep()
{

}
//-----------------------------------------------------------------------
void ParamStep::_initialize(const Key& key)
{
    TransStep::_initialize(key);
}
//-----------------------------------------------------------------------
void ParamStep::initialize(const TransStep::Key& key, CallbackFun func)
{
    _initialize(key);
    m_func = func;
}
//-----------------------------------------------------------------------
void ParamStep::call(void* param)
{
    if (m_func != nullptr)
    {
        m_func(param);
    }
}