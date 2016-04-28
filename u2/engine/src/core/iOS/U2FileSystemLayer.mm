#include "U2FileSystemLayer.h"

#include "iosUtils.h"
#import <Foundation/NSPathUtilities.h>
#import <Foundation/NSFileManager.h>


U2EG_NAMESPACE_USING


void FileSystemLayer::getConfigPaths()
{
    mConfigPaths.push_back(iOSDocumentsDirectory() + "/");
	mConfigPaths.push_back(macBundlePath() + "/");
}
//---------------------------------------------------------------------
void FileSystemLayer::prepareUserHome(const String& subdir)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);

    mHomePath = String([[paths objectAtIndex:0] cStringUsingEncoding:NSASCIIStringEncoding]) + "/";
}
//---------------------------------------------------------------------
bool FileSystemLayer::fileExists(const String& path)
{
	return access(path.c_str(), R_OK) == 0;
}
//---------------------------------------------------------------------
bool FileSystemLayer::createDirectory(const String& path)
{
    NSString *directoryPath = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
    NSError *error;

    if (![[NSFileManager defaultManager] createDirectoryAtPath:directoryPath
                                    withIntermediateDirectories:NO
                                                    attributes:nil
                                                            error:&error])
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------
bool FileSystemLayer::removeDirectory(const String& path)
{
    return false;
}
//---------------------------------------------------------------------
bool FileSystemLayer::removeFile(const String& path)
{
    return false;
}
//---------------------------------------------------------------------
bool FileSystemLayer::renameFile(const String& oldname, const String& newname)
{
    return false;
}