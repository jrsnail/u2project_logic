#include "macUtils.h"

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>


U2EG_NAMESPACE_BEGIN


// Basically a dummy function.  Dynamic libraries aren't supported on iOS
void* mac_loadDylib(const char* name)
{
    return NULL;
}

String macTempFileName()
{
    NSString *tempFilePath;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    for (;;) {
        NSString *baseName = [NSString stringWithFormat:@"tmp-%x", arc4random()];
        tempFilePath = [NSTemporaryDirectory() stringByAppendingPathComponent:baseName];
        if (![fileManager fileExistsAtPath:tempFilePath])
            break;
    }
    return String([tempFilePath cStringUsingEncoding:NSASCIIStringEncoding]);
}

String macBundlePath()
{
    char path[PATH_MAX];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);
    
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);
    
    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);
    
    CFStringGetCString(cfStringRef, path, PATH_MAX, kCFStringEncodingASCII);
    
    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);
    
    return String(path);
}

String iOSDocumentsDirectory()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    return String([documentsDirectory cStringUsingEncoding:NSASCIIStringEncoding]);
}

String macCachePath()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDirectory = [paths objectAtIndex:0];

    return [[cachesDirectory stringByAppendingString:@"/"] cStringUsingEncoding:NSASCIIStringEncoding];
}

String macResourcePath()
{
    return macBundlePath();
}


U2EG_NAMESPACE_END
