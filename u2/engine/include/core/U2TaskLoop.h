#ifndef __U2TaskLoop_H__
#define __U2TaskLoop_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2Singleton.h"
#include "U2SimpleObjectManager.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Task;
class Scheduler;


class TaskLoop : public Object, public GeneralAllocatedObject
{
public:
    // A DestructionObserver is notified when the current MessageLoop is being
    // destroyed.  These observers are notified prior to MessageLoop::current()
    // being changed to return NULL.  This gives interested parties the chance to
    // do final cleanup that depends on the MessageLoop.
    //
    // NOTE: Any tasks posted to the MessageLoop during this notification will
    // not be run.  Instead, they will be deleted.
    //
    class TaskLoopListener 
    {
    public:
        virtual void postRunCurrentTaskLoop(TaskLoop* loop) = 0;
        virtual void postQuitCurrentTaskLoop(TaskLoop* loop) = 0;
        virtual void prePauseCurrentTaskLoop(TaskLoop* loop) = 0;
        virtual void postResumeCurrentTaskLoop(TaskLoop* loop) = 0;
        virtual void preDestroyCurrentTaskLoop(TaskLoop* loop) = 0;

    protected:
        virtual ~TaskLoopListener() {};
    };

    // A TaskObserver is an object that receives task notifications from the
    // MessageLoop.
    //
    // NOTE: A TaskObserver implementation should be extremely fast!
    class TaskListener
    {
    public:
        TaskListener() {};

        // This method is called before processing a task.
        virtual void WillProcessTask() = 0;

        // This method is called after processing a task.
        virtual void DidProcessTask() = 0;

    protected:
        virtual ~TaskListener() {};
    };

public:
    explicit TaskLoop(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~TaskLoop();

    // Add a DestructionObserver, which will start receiving notifications
    // immediately.
    void addTaskLoopListener(TaskLoopListener* listener);

    // Remove a DestructionObserver.  It is safe to call this method while a
    // DestructionObserver is receiving a notification callback.
    void removeTaskLoopListener(TaskLoopListener* listener);

    // These functions can only be called on the same thread that |this| is
    // running on.
    void addTaskListener(TaskListener* listener);
    void removeTaskListener(TaskListener* listener);

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
    virtual void postTask(Task* task) = 0;
    
    // Posts |task| on the current TaskRunner.  On completion, |reply|
    // is posted to the thread that called PostTaskAndReply().  Both
    // |task| and |reply| are guaranteed to be deleted on the thread
    // from which PostTaskAndReply() is invoked.  This allows objects
    // that must be deleted on the originating thread to be bound into
    // the |task| and |reply| Closures.  In particular, it can be useful
    // to use WeakPtr<> in the |reply| Closure so that the reply
    // operation can be canceled. See the following pseudo-code:
    //
    // class DataBuffer : public RefCountedThreadSafe<DataBuffer> {
    //  public:
    //   // Called to add data into a buffer.
    //   void AddData(void* buf, size_t length);
    //   ...
    // };
    //
    //
    // class DataLoader : public SupportsWeakPtr<DataLoader> {
    //  public:
    //    void GetData() {
    //      scoped_refptr<DataBuffer> buffer = new DataBuffer();
    //      target_thread_.message_loop_proxy()->PostTaskAndReply(
    //          FROM_HERE,
    //          base::Bind(&DataBuffer::AddData, buffer),
    //          base::Bind(&DataLoader::OnDataReceived, AsWeakPtr(), buffer));
    //    }
    //
    //  private:
    //    void OnDataReceived(scoped_refptr<DataBuffer> buffer) {
    //      // Do something with buffer.
    //    }
    // };
    //
    //
    // Things to notice:
    //   * Results of |task| are shared with |reply| by binding a shared argument
    //     (a DataBuffer instance).
    //   * The DataLoader object has no special thread safety.
    //   * The DataLoader object can be deleted while |task| is still running,
    //     and the reply will cancel itself safely because it is bound to a
    //     WeakPtr<>.
    virtual void postTaskAndReply(Task* task, Task* reply) = 0;

    // Run the message loop.
    virtual void run() = 0;

    // Signals the Run method to return after it is done processing all pending
    // messages.  This method may only be called on the same thread that called
    // Run, and Run must still be on the call stack.
    //
    // Use QuitClosure if you need to Quit another thread's MessageLoop, but note
    // that doing so is fairly dangerous if the target thread makes nested calls
    // to MessageLoop::Run.  The problem being that you won't know which nested
    // run loop you are quitting, so be careful!
    virtual void quit() = 0;

    virtual void pause() = 0;

    virtual void resume() = 0;

    virtual String getThreadId() = 0;

    virtual void postSchedulerTask(const String& schedulerTaskName
        , const String& taskType, const String& taskName
        , u2uint64 period, bool repeat = false, bool catchUp = true);

    virtual void join() {};

    virtual bool isRunning() = 0;

    virtual bool isPausing() = 0;

protected:
    // Runs the specified Task.
    void _runTask(Task* task);
    
    void _postRunCurrentTaskLoop();
    void _postQuitCurrentTaskLoop();
    void _prePauseCurrentTaskLoop();
    void _postResumeCurrentTaskLoop();
    void _preDestroyCurrentTaskLoop();


protected:
    typedef vector<TaskLoopListener*>::type TaskLoopListenerList;
    TaskLoopListenerList m_TaskLoopListeners;

    typedef vector<TaskListener*>::type TaskListenerList;
    TaskListenerList     m_TaskListeners;

    Scheduler*           m_pScheduler;
};



class TaskLoopManager : public TaskLoop::TaskLoopListener
    , public Singleton < TaskLoopManager >
    , public SimpleObjectManager<TaskLoop>
    , public GeneralAllocatedObject
{
public:
    TaskLoopManager();
    virtual ~TaskLoopManager();

    virtual TaskLoop* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

    void postTask(const String& loopName, Task* task);
    void postTaskAndReply(const String& loopName, Task* task, Task* reply);

    // Returns the TaskLoop object for the current thread, or null if none.
    static TaskLoop* current();

    void quitAll();
    void runAll();
    void pauseAll();
    void resumeAll();

    virtual void postRunCurrentTaskLoop(TaskLoop* loop) override;
    virtual void postQuitCurrentTaskLoop(TaskLoop* loop) override;
    virtual void prePauseCurrentTaskLoop(TaskLoop* loop) override;
    virtual void postResumeCurrentTaskLoop(TaskLoop* loop) override;
    virtual void preDestroyCurrentTaskLoop(TaskLoop* loop) override;

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static TaskLoopManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static TaskLoopManager* getSingletonPtr(void);

protected:
    typedef map<String, TaskLoop* >::type   TaskLoopMap;
    static TaskLoopMap      ms_TaskLoops;
};



/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2TaskLoop_H__
