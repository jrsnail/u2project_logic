#ifndef __AppLuaTasks__
#define __AppLuaTasks__


#include "U2GameBase.h"


U2EG_NAMESPACE_BEGIN


class CreateLuaScriptLuaTask : public LuaTask
{
public:
    CreateLuaScriptLuaTask(const std::string& type, const std::string& name);

    virtual ~CreateLuaScriptLuaTask();

    virtual void run() override;
};


class SetViewCompUiNameLuaTask : public LuaTask
{
public:
    SetViewCompUiNameLuaTask(const std::string& type, const std::string& name);

    virtual ~SetViewCompUiNameLuaTask();

    virtual void run() override;
};


class SetViewCompEnterActionLuaTask : public LuaTask
{
public:
    SetViewCompEnterActionLuaTask(const std::string& type, const std::string& name);

    virtual ~SetViewCompEnterActionLuaTask();

    virtual void run() override;
};


class SetViewCompExitActionLuaTask : public LuaTask
{
public:
    SetViewCompExitActionLuaTask(const std::string& type, const std::string& name);

    virtual ~SetViewCompExitActionLuaTask();

    virtual void run() override;
};


class CenterViewCompLuaTask : public LuaTask
{
public:
    CenterViewCompLuaTask(const std::string& type, const std::string& name);

    virtual ~CenterViewCompLuaTask();

    virtual void run() override;
};

class PreloadLuaTask : public LuaTask
{
public:
    PreloadLuaTask(const std::string& type, const std::string& name);

    virtual ~PreloadLuaTask();

    virtual void run() override;

private:
    void _preload();
};


U2EG_NAMESPACE_END


#endif /* defined(__AppLuaTasks__) */
