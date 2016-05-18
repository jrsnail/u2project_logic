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
        virtual void WillProcessTask(TimeTicks time_posted) = 0;

        // This method is called after processing a task.
        virtual void DidProcessTask(TimeTicks time_posted) = 0;

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
                : PendingTask(_calculateDelayedRuntime(0), true)
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
    >::type PostDelayedTask(Function&& f, u2uint64 delay_ms)
    {

        typedef typename std::remove_reference<Function>::type function_type;

        class WrappedFunction : public PendingTask
        {
            Function f;
        public:
            WrappedFunction(function_type&& f)
                : PendingTask(_calculateDelayedRuntime(delay_ms), true)
                , f(std::move(f)) { }
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

    template<class Function>
    typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
        std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value
    >::type PostNonNestableDelayedTask(Function&& f, u2uint64 delay_ms)
    {

        typedef typename std::remove_reference<Function>::type function_type;

        class WrappedFunction : public PendingTask
        {
            Function f;
        public:
            WrappedFunction(function_type&& f)
                : PendingTask(_calculateDelayedRuntime(delay_ms), false)
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

    virtual bool DoDelayedWork(TimeTicks* next_delayed_work_time) override;

    virtual bool DoIdleWork() override;

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

    // This method is a variant of Quit, that does not wait for pending messages
    // to be processed before returning from Run.
    void QuitNow();

    // Enables or disables the recursive task processing. This happens in the case
    // of recursive message loops. Some unwanted message loop may occurs when
    // using common controls or printer functions. By default, recursive task
    // processing is disabled.
    //
    // Please utilize |ScopedNestableTaskAllower| instead of calling these methods
    // directly.  In general nestable message loops are to be avoided.  They are
    // dangerous and difficult to get right, so please use with extreme caution.
    //
    // The specific case where tasks get queued is:
    // - The thread is running a message loop.
    // - It receives a task #1 and execute it.
    // - The task #1 implicitly start a message loop, like a MessageBox in the
    //   unit test. This can also be StartDoc or GetSaveFileName.
    // - The thread receives a task #2 before or while in this second message
    //   loop.
    // - With NestableTasksAllowed set to true, the task #2 will run right away.
    //   Otherwise, it will get executed right after task #1 completes at "thread
    //   message loop level".
    void SetNestableTasksAllowed(bool allowed);
    inline bool isNestableTasksAllowed() const;

    // These functions can only be called on the same thread that |this| is
    // running on.
    void AddTaskObserver(TaskObserver* task_observer);
    void RemoveTaskObserver(TaskObserver* task_observer);

protected:
    void _addToIncomingQueue(PendingTask* pending_task);

    // Adds the pending task to delayed_work_queue_.
    void _addToDelayedWorkQueue(PendingTask* pending_task);

    // Calculates the time at which a PendingTask should run.
    TimeTicks _calculateDelayedRuntime(u2int64 delay_ms);

    // A surrounding stack frame around the running of the message loop that
    // supports all saving and restoring of state, as is needed for any/all (ugly)
    // recursive calls.
    void _runInternal();

    // Calls RunTask or queues the pending_task on the deferred task list if it
    // cannot be run right now.  Returns true if the task was run.
    bool _deferOrRunPendingTask(PendingTask* pending_task);

    // Load tasks from the incoming_queue_ into work_queue_ if the latter is
    // empty.  The former requires a lock to access, while the latter is directly
    // accessible on this thread.
    void _reloadWorkQueue();

    // Runs the specified PendingTask.
    void _runTask(PendingTask* pending_task);

    // Called to process any delayed non-nestable tasks.
    bool _processNextDelayedNonNestableTask();

protected:
    struct RunState 
    {
        // Used to count how many Run() invocations are on the stack.
        u2int32 nRunDepth;

        // Used to record that Quit() was called, or that we should quit the pump
        // once it becomes idle.
        bool bQuitReceived;
    };

     class _U2Export AutoRunState : RunState
     {
     public:
         explicit AutoRunState(MsgLoop* loop);
         ~AutoRunState();
     private:
         MsgLoop* m_pLoop;
         RunState* m_pPreviousState;
     };


protected:
    typedef vector<DestructionObserver*>::type DestructionListenerList;
    DestructionListenerList m_DestructionListeners;

    typedef vector<TaskObserver*>::type TaskListenerList;
    TaskListenerList m_TaskListeners;

    Type m_eType;
    std::unique_ptr<MsgPump> m_spPump;

    // PendingTasks are sorted by their |delayed_run_time| property.
    typedef std::priority_queue<PendingTask*> DelayedTaskQueue;
    // Contains delayed tasks, sorted by their 'delayed_run_time' property.
    DelayedTaskQueue delayed_work_queue_;

    // A null terminated list which creates an incoming_queue of tasks that are
    // acquired under a mutex for processing on this instance's thread. These
    // tasks have not yet been sorted out into items for our work_queue_ vs items
    // that will be handled by the TimerManager.
    std::queue<PendingTask*> m_IncomingQueue;
    // Protect access to m_IncomingQueue.
    U2_MUTEX(m_mtxIncomingQueue);

    // A queue of non-nestable tasks that we had to defer because when it came
    // time to execute them we were in a nested message loop.  They will execute
    // once we're out of nested message loops.
    std::queue<PendingTask*> m_DeferredNonNestableWorkQueue;

    // A list of tasks that need to be processed by this instance.  Note that
    // this queue is only accessed (push/pop) by our current thread.
    std::queue<PendingTask*> m_WorkQueue;

    // The next sequence number to use for delayed tasks.
    u2int32 m_nNextSequenceNum;

    // A recursion block that prevents accidentally running additional tasks when
    // insider a (accidentally induced?) nested message pump.
    bool m_bNestableTasksAllowed;

    RunState* m_pState;

    // A recent snapshot of Time::Now(), used to check delayed_work_queue_.
    TimeTicks m_RecentTime;
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2MsgLoop_H__
