#ifndef __U2ConfigFile_H__
#define __U2ConfigFile_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2IteratorWrapper.h"
#include "U2Stream.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/
/** Class for quickly loading settings from a text file.
    @remarks
        This class is designed to quickly parse a simple file containing
        key/value pairs, mainly for use in configuration settings.
    @par
        This is a very simplified approach, no multiple values per key
        are allowed, no grouping or context is being kept etc.
    @par
        By default the key/values pairs are tokenised based on a
        separator of Tab, the colon (:) or equals (=) character. Each
        key - value pair must end in a carriage return.
    @par
        Settings can be optionally grouped in sections, using a header
        beforehand of the form [SectionName]. 
*/
class _U2Export ConfigFile : public GeneralAllocatedObject
{
public:
    ConfigFile();
    virtual ~ConfigFile();
    /// load from a filename (not using resource group locations)
    void load(const String& filename, const String& separators = "\t:=", bool trimWhitespace = true);
    /// load from a filename (using resource group locations)
    void load(const String& filename, const String& resourceGroup, const String& separators = "\t:=", bool trimWhitespace = true);
    /// load from a data stream
    void load(const InStreamPtr& stream, const String& separators = "\t:=", bool trimWhitespace = true);
    /// load from a filename (not using resource group locations)
    void loadDirect(const String& filename, const String& separators = "\t:=", bool trimWhitespace = true);
    /// load from a filename (using resource group locations)
    void loadFromResourceSystem(const String& filename, const String& resourceGroup, const String& separators = "\t:=", bool trimWhitespace = true);

    /** Gets the first setting from the file with the named key. 
    @param key The name of the setting
    @param section The name of the section it must be in (if any)
    @param defaultValue The value to return if the setting is not found
    */
    String getSetting(const String& key, const String& section = BLANK, const String& defaultValue = BLANK) const;
    /** Gets all settings from the file with the named key. */
    StringVector getMultiSetting(const String& key, const String& section = BLANK) const;

    typedef multimap<String, String>::type			SettingsMultiMap;
    typedef MapIterator<SettingsMultiMap>			SettingsIterator;
    /** Gets an iterator for stepping through all the keys / values in the file. */
    typedef map<String, SettingsMultiMap*>::type	SettingsBySection;
    typedef MapIterator<SettingsBySection>			SectionIterator;
    /** Get an iterator over all the available sections in the config file */
    SectionIterator getSectionIterator(void);
    /** Get an iterator over all the available settings in a section */
    SettingsIterator getSettingsIterator(const String& section = BLANK);

    /** Clear the settings */
    void clear(void);

protected:
    SettingsBySection mSettings;
};
/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif
