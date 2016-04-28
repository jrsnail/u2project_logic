#include <CoreFoundation/CoreFoundation.h>
#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_BEGIN


CFBundleRef mac_loadExeBundle(const char *path);
void * mac_getBundleSym(CFBundleRef bundle, const char *name);
bool mac_unloadExeBundle(CFBundleRef bundle);
void* mac_loadDylib(const char* name);
void* mac_loadFramework(String name);
_U2Export String macPluginPath();
_U2Export String macResourcePath();
_U2Export String macBundlePath();
_U2Export String macFrameworksPath();
_U2Export String macCachePath();
_U2Export String macTempFileName(void);


U2EG_NAMESPACE_END
