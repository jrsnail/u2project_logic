#ifndef __U2PredefinedSystems__
#define __U2PredefinedSystems__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2System.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class GameObject;


class RenderSystem : public System
{
public:
    RenderSystem(const String& type, const String& name);
    virtual ~RenderSystem();

    virtual void enter() override;
    virtual void exit() override;
    virtual void execute(u2real dt) override;
    virtual void pause() override;
    virtual void resume() override;

protected:
    void _enter();
    void _exit();
    void _execute(GameObject* gameObj, u2real dt);
    void _pause();
    void _resume();
};


class InputSystem : public System
{
public:
    InputSystem(const String& type, const String& name);
    virtual ~InputSystem();

    virtual void enter() override;
    virtual void exit() override;
    virtual void execute(u2real dt) override;
    virtual void pause() override;
    virtual void resume() override;

protected:
    void _enter();
    void _exit();
    void _execute(GameObject* gameObj, u2real dt);
    void _pause();
    void _resume();
};


class MoveSystem : public System
{
public:
    MoveSystem(const String& type, const String& name);
    virtual ~MoveSystem();

    virtual void enter() override;
    virtual void exit() override;
    virtual void execute(u2real dt) override;
    virtual void pause() override;
    virtual void resume() override;

protected:
    void _enter();
    void _exit();
    void _execute(GameObject* gameObj, u2real dt);
    void _pause();
    void _resume();
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif /* defined(__U2PredefinedSystems__) */
