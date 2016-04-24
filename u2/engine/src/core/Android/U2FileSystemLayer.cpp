#include "U2FileSystemLayer.h"


U2EG_NAMESPACE_USING


void FileSystemLayer::getConfigPaths()
{
}
//---------------------------------------------------------------------
void FileSystemLayer::prepareUserHome(const String& subdir)
{
}
//---------------------------------------------------------------------
bool FileSystemLayer::fileExists(const String& path)
{
    return access(path.c_str(), 00) == 0;
}
//---------------------------------------------------------------------
bool FileSystemLayer::createDirectory(const String& path)
{
    return false;
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
