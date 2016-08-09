#ifndef __PredefinedSystems__
#define __PredefinedSystems__


#include "U2Core.h"


U2EG_NAMESPACE_USING



class RenderSystem : public System
{
public:
    RenderSystem(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
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
    InputSystem(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
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
    MoveSystem(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
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



#endif /* defined(__PredefinedSystems__) */
