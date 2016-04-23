#include "U2FileSystemLayer.h"

#define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#if U2_PLATFORM == U2_PLATFORM_WIN32
#  include <shlobj.h>
#endif
#include <io.h>
#include <direct.h>
#include <errno.h>


U2EG_NAMESPACE_USING


bool widePathToOgreString(String& dest, const WCHAR* wpath)
{
    // need to convert to narrow (OEM or ANSI) codepage so that fstream can use it 
    // properly on international systems.
    char npath[MAX_PATH];
#if U2_PLATFORM == U2_PLATFORM_WIN32
    // Note, that on legacy CRT versions codepage for narrow CRT file functions can be changed using 
    // SetFileApisANSI/OEM, but on modern runtimes narrow pathes are always widened using ANSI codepage.
    // We suppose that on such runtimes file APIs codepage is left in default, ANSI state.
    UINT codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
#elif U2_PLATFORM == U2_PLATFORM_WINRT
    // Runtime is modern, narrow calls are widened inside CRT using CP_ACP codepage.
    UINT codepage = CP_ACP;
#endif
    if (0 == WideCharToMultiByte(codepage, 0 /* Use default flags */, wpath, -1, npath, sizeof(npath), NULL, NULL))
    {
        dest.clear();
        return false;
    }

    // success
    dest = npath;
    return true;
}

void FileSystemLayer::getConfigPaths()
{
#if U2_PLATFORM == U2_PLATFORM_WIN32
    // try to determine the application's path
    DWORD bufsize = 256;
    char* resolved = 0;
    do
    {
        char* buf = U2_ALLOC_T(char, bufsize, MEMCATEGORY_GENERAL);
        DWORD retval = GetModuleFileNameA(NULL, buf, bufsize);
        if (retval == 0)
        {
            // failed
            U2_FREE(buf, MEMCATEGORY_GENERAL);
            break;
        }

        if (retval < bufsize)
        {
            // operation was successful.
            resolved = buf;
        }
        else
        {
            // buffer was too small, grow buffer and try again
            U2_FREE(buf, MEMCATEGORY_GENERAL);
            bufsize <<= 1;
        }
    } while (!resolved);

    String appPath = resolved;
    if (resolved)
        U2_FREE(resolved, MEMCATEGORY_GENERAL);
    if (!appPath.empty())
    {
        // need to strip the application filename from the path
        String::size_type pos = appPath.rfind('\\');
        if (pos != String::npos)
            appPath.erase(pos);
    }
    else
    {
        // fall back to current working dir
        appPath = ".";
    }

#elif U2_PLATFORM == U2_PLATFORM_WINRT
    String appPath;
    if(!widePathToOgreString(appPath, Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data()))
    {
        // fallback to current working dir
        appPath = ".";
    }
#endif

    // use application path as config search path
    mConfigPaths.push_back(appPath + '\\');
}
//---------------------------------------------------------------------
void FileSystemLayer::prepareUserHome(const String& subdir)
{
    // fill mHomePath
#if U2_PLATFORM == U2_PLATFORM_WIN32
    WCHAR wpath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, wpath)))
        widePathToOgreString(mHomePath, wpath);
#elif U2_PLATFORM == U2_PLATFORM_WINRT
    widePathToOgreString(mHomePath, Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data());
#endif

    if(!mHomePath.empty())
    {
        // create U2 subdir
        mHomePath += "\\U2\\";
        if (!createDirectory(mHomePath))
        {
            // couldn't create directory, fall back to current working dir
            mHomePath.clear();
        }
        else
        {
            mHomePath += subdir + '\\';
            // create release subdir
            if (!createDirectory(mHomePath))
            {
                // couldn't create directory, fall back to current working dir
                mHomePath.clear();
            }
        }
    }
}
//---------------------------------------------------------------------
bool FileSystemLayer::fileExists(const String& path)
{
    return _access(path.c_str(), 04) == 0; // Use CRT API rather than GetFileAttributesExA to pass Windows Store validation
}
//---------------------------------------------------------------------
bool FileSystemLayer::createDirectory(const String& path)
{
    return !_mkdir(path.c_str()) || errno == EEXIST; // Use CRT API rather than CreateDirectoryA to pass Windows Store validation
}
//---------------------------------------------------------------------
bool FileSystemLayer::removeDirectory(const String& path)
{
    return !_rmdir(path.c_str()) || errno == ENOENT; // Use CRT API to pass Windows Store validation
}
//---------------------------------------------------------------------
bool FileSystemLayer::removeFile(const String& path)
{
    return !_unlink(path.c_str()) || errno == ENOENT; // Use CRT API to pass Windows Store validation
}
//---------------------------------------------------------------------
bool FileSystemLayer::renameFile(const String& oldname, const String& newname)
{
    if(fileExists(oldname) && fileExists(newname))
        removeFile(newname);
    return !rename(oldname.c_str(), newname.c_str()); // Use CRT API to pass Windows Store validation
}