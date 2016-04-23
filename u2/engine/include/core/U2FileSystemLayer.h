#ifndef __U2FileSystemLayer_H__
#define __U2FileSystemLayer_H__

#include "U2Prerequisites.h"
#include "U2StringUtil.h"


U2EG_NAMESPACE_BEGIN


/** Provides methods to find out where the Ogre config files are stored
    and where logs and settings files should be written to.
@remarks
	In modern multi-user OS, a standard user account will often not
	have write access to the path where the SampleBrowser is stored.
	In order to still be able to store graphics settings and log
	output and for the user to overwrite the default Ogre config files,
	this class tries to create a folder inside the user's home directory.
	Specialised implementations for each individual platform must
	subclass this abstract interface.
*/

/** Implementation for the FileSystemLayer interface. */
class _U2Export FileSystemLayer : public GeneralAllocatedObject
{
public:
    /** Creates a concrete platform-dependent implementation of FileSystemLayer.
    @param subdir
		A subdirectory inside the user's path to distinguish between
		different Ogre releases.
    */
    FileSystemLayer(const String& subdir)
    {
        // determine directories to search for config files
        getConfigPaths();
        // prepare write location in user directory
        prepareUserHome(subdir);
    }
        
    /** Search for the given config file in the user's home path. If it can't
        be found there, the function falls back to the system-wide install
        path for Ogre config files. (Usually the same place where the
        SampleBrowser resides, or a special config path above that path.)
    @param filename
		The config file name (without path)
    @return
		The full path to the config file.
    */
    const String getConfigFilePath(String filename) const
    {
#if U2_DEBUG_MODE == 1 && (U2_PLATFORM != U2_PLATFORM_APPLE && U2_PLATFORM != U2_PLATFORM_APPLE_IOS)
        // add U2_BUILD_SUFFIX (default: "_d") to config file names
        String::size_type pos = filename.rfind('.');
        if (pos != String::npos)
            filename = filename.substr(0, pos) + U2_BUILD_SUFFIX + filename.substr(pos);
#endif

        // look for the requested file in several locations:
            
        // 1. in the writable path (so user can provide custom files)
        String path = getWritablePath(filename);
        if (fileExists(path))
            return path;
            
        // 2. in the config file search paths
        for (size_t i = 0; i < mConfigPaths.size(); ++i)
        {
            path = mConfigPaths[i] + filename;
            if (fileExists(path))
                return path;
        }
            
        // 3. fallback to current working dir
        return filename;
    }
        
    /** Find a path where the given filename can be written to. This path 
        will usually be in the user's home directory. This function should
        be used for any output like logs and graphics settings.
        @param filename
        Name of the file.
        @return
        The full path to a writable location for the given filename.
    */
    const String getWritablePath(const String& filename) const
    {
        return mHomePath + filename;
    }
        
    void setConfigPaths(const StringVector &paths){
        mConfigPaths = paths;
    }
        
    void setHomePath(const String &path){
        mHomePath = path;
    }
        
    /** Create a directory. */
    static bool createDirectory(const String& name);
    /** Delete a directory. Should be empty */
    static bool removeDirectory(const String& name);
    /** Test if the given file exists. */
    static bool fileExists(const String& path);
    /** Delete a file. */
    static bool removeFile(const String& path);
    /** Rename a file. */
    static bool renameFile(const String& oldpath, const String& newpath);

private:
	/** Determine config search paths. */
	void getConfigPaths();

	/** Create an Ogre directory and the given subdir in the user's home. */
	void prepareUserHome(const String& subdir);

private:
    StringVector mConfigPaths;
    String mHomePath;
};


U2EG_NAMESPACE_END


#endif
