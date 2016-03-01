//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2CodeCvt__
#define __U2CodeCvt__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_BEGIN


class CodeCvt
{
public:
    CodeCvt();
    virtual ~CodeCvt();

    static int utf8_charlen(const char * p);
    static int utf8_charlen2(const char * p);

    static int mbstowcs(wchar_t *wcstr, const char *utf8str, size_t count);
    static int wcstombs(char *utf8str, const wchar_t *wcstr, size_t count);

protected:
    /** Convert a UTF-8 character to a wide char.
    @return the length of the UTF-8 input character in bytes.
    */
    static int _utf8_to_wc4(wchar_t *wchar, const char *utf8char);

    /** Convert a UTF-8 string to a wide char string.
        No more than 'count' wide chars will be written to the output buffer.
    @return the size of the converted string in wide chars, excl null terminator.
    */
    static int _utf8s_to_wcs4(wchar_t *wcstr, const char *utf8str, size_t count);

    /** Convert one wide char to a UTF-8 character.
        No more than 'count' bytes will be written to the output buffer.
    @return the length of the converted UTF-8 character in bytes.
    */
    static int _wc4_to_utf8(char *utf8char, wchar_t wchar, size_t count);

    /** Convert a wide char string to a UTF-8 string.
        No more than 'count' bytes will be written to the output buffer.
    @return the # of bytes written to the output buffer, excl null terminator.
    */
    static int _wcs4_to_utf8s(char *utf8str, const wchar_t *wcstr, size_t count);

};


U2EG_NAMESPACE_END

#endif /* defined(__U2CodeCvt__) */
