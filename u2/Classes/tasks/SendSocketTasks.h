#ifndef __SendSocketTasks_H__
#define __SendSocketTasks_H__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class MoveSST : public SendSocketTask
{
public:
    MoveSST(const String& type, const String& name);

    virtual ~MoveSST();

};


#endif /* defined(__SendSocketTasks_H__) */
