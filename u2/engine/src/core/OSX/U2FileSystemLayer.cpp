#include "U2FileSystemLayer.h"

#include "macUtils.h"
#include <pwd.h>


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void FileSystemLayer::getConfigPaths()
{
    mConfigPaths.push_back(macBundlePath() + "/Contents/Resources/");
    mConfigPaths.push_back(macBundlePath() + "/");
}
//---------------------------------------------------------------------
void FileSystemLayer::prepareUserHome(const String& subdir)
{
    struct passwd* pwd = getpwuid(getuid());
    if (pwd)
    {
        mHomePath = pwd->pw_dir;
    }
    else
    {
        // try the $HOME environment variable
        mHomePath = getenv("HOME");
    }

    if (!mHomePath.empty())
    {
        // create an U2 subdir in application support
        mHomePath.append("/Library/Application Support/U2/");
        if (mkdir(mHomePath.c_str(), 0755) != 0 && errno != EEXIST)
        {
            // can't create dir
            mHomePath.clear();
        }
        else
        {
            // now create the given subdir
            mHomePath.append(subdir + '/');
            if (mkdir(mHomePath.c_str(), 0755) != 0 && errno != EEXIST)
            {
                // can't create dir
                mHomePath.clear();
            }
        }
    }

    if (mHomePath.empty())
    {
        // couldn't create dir in home directory, fall back to cwd
        mHomePath = "./";
    }
}
//---------------------------------------------------------------------
bool FileSystemLayer::fileExists(const String& path)
{
    return access(path.c_str(), R_OK) == 0;
}
//---------------------------------------------------------------------
bool FileSystemLayer::createDirectory(const String& path)
{
    return !mkdir(path.c_str(), 0755) || errno == EEXIST;
}
//---------------------------------------------------------------------
bool FileSystemLayer::removeDirectory(const String& path)
{
    return !rmdir(path.c_str()) || errno == ENOENT;
}
//---------------------------------------------------------------------
bool FileSystemLayer::removeFile(const String& path)
{
    return !unlink(path.c_str()) || errno == ENOENT;
}
//---------------------------------------------------------------------
bool FileSystemLayer::renameFile(const String& oldname, const String& newname)
{
    return !rename(oldname.c_str(), newname.c_str());
}