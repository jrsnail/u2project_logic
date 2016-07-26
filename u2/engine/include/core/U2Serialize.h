#ifndef __U2Serialize__
#define __U2Serialize__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN



class Serialize : public Object
{
public:
    Serialize(const String& type, const String& name);
    virtual ~Serialize();

    virtual void load(const void* data) = 0;
    virtual void save() = 0;

protected:
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Serialize__) */
