#ifndef __U2SearchOps_H__
#define __U2SearchOps_H__

// Emulate _findfirst, _findnext on non-Windows platforms

#include "U2Platform.h"


#if U2_PLATFORM != U2_PLATFORM_WIN32 && U2_PLATFORM != U2_PLATFORM_WINRT

#include <unistd.h>

/* Our simplified data entry structure */
struct _finddata_t
{
    char *name;
    int attrib;
    unsigned long size;
};

#define _A_NORMAL 0x00  /* Normalfile-Noread/writerestrictions */
#define _A_RDONLY 0x01  /* Read only file */
#define _A_HIDDEN 0x02  /* Hidden file */
#define _A_SYSTEM 0x04  /* System file */
#define _A_ARCH   0x20  /* Archive file */


#define _A_SUBDIR 0x10  /* Subdirectory */

intptr_t _findfirst(const char *pattern, struct _finddata_t *data);
int _findnext(intptr_t id, struct _finddata_t *data);
int _findclose(intptr_t id);

#endif

#endif
