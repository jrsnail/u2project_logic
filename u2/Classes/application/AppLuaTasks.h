#ifndef __AppLuaTasks__
#define __AppLuaTasks__


#include "U2GameBase.h"


U2EG_NAMESPACE_BEGIN


class CreateLuaScriptLuaTask : public LuaTask
{
public:
    CreateLuaScriptLuaTask(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~CreateLuaScriptLuaTask();

    virtual void run() override;
};


class SetViewCompUiNameLuaTask : public LuaTask
{
public:
    SetViewCompUiNameLuaTask(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~SetViewCompUiNameLuaTask();

    virtual void run() override;
};


class SetViewCompEnterActionLuaTask : public LuaTask
{
public:
    SetViewCompEnterActionLuaTask(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~SetViewCompEnterActionLuaTask();

    virtual void run() override;
};


class SetViewCompExitActionLuaTask : public LuaTask
{
public:
    SetViewCompExitActionLuaTask(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~SetViewCompExitActionLuaTask();

    virtual void run() override;
};


class CenterViewCompLuaTask : public LuaTask
{
public:
    CenterViewCompLuaTask(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~CenterViewCompLuaTask();

    virtual void run() override;
};

class PreloadLuaTask : public LuaTask
{
public:
    PreloadLuaTask(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~PreloadLuaTask();

    virtual void run() override;

private:
    void _preload();
};


U2EG_NAMESPACE_END


#endif /* defined(__AppLuaTasks__) */
