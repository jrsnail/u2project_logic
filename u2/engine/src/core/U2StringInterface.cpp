#include "U2StringInterface.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
U2_STATIC_MUTEX_INSTANCE( StringInterface::msDictionaryMutex );
ParamDictionaryMap StringInterface::msDictionary;
//-----------------------------------------------------------------------
const ParameterList& StringInterface::getParameters(void) const
{
    static ParameterList emptyList;

    const ParamDictionary* dict = getParamDictionary();
    if (dict)
        return dict->getParameters();
    else
        return emptyList;
}
//-----------------------------------------------------------------------
bool StringInterface::setParameter(const String& name, const String& value)
{
    // Get dictionary
    ParamDictionary* dict = getParamDictionary();

    if (dict)
    {
        // Look up command object
        ParamCommand* cmd = dict->getParamCommand(name);
        if (cmd)
        {
            cmd->doSet(this, value);
            return true;
        }
    }
    // Fallback
    return false;
}
//-----------------------------------------------------------------------
void StringInterface::setParameterList(const NameValuePairList& paramList)
{
    NameValuePairList::const_iterator i, iend;
    iend = paramList.end();
    for (i = paramList.begin(); i != iend; ++i)
    {
        setParameter(i->first, i->second);
    }
}
//-----------------------------------------------------------------------
void StringInterface::cleanupDictionary ()
{
    U2_LOCK_MUTEX( msDictionaryMutex );

    msDictionary.clear();
}