#include <CoreFoundation/CoreFoundation.h>
#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_BEGIN


void* mac_loadDylib(const char* name);
_U2Export String macCachePath();
_U2Export String macBundlePath(void);
_U2Export String macTempFileName(void);
_U2Export String iOSDocumentsDirectory(void);
_U2Export String macResourcePath();


U2EG_NAMESPACE_END
