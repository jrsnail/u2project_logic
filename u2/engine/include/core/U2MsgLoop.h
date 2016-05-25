#ifndef __U2MsgLoop_H__
#define __U2MsgLoop_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MsgPump.h"
#include "U2Time.h"
#include <queue>
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class ThreadPoolTask;
class PendingTask;


class _U2Export MsgLoop : public MsgPump::Delegate
{
public:
    // A MessageLoop has a particular type, which indicates the set of
    // asynchronous events it may process in addition to tasks and timers.
    //
    // TYPE_DEFAULT
    //   This type of ML only supports tasks and timers.
    //
    // TYPE_UI
    //   This type of ML also supports native UI events (e.g., Windows messages).
    //   See also MessageLoopForUI.
    //
    // TYPE_IO
    //   This type of ML also supports asynchronous IO.  See also
    //   MessageLoopForIO.
    //
    enum Type 
    {
        TYPE_DEFAULT,
        TYPE_UI,
        TYPE_IO
    };

    // A DestructionObserver is notified when the current MessageLoop is being
    // destroyed.  These observers are notified prior to MessageLoop::current()
    // being changed to return NULL.  This gives interested parties the chance to
    // do final cleanup that depends on the MessageLoop.
    //
    // NOTE: Any tasks posted to the MessageLoop during this notification will
    // not be run.  Instead, they will be deleted.
    //
    class _U2Export DestructionObserver 
    {
    public:
        virtual void WillDestroyCurrentMessageLoop() = 0;

    protected:
        virtual ~DestructionObserver();
    };

    // A TaskObserver is an object that receives task notifications from the
    // MessageLoop.
    //
    // NOTE: A TaskObserver implementation should be extremely fast!
    class _U2Export TaskObserver
    {
    public:
        TaskObserver() {};

        // This method is called before processing a task.
        virtual void WillProcessTask() = 0;

        // This method is called after processing a task.
        virtual void DidProcessTask() = 0;

    protected:
        virtual ~TaskObserver() {};
    };

public:
    explicit MsgLoop(Type type = TYPE_DEFAULT);
    virtual ~MsgLoop();

    // Returns the MessageLoop object for the current thread, or null if none.
    static MsgLoop* current();

    // Add a DestructionObserver, which will start receiving notifications
    // immediately.
    void AddDestructionObserver(DestructionObserver* destruction_observer);

    // Remove a DestructionObserver.  It is safe to call this method while a
    // DestructionObserver is receiving a notification callback.
    void RemoveDestructionObserver(DestructionObserver* destruction_observer);

    // Returns the type passed to the constructor.
    Type type() const { return m_eType; }

    // The "PostTask" family of methods call the task's Run method asynchronously
    // from within a message loop at some point in the future.
    //
    // With the PostTask variant, tasks are invoked in FIFO order, inter-mixed
    // with normal UI or IO event processing.  With the PostDelayedTask variant,
    // tasks are called after at least approximately 'delay_ms' have elapsed.
    //
    // The NonNestable variants work similarly except that they promise never to
    // dispatch the task from a nested invocation of MessageLoop::Run.  Instead,
    // such tasks get deferred until the top-most MessageLoop::Run is executing.
    //
    // The MessageLoop takes ownership of the Task, and deletes it after it has
    // been Run().
    //
    // NOTE: These methods may be called on any thread.  The Task will be invoked
    // on the thread that executes MessageLoop::Run().
    template<class Function>
    typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
        std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value
    >::type PostTask(Function&& f)
    {

        typedef typename std::remove_reference<Function>::type function_type;

        class WrappedFunction : public PendingTask
        {
            Function f;
        public:
            WrappedFunction(function_type&& f) 
                : PendingTask(true)
                , f(std::move(f))
            { }
            virtual void operator()() override
            {
                f();
                delete this;
            }
        };

        _addToIncomingQueue(new WrappedFunction(std::forward<Function>(f)));
    }

    template<class Function>
    typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
        std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value
    >::type PostNonNestableTask(Function&& f)
    {

        typedef typename std::remove_reference<Function>::type function_type;

        class WrappedFunction : public PendingTask
        {
            Function f;
        public:
            WrappedFunction(function_type&& f)
                : PendingTask(_calculateDelayedRuntime(0), false)
                , f(std::move(f)) { }
            virtual void operator()() override
            {
                f();
                delete this;
            }
        };

        _addToIncomingQueue(new WrappedFunction(std::forward<Function>(f)));
    }

    virtual bool DoWork() override;

    // Run the message loop.
    void Run();

    // Process all pending tasks, windows messages, etc., but don't wait/sleep.
    // Return as soon as all items that can be run are taken care of.
    void RunAllPending();

    // Signals the Run method to return after it is done processing all pending
    // messages.  This method may only be called on the same thread that called
    // Run, and Run must still be on the call stack.
    //
    // Use QuitClosure if you need to Quit another thread's MessageLoop, but note
    // that doing so is fairly dangerous if the target thread makes nested calls
    // to MessageLoop::Run.  The problem being that you won't know which nested
    // run loop you are quitting, so be careful!
    void Quit();

    // These functions can only be called on the same thread that |this| is
    // running on.
    void AddTaskObserver(TaskObserver* task_observer);
    void RemoveTaskObserver(TaskObserver* task_observer);

protected:
    void _addToIncomingQueue(PendingTask* pending_task);

    // A surrounding stack frame around the running of the message loop that
    // supports all saving and restoring of state, as is needed for any/all (ugly)
    // recursive calls.
    void _runInternal();

    // Load tasks from the incoming_queue_ into work_queue_ if the latter is
    // empty.  The former requires a lock to access, while the latter is directly
    // accessible on this thread.
    void _reloadWorkQueue();

    // Runs the specified PendingTask.
    void _runTask(PendingTask* pending_task);


protected:
    static map<String, std::shared_ptr<MsgLoop> >::type      ms_MsgLoops;

    typedef vector<DestructionObserver*>::type DestructionListenerList;
    DestructionListenerList m_DestructionListeners;

    typedef vector<TaskObserver*>::type TaskListenerList;
    TaskListenerList m_TaskListeners;

    Type m_eType;
    std::unique_ptr<MsgPump> m_spPump;

    // A null terminated list which creates an incoming_queue of tasks that are
    // acquired under a mutex for processing on this instance's thread. These
    // tasks have not yet been sorted out into items for our work_queue_ vs items
    // that will be handled by the TimerManager.
    std::queue<PendingTask*> m_IncomingQueue;
    // Protect access to m_IncomingQueue.
    U2_MUTEX(m_mtxIncomingQueue);

    // A list of tasks that need to be processed by this instance.  Note that
    // this queue is only accessed (push/pop) by our current thread.
    std::queue<PendingTask*> m_WorkQueue;

};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2MsgLoop_H__
