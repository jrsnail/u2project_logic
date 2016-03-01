//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2MacroCommand__
#define __U2MacroCommand__


#include "cocos2d.h"
#include "Command.h"


class MacroCommand : public Command
{
public:
    MacroCommand(const String& type);
    virtual ~MacroCommand();

    virtual void go(INotification const& notification) override;
    virtual void back() override;
    virtual bool canBack() override;
    virtual std::string serialize() override;
    virtual void deserialize(const std::string& str) override;

protected:
    /**
    * Initialize the <code>MacroCommand</code>.
    *
    * <P>
    * In your subclass, override this method to
    * initialize the <code>MacroCommand</code>'s <i>SubCommand</i>
    * list with <code>ICommand</code> class references.
    *
    * <P>
    * Note that <i>SubCommand</i>s may be any <code>ICommand</code> implementor,
    * <code>MacroCommand</code>s or <code>SimpleCommands</code> are both acceptable.
    */
    virtual void initializeMacroCommand(void);

    /**
    * Add a <i>SubCommand</i>.
    *
    * <P>
    * The <i>SubCommands</i> will be called in First In/First Out (FIFO)
    * order.</P>
    *
    * @param command a reference to the <code>ICommand</code> of the <code>ICommand</code>.
    */
    virtual void addSubCommand(Command* command);

protected:
    typedef std::list<Command*>         CommandList;
    CommandList _sub_commands;
};

#endif /* defined(__U2MacroCommand__) */
