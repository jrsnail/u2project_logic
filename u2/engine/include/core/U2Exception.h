#ifndef __U2Exception_H__
#define __U2Exception_H__

// Precompiler options
#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
//#include "U2String.h"
#include <exception>
#include "U2HeaderPrefix.h"



// Check for U2 assert mode

// RELEASE_EXCEPTIONS mode
#if U2_ASSERT_MODE == 1
#   if U2_DEBUG_MODE
#       define U2Assert( a, b ) assert( (a) && (b) )
#   else
#       define U2Assert( a, b ) if( !(a) ) U2_EXCEPT( u2::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#   endif

// EXCEPTIONS mode
#elif U2_ASSERT_MODE == 2
#   define U2Assert( a, b ) if( !(a) ) U2_EXCEPT( u2::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")

// STANDARD mode
#else
#   define U2Assert( a, b ) assert( (a) && (b) )
#endif


U2EG_NAMESPACE_BEGIN



/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/
/** When thrown, provides information about an error that has occurred inside the engine.
    @remarks
        U2 never uses return values to indicate errors. Instead, if an
        error occurs, an exception is thrown, and this is the object that
        encapsulates the detail of the problem. The application using
        U2 should always ensure that the exceptions are caught, so all
        U2 engine functions should occur within a
        try{} catch(u2::Exception& e) {} block.
    @par
        The user application should never create any instances of this
        object unless it wishes to unify its error handling using the
        same object.
*/
class _U2Export Exception : public std::exception
{
protected:
    long line;
    int number;
    String typeName;
    String description;
    String source;
    String file;
    mutable String fullDesc;
public:
    /** Static definitions of error codes.
        @todo
            Add many more exception codes, since we want the user to be able
            to catch most of them.
    */
    enum ExceptionCodes 
	{
        ERR_CANNOT_WRITE_TO_FILE,
        ERR_INVALID_STATE,
        ERR_INVALIDPARAMS,
        ERR_RENDERINGAPI_ERROR,
        ERR_DUPLICATE_ITEM,
        ERR_ITEM_NOT_FOUND,
        ERR_FILE_NOT_FOUND,
        ERR_INTERNAL_ERROR,
        ERR_RT_ASSERTION_FAILED,
        ERR_NOT_IMPLEMENTED,
        ERR_INVALID_CALL
    };

    /** Default constructor.
    */
    Exception( int number, const String& description, const String& source );

    /** Advanced constructor.
    */
    Exception( int number, const String& description, const String& source, const char* type, const char* file, long line );

    /** Copy constructor.
    */
    Exception(const Exception& rhs);

    /// Needed for compatibility with std::exception
    ~Exception() throw() {}

    /** Assignment operator.
    */
    Exception & operator = (const Exception& rhs);

    /** Returns a string with the full description of this error.
        @remarks
            The description contains the error number, the description
            supplied by the thrower, what routine threw the exception,
            and will also supply extra platform-specific information
            where applicable. For example - in the case of a rendering
            library error, the description of the error will include both
            the place in which U2 found the problem, and a text
            description from the 3D rendering library, if available.
    */
    virtual const String& getFullDescription(void) const;

    /** Gets the error code.
    */
    virtual int getNumber(void) const throw();

    /** Gets the source function.
    */
    virtual const String &getSource() const { return source; }

    /** Gets source file name.
    */
    virtual const String &getFile() const { return file; }

    /** Gets line number.
    */
    virtual long getLine() const { return line; }

    /** Returns a string with only the 'description' field of this exception. Use 
        getFullDescriptionto get a full description of the error including line number,
        error number and what function threw the exception.
    */
    virtual const String &getDescription(void) const { return description; }

    /// Override std::exception::what
    const char* what() const throw() { return getFullDescription().c_str(); }
        
};


/** Template struct which creates a distinct type for each exception code.
@note
This is useful because it allows us to create an overloaded method
for returning different exception types by value without ambiguity. 
From 'Modern C++ Design' (Alexandrescu 2001).
*/

// Specialised exceptions allowing each to be caught specifically
// backwards-compatible since exception codes still used

class _U2Export UnimplementedException : public Exception 
{
public:
    UnimplementedException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "UnimplementedException", inFile, inLine) {}
};
class _U2Export FileNotFoundException : public Exception
{
public:
    FileNotFoundException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "FileNotFoundException", inFile, inLine) {}
};
class _U2Export IOException : public Exception
{
public:
    IOException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "IOException", inFile, inLine) {}
};
class _U2Export InvalidStateException : public Exception
{
public:
    InvalidStateException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "InvalidStateException", inFile, inLine) {}
};
class _U2Export InvalidParametersException : public Exception
{
public:
    InvalidParametersException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "InvalidParametersException", inFile, inLine) {}
};
class _U2Export ItemIdentityException : public Exception
{
public:
    ItemIdentityException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "ItemIdentityException", inFile, inLine) {}
};
class _U2Export InternalErrorException : public Exception
{
public:
    InternalErrorException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "InternalErrorException", inFile, inLine) {}
};
class _U2Export RenderingAPIException : public Exception
{
public:
    RenderingAPIException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "RenderingAPIException", inFile, inLine) {}
};
class _U2Export RuntimeAssertionException : public Exception
{
public:
    RuntimeAssertionException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "RuntimeAssertionException", inFile, inLine) {}
};
class _U2Export InvalidCallException : public Exception
{
public:
    InvalidCallException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
        : Exception(inNumber, inDescription, inSource, "InvalidCallException", inFile, inLine) {}
};

/** Class implementing dispatch methods in order to construct by-value
    exceptions of a derived type based just on an exception code.
@remarks
    This nicely handles construction of derived Exceptions by value (needed
    for throwing) without suffering from ambiguity - each code is turned into
    a distinct type so that methods can be overloaded. This allows U2_EXCEPT
    to stay small in implementation (desirable since it is embedded) whilst
    still performing rich code-to-type mapping. 
*/
class ExceptionFactory
{
private:
    /// Private constructor, no construction
    ExceptionFactory() {}

public:
    static U2_NORETURN void throwException(
        Exception::ExceptionCodes code, int number,
        const String& desc, 
        const String& src, const char* file, long line)
    {
        switch (code)
        {
        case Exception::ERR_CANNOT_WRITE_TO_FILE:   throw IOException(number, desc, src, file, line);
        case Exception::ERR_INVALID_STATE:          throw InvalidStateException(number, desc, src, file, line);
        case Exception::ERR_INVALIDPARAMS:          throw InvalidParametersException(number, desc, src, file, line);
        case Exception::ERR_RENDERINGAPI_ERROR:     throw RenderingAPIException(number, desc, src, file, line);
        case Exception::ERR_DUPLICATE_ITEM:         throw ItemIdentityException(number, desc, src, file, line);
        case Exception::ERR_ITEM_NOT_FOUND:         throw ItemIdentityException(number, desc, src, file, line);
        case Exception::ERR_FILE_NOT_FOUND:         throw FileNotFoundException(number, desc, src, file, line);
        case Exception::ERR_INTERNAL_ERROR:         throw InternalErrorException(number, desc, src, file, line);
        case Exception::ERR_RT_ASSERTION_FAILED:    throw RuntimeAssertionException(number, desc, src, file, line);
        case Exception::ERR_NOT_IMPLEMENTED:        throw UnimplementedException(number, desc, src, file, line);
        case Exception::ERR_INVALID_CALL:           throw InvalidCallException(number, desc, src, file, line);
        default:                                    throw Exception(number, desc, src, "Exception", file, line);
        }
    }

};
    

    
#ifndef U2_EXCEPT
#define U2_EXCEPT(code, desc, src)         u2::ExceptionFactory::throwException(code, code, desc, src, __FILE__, __LINE__)
#define U2_EXCEPT_EX(code, num, desc, src) u2::ExceptionFactory::throwException(code, num, desc, src, __FILE__, __LINE__)
#else
#define U2_EXCEPT_EX(code, num, desc, src) U2_EXCEPT(code, desc, src)
#endif


/** @} */
/** @} */

U2EG_NAMESPACE_END



#include "U2HeaderSuffix.h"

#endif
