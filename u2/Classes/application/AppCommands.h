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
    StartupCommand(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~StartupCommand();

    virtual void go(const Notification& notification) override;
};


class Trans2ShadeCommand : public SimpleCommand
{
public:
    Trans2ShadeCommand(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~Trans2ShadeCommand();

    virtual void go(const Notification& notification) override;
};


class StartupLuaCommand : public SimpleCommand
{
public:
    StartupLuaCommand(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~StartupLuaCommand();

    virtual void go(const Notification& notification) override;
};


/*
class HotUpdateCommand : public SimpleCommand
{
public:
    HotUpdateCommand(const String& type, const String& name);
    virtual ~HotUpdateCommand();

    virtual void go(const Notification& notification) override;

private:
    void _parseServerJson(const String& json);
    void _parseLocalJson();
    void _calUpdateList();
    void _startDownload();

private:
    struct StUpdatingFile
    {
        String szUrl;
        String szFolder;
        String szTmpFilename;
        bool bCompleted;

        StUpdatingFile::StUpdatingFile() 
            : bCompleted(false)
        {

        }
    };

    /// <key, StUpdatingFile>
    typedef std::map<String, StUpdatingFile>               FileList;

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
