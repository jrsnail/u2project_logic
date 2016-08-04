#ifndef __BattleCommands__
#define __BattleCommands__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING



class Trans2BattleCommand : public SimpleCommand
{
public:
    Trans2BattleCommand(const std::string& type, const std::string& name);
    virtual ~Trans2BattleCommand();

    virtual void go(const Notification& notification) override;
};



#endif /* defined(__BattleCommands__) */
