//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2CodeCvt.h"



U2EG_NAMESPACE_USING


/*-----------------------------------------------------------------------------
UTF-8 Format Summary

ASCII chars                                                                7 bits
0xxxxxxx

2-character UTF-8 sequence:        11 bits
110xxxxx  10xxxxxx

3-character UTF-8                  16 bits
1110xxxx  10xxxxxx  10xxxxxx

4-char UTF-8                       21 bits
11110xxx  10xxxxxx  10xxxxxx  10xxxxxx

5-char UTF-8                       26 bits
111110xx  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx

6-char UTF-8                       31 bits
1111110x  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx

Unicode address space   (0 - 0x10FFFF)    21 bits
ISO-10646 address space (0 - 0x7FFFFFFF)  31 bits

Note: This code does not prevent UTF-8 sequences which are longer than
necessary from being decoded.
*/


/*
* Returns length indicated by first byte.
*/
static const char utf8_lentab[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0
};

/* mask of required bits in second octet */
#undef c
#define c const char
static c utf8_mintab[] = {
    (c)0x20, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80,
    (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80,
    (c)0x30, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80, (c)0x80,
    (c)0x38, (c)0x80, (c)0x80, (c)0x80, (c)0x3c, (c)0x80, (c)0x00, (c)0x00
};
#undef c

static unsigned char mask[] = { 0, 0x7f, 0x1f, 0x0f, 0x07, 0x03, 0x01 };


#define U2_UTF8_ISASCII( p) ( !(*( const unsigned char *)( p) & 0x80 ) )
#define U2_UTF8_CHARLEN( p) ( U2_UTF8_ISASCII( p) \
            ? 1 : utf8_lentab[*( const unsigned char *)( p) ^ 0x80] )

/* This is like CHARLEN but additionally validates to make sure
* the char used the shortest possible encoding.
* 'l' is used to temporarily hold the result of CHARLEN.
*/
#define U2_UTF8_CHARLEN2( p, l) ( ( ( l = U2_UTF8_CHARLEN( p )) < 3 || \
            ( utf8_mintab[*( const unsigned char *)( p) & 0x1f] & ( p)[1] ) ) ? \
                                     l : 0 )

#define U2_MAX_UTF8_LEN   ( sizeof( wchar_t) * 3/2 )


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CodeCvt::CodeCvt()
{
}
//-----------------------------------------------------------------------
CodeCvt::~CodeCvt()
{
}
//-----------------------------------------------------------------------
int CodeCvt::utf8_charlen(const char * p)
{
    if (!(*p & 0x80))
        return 1;

    return utf8_lentab[*(const unsigned char *)p ^ 0x80];
}
//-----------------------------------------------------------------------
int CodeCvt::utf8_charlen2(const char * p)
{
    int i = U2_UTF8_CHARLEN(p);

    if (i > 2) {
        if (!(utf8_mintab[*p & 0x1f] & p[1]))
            i = 0;
    }
    return i;
}
//-----------------------------------------------------------------------
int CodeCvt::_utf8_to_wc4(wchar_t *wchar, const char *utf8char)
{
    int utflen, i;
    wchar_t ch;

    if (utf8char == NULL) return -1;

    /* Get UTF-8 sequence length from 1st byte */
    utflen = U2_UTF8_CHARLEN2(utf8char, utflen);

    if (utflen == 0 || utflen > (int)U2_MAX_UTF8_LEN) return -1;

    /* First byte minus length tag */
    ch = (wchar_t)(utf8char[0] & mask[utflen]);

    for (i = 1; i < utflen; i++) {
        /* Subsequent bytes must start with 10 */
        if ((utf8char[i] & 0xc0) != 0x80) return -1;

        ch <<= 6;                                     /* 6 bits of data in each subsequent byte */
        ch |= (wchar_t)(utf8char[i] & 0x3f);
    }

    if (wchar) *wchar = ch;

    return utflen;
}
//-----------------------------------------------------------------------
int CodeCvt::_utf8s_to_wcs4(wchar_t *wcstr, const char *utf8str, size_t count)
{
    size_t wclen = 0;
    int utflen, i;
    wchar_t ch;


    /* If input ptr is NULL or empty... */
    if (utf8str == NULL || !*utf8str) {
        if (wcstr)
            * wcstr = 0;
        return 0;
    }

    /* Examine next UTF-8 character.  If output buffer is NULL, ignore count */
    while (*utf8str && (wcstr == NULL || wclen<count)) {
        /* Get UTF-8 sequence length from 1st byte */
        utflen = U2_UTF8_CHARLEN2(utf8str, utflen);

        if (utflen == 0 || utflen >(int)U2_MAX_UTF8_LEN) return -1;

        /* First byte minus length tag */
        ch = (wchar_t)(utf8str[0] & mask[utflen]);

        for (i = 1; i < utflen; i++) {
            /* Subsequent bytes must start with 10 */
            if ((utf8str[i] & 0xc0) != 0x80) return -1;

            ch <<= 6;                                 /* 6 bits of data in each subsequent byte */
            ch |= (wchar_t)(utf8str[i] & 0x3f);
        }

        if (wcstr) wcstr[wclen] = ch;

        utf8str += utflen; /* Move to next UTF-8 character */
        wclen++;                          /* Count number of wide chars stored/required */
    }

    /* Add null terminator if there's room in the buffer. */
    if (wcstr && wclen < count) wcstr[wclen] = 0;

    return wclen;
}
//-----------------------------------------------------------------------
int CodeCvt::_wc4_to_utf8(char *utf8char, wchar_t wchar, size_t count)
{
    int len = 0;

    if (utf8char == NULL)   /* Just determine the required UTF-8 char length. */
    {                                                                    /* Ignore count */
        if (wchar < 0)
            return -1;
        if (wchar < 0x80)
            return 1;
        if (wchar < 0x800)
            return 2;
        if (wchar < 0x10000)
            return 3;
        if (wchar < 0x200000)
            return 4;
        if (wchar < 0x4000000)
            return 5;
        if (wchar < 0x80000000)
            return 6;
        return -1;
    }


    if (wchar < 0) {                                             /* Invalid wide character */
        len = -1;

    }
    else if (wchar < 0x80) {
        if (count >= 1) {
            utf8char[len++] = (char)wchar;
        }

    }
    else if (wchar < 0x800) {
        if (count >= 2) {
            utf8char[len++] = 0xc0 | (wchar >> 6);
            utf8char[len++] = 0x80 | (wchar & 0x3f);
        }

    }
    else if (wchar < 0x10000) {
        if (count >= 3) {
            utf8char[len++] = 0xe0 | (wchar >> 12);
            utf8char[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8char[len++] = 0x80 | (wchar & 0x3f);
        }

    }
    else if (wchar < 0x200000) {
        if (count >= 4) {
            utf8char[len++] = 0xf0 | (wchar >> 18);
            utf8char[len++] = 0x80 | ((wchar >> 12) & 0x3f);
            utf8char[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8char[len++] = 0x80 | (wchar & 0x3f);
        }

    }
    else if (wchar < 0x4000000) {
        if (count >= 5) {
            utf8char[len++] = 0xf8 | (wchar >> 24);
            utf8char[len++] = 0x80 | ((wchar >> 18) & 0x3f);
            utf8char[len++] = 0x80 | ((wchar >> 12) & 0x3f);
            utf8char[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8char[len++] = 0x80 | (wchar & 0x3f);
        }

    }
    else if (wchar < 0x80000000) {
        if (count >= 6) {
            utf8char[len++] = 0xfc | (wchar >> 30);
            utf8char[len++] = 0x80 | ((wchar >> 24) & 0x3f);
            utf8char[len++] = 0x80 | ((wchar >> 18) & 0x3f);
            utf8char[len++] = 0x80 | ((wchar >> 12) & 0x3f);
            utf8char[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8char[len++] = 0x80 | (wchar & 0x3f);
        }

    }
    else
        len = -1;

    return len;

}
//-----------------------------------------------------------------------
int CodeCvt::_wcs4_to_utf8s(char *utf8str, const wchar_t *wcstr, size_t count)
{
    int len = 0;
    int n;
    char *p = utf8str;
    wchar_t empty = 0;                         /* To avoid use of L"" construct */

    if (wcstr == NULL)              /* Treat input ptr NULL as an empty string */
        wcstr = &empty;

    if (utf8str == NULL)             /* Just compute size of output, excl null */
    {
        while (*wcstr)
        {
            /* Get UTF-8 size of next wide char */
            n = _wc4_to_utf8(NULL, *wcstr++, U2_MAX_UTF8_LEN);
            if (n == -1)
                return -1;
            len += n;
        }

        return len;
    }


    /* Do the actual conversion. */

    n = 1;                                                            /* In case of empty wcstr */
    while (*wcstr)
    {
        n = _wc4_to_utf8(p, *wcstr++, count);

        if (n <= 0)                         /* If encoding error (-1) or won't fit (0), quit */
            break;

        p += n;
        count -= n;                                   /* Space left in output buffer */
    }

    /* If not enough room for last character, pad remainder with null
    so that return value = original count, indicating buffer full. */
    if (n == 0)
    {
        while (count--)
            * p++ = 0;
    }

    /* Add a null terminator if there's room. */
    else if (count)
        *p = 0;

    if (n == -1)                            /* Conversion encountered invalid wide char. */
        return -1;

    /* Return the number of bytes written to output buffer, excl null. */
    return (p - utf8str);
}
//-----------------------------------------------------------------------
int CodeCvt::mbstowcs(wchar_t *wcstr, const char *utf8str, size_t count)
{
    size_t byteCount = sizeof(wchar_t);
    if (byteCount == 4)
    {
        return _utf8s_to_wcs4(wcstr, utf8str, count);
    }
    else if (byteCount == 2)
    {
        return 0;
    }
    else
    {
        assert(0);
        return -1;
    }
}
//-----------------------------------------------------------------------
int CodeCvt::wcstombs(char *utf8str, const wchar_t *wcstr, size_t count)
{
    size_t byteCount = sizeof(wchar_t);
    if (byteCount == 4)
    {
        return _wcs4_to_utf8s(utf8str, wcstr, count);
    }
    else if (byteCount == 2)
    {
        return 0;
    }
    else
    {
        assert(0);
        return -1;
    }
}