#ifndef __U2GameObjectSerialize__
#define __U2GameObjectSerialize__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Serialize.h"


U2EG_NAMESPACE_BEGIN



class GameObjectSerialize : public Serialize
{
public:
    GameObjectSerialize(const String& type, const String& name);
    virtual ~GameObjectSerialize();

    virtual void load(const void* data) override;
    virtual void save() override;

protected:
};


U2EG_NAMESPACE_END

#endif /* defined(__U2GameObjectSerialize__) */
