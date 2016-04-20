#include "U2ConfigFile.h"

#include "U2Exception.h"
#include "U2StringUtil.h"
#include "U2FileStream.h"
#include "U2ResourceGroupManager.h"
#include <iostream>


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
ConfigFile::ConfigFile()
{
}
//-----------------------------------------------------------------------
ConfigFile::~ConfigFile()
{
    SettingsBySection::iterator seci, secend;
    secend = mSettings.end();
    for (seci = mSettings.begin(); seci != secend; ++seci)
    {
        U2_DELETE_T(seci->second, SettingsMultiMap, MEMCATEGORY_GENERAL);
    }
}
//-----------------------------------------------------------------------
void ConfigFile::clear(void)
{
    for (SettingsBySection::iterator seci = mSettings.begin(); 
        seci != mSettings.end(); ++seci)
    {
        U2_DELETE_T(seci->second, SettingsMultiMap, MEMCATEGORY_GENERAL);
    }
    mSettings.clear();
}
//-----------------------------------------------------------------------
void ConfigFile::load(const String& filename, const String& separators, bool trimWhitespace)
{
    loadDirect(filename, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::load(const String& filename, const String& resourceGroup, 
    const String& separators, bool trimWhitespace)
{
    loadFromResourceSystem(filename, resourceGroup, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::loadDirect(const String& filename, const String& separators, 
    bool trimWhitespace)
{
    /* Open the configuration file */
    std::ifstream fp;
    // Always open in binary mode
    fp.open(filename.c_str(), std::ios::in | std::ios::binary);
    if(!fp)
        U2_EXCEPT(
        Exception::ERR_FILE_NOT_FOUND, "'" + filename + "' file not found!", "ConfigFile::load" );

    // Wrap as a stream
    InStreamPtr stream(U2_NEW FileInStream(filename, &fp, false));

    load(stream, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::loadFromResourceSystem(const String& filename, 
    const String& resourceGroup, const String& separators, bool trimWhitespace)
{
    InStreamPtr stream = 
        ResourceGroupManager::getSingleton().openResource(filename, resourceGroup);
    load(stream, separators, trimWhitespace);
}
//-----------------------------------------------------------------------
void ConfigFile::load(const InStreamPtr& stream, const String& separators, 
    bool trimWhitespace)
{
    /* Clear current settings map */
    clear();

    String currentSection = BLANK;
    SettingsMultiMap* currentSettings = U2_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
    mSettings[currentSection] = currentSettings;


    /* Process the file line for line */
    String line, optName, optVal;
    while (!stream->eof())
    {
        line = stream->getLine();
        /* Ignore comments & blanks */
        if (line.length() > 0 && line.at(0) != '#' && line.at(0) != '@')
        {
            if (line.at(0) == '[' && line.at(line.length()-1) == ']')
            {
                // Section
                currentSection = line.substr(1, line.length() - 2);
                SettingsBySection::const_iterator seci = mSettings.find(currentSection);
                if (seci == mSettings.end())
                {
                    currentSettings = U2_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
                    mSettings[currentSection] = currentSettings;
                }
                else
                {
                    currentSettings = seci->second;
                } 
            }
            else
            {
                /* Find the first separator character and split the string there */
                String::size_type separator_pos = line.find_first_of(separators, 0);
                if (separator_pos != String::npos)
                {
                    optName = line.substr(0, separator_pos);
                    /* Find the first non-separator character following the name */
                    String::size_type nonseparator_pos = line.find_first_not_of(separators, separator_pos);
                    /* ... and extract the value */
                    /* Make sure we don't crash on an empty setting (it might be a valid value) */
                    optVal = (nonseparator_pos == String::npos) ? "" : line.substr(nonseparator_pos);
                    if (trimWhitespace)
                    {
                        StringUtil::trim(optVal);
                        StringUtil::trim(optName);
                    }
                    currentSettings->insert(SettingsMultiMap::value_type(optName, optVal));
                }
            }
        }
    }
}
//-----------------------------------------------------------------------
String ConfigFile::getSetting(const String& key, const String& section, const String& defaultValue) const
{
    SettingsBySection::const_iterator seci = mSettings.find(section);
    if (seci == mSettings.end())
    {
        return defaultValue;
    }
    else
    {
        SettingsMultiMap::const_iterator i = seci->second->find(key);
        if (i == seci->second->end())
        {
            return defaultValue;
        }
        else
        {
            return i->second;
        }
    }
}
//-----------------------------------------------------------------------
StringVector ConfigFile::getMultiSetting(const String& key, const String& section) const
{
    StringVector ret;

    SettingsBySection::const_iterator seci = mSettings.find(section);
    if (seci != mSettings.end())
    {
        SettingsMultiMap::const_iterator i;

        i = seci->second->find(key);
        // Iterate over matches
        while (i != seci->second->end() && i->first == key)
        {
            ret.push_back(i->second);
            ++i;
        }
    }
    return ret;
}
//-----------------------------------------------------------------------
ConfigFile::SettingsIterator ConfigFile::getSettingsIterator(const String& section)
{
    SettingsBySection::const_iterator seci = mSettings.find(section);
    if (seci == mSettings.end())
    {
        U2_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
            "Cannot find section " + section, 
            "ConfigFile::getSettingsIterator");
    }
    else
    {
        return SettingsIterator(seci->second->begin(), seci->second->end());
    }
}
//-----------------------------------------------------------------------
ConfigFile::SectionIterator ConfigFile::getSectionIterator(void)
{
    return SectionIterator(mSettings.begin(), mSettings.end());
}
