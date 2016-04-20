#ifndef __U2ScriptLoader_H__
#define __U2ScriptLoader_H__

#include "U2Prerequisites.h"
#include "U2Stream.h"
#include "U2StringUtil.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/
/** Abstract class defining the interface used by classes which wish 
    to perform script loading to define instances of whatever they manage.
@remarks
    Typically classes of this type wish to either parse individual script files
    on demand, or be called with a group of files matching a certain pattern
    at the appropriate time. Normally this will coincide with resource loading,
    although the script use does not necessarily have to be a ResourceManager
    (which subclasses from this class), it may be simply a script loader which 
    manages non-resources but needs to be synchronised at the same loading points.
@par
    Subclasses should add themselves to the ResourceGroupManager as a script loader
    if they wish to be called at the point a resource group is loaded, at which 
    point the parseScript method will be called with each file which matches a 
    the pattern returned from getScriptPatterns.
*/
class _U2Export ScriptLoader
{
public:
    virtual ~ScriptLoader();

    /** Gets the file patterns which should be used to find scripts for this
        class.
    @remarks
        This method is called when a resource group is loaded if you use 
        ResourceGroupManager::_registerScriptLoader.
    @return
        A list of file patterns, in the order they should be searched in.
    */
    virtual const StringVector& getScriptPatterns(void) const = 0;

    /** Parse a script file.
    @param stream Weak reference to a data stream which is the source of the script
    @param groupName The name of a resource group which should be used if any resources
        are created during the parse of this script.
    */
    virtual void parseScript(InStreamPtr& stream, const String& groupName) = 0;

    /** Gets the relative loading order of scripts of this type.
    @remarks
        There are dependencies between some kinds of scripts, and to enforce
        this all implementors of this interface must define a loading order. 
    @return A value representing the relative loading order of these scripts
        compared to other script users, where higher values load later.
    */
    virtual u2real getLoadingOrder(void) const  = 0;

};

/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif
