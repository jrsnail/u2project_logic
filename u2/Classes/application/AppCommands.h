//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __AppCommands__
#define __AppCommands__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING


class Context;


class StartupCommand : public SimpleCommand
{
public:
    StartupCommand(const std::string& type, const std::string& name);
    virtual ~StartupCommand();

    virtual void go(const Notification& notification) override;
};


class Trans2ShadeCommand : public SimpleCommand
{
public:
    Trans2ShadeCommand(const std::string& type, const std::string& name);
    virtual ~Trans2ShadeCommand();

    virtual void go(const Notification& notification) override;
};


class StartupLuaCommand : public SimpleCommand
{
public:
    StartupLuaCommand(const std::string& type, const std::string& name);
    virtual ~StartupLuaCommand();

    virtual void go(const Notification& notification) override;
};


/*
class HotUpdateCommand : public SimpleCommand
{
public:
    HotUpdateCommand(const std::string& type, const std::string& name);
    virtual ~HotUpdateCommand();

    virtual void go(const Notification& notification) override;

private:
    void _parseServerJson(const std::string& json);
    void _parseLocalJson();
    void _calUpdateList();
    void _startDownload();

private:
    struct StUpdatingFile
    {
        std::string szUrl;
        std::string szFolder;
        std::string szTmpFilename;
        bool bCompleted;

        StUpdatingFile::StUpdatingFile() 
            : bCompleted(false)
        {

        }
    };

    /// <key, StUpdatingFile>
    typedef std::map<std::string, StUpdatingFile>               FileList;

    struct StUpdatingCollection
    {
        Version version;
        FileList updatingList;

        StUpdatingCollection::StUpdatingCollection() 
            : version(0)
        {

        }
    };

private:
    typedef std::map<Version, FileList, std::less<Version> >    VersionMap;
    VersionMap      m_VersionMap;
    StUpdatingCollection    m_stUpdatingCollection;
};
*/


#endif /* defined(__ApplicationCommands__) */
