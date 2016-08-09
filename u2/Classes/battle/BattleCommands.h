#ifndef __BattleCommands__
#define __BattleCommands__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING



class Trans2BattleCommand : public SimpleCommand
{
public:
    Trans2BattleCommand(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~Trans2BattleCommand();

    virtual void go(const Notification& notification) override;
};



#endif /* defined(__BattleCommands__) */
