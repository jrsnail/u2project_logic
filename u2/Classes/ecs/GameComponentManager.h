#ifndef __GameComponentManager__
#define __GameComponentManager__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameComponentManager : public ComponentManager
{
public:
    GameComponentManager();

    virtual ~GameComponentManager();

    virtual Resource* createImpl(const u2::String& name, ResourceHandle handle,
        const u2::String& group, bool isManual, ManualResourceLoader* loader,
        const NameValuePairList* createParams) override;

};


#endif /* defined(__GameComponentManager__) */
