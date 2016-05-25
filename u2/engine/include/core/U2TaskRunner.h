#ifndef __U2TaskRunner_H__
#define __U2TaskRunner_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2Time.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Task;



// A TaskRunner is an object that runs posted tasks (in the form of
// Closure objects).  The TaskRunner interface provides a way of
// decoupling task posting from the mechanics of how each task will be
// run.  TaskRunner provides very weak guarantees as to how posted
// tasks are run (or if they're run at all).  In particular, it only
// guarantees:
//
//   - Posting a task will not run it synchronously.  That is, no
//     Post*Task method will call task.Run() directly.
//
//   - Increasing the delay can only delay when the task gets run.
//     That is, increasing the delay may not affect when the task gets
//     run, or it could make it run later than it normally would, but
//     it won't make it run earlier than it normally would.
//
// TaskRunner does not guarantee the order in which posted tasks are
// run, whether tasks overlap, or whether they're run on a particular
// thread.  Also it does not guarantee a memory model for shared data
// between tasks.  (In other words, you should use your own
// synchronization/locking primitives if you need to share data
// between tasks.)
//
// Implementations of TaskRunner should be thread-safe in that all
// methods must be safe to call on any thread.  Ownership semantics
// for TaskRunners are in general not clear, which is why the
// interface itself is RefCountedThreadSafe.
//
// Some theoretical implementations of TaskRunner:
//
//   - A TaskRunner that uses a thread pool to run posted tasks.
//
//   - A TaskRunner that, for each task, spawns a non-joinable thread
//     to run that task and immediately quit.
//
//   - A TaskRunner that stores the list of posted tasks and has a
//     method Run() that runs each runnable task in random order.
class _U2Export TaskRunner : public Object, public GeneralAllocatedObject
{
public:
    // Posts the given task to be run.  Returns true if the task may be
    // run at some point in the future, and false if the task definitely
    // will not be run.
    //
    // Equivalent to PostDelayedTask(from_here, task, 0).
    bool PostTask(Task* task);

    // Like PostTask, but tries to run the posted task only after
    // |delay_ms| has passed.
    //
    // It is valid for an implementation to ignore |delay_ms|; that is,
    // to have PostDelayedTask behave the same as PostTask.
    virtual bool PostDelayedTask(Task* task, TimeDelta delay) = 0;

    // Returns true if the current thread is a thread on which a task
    // may be run, and false if no task will be run on the current
    // thread.
    //
    // It is valid for an implementation to always return true, or in
    // general to use 'true' as a default value.
    virtual bool RunTasksOnCurrentThread() const = 0;

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
    bool PostTaskAndReply(Task* task, Task* reply);

protected:
    // Called when this object should be destroyed.  By default simply
    // deletes |this|, but can be overridden to do something else, like
    // delete on a certain thread.
    virtual void OnDestruct() const;

protected:
    TaskRunner();
    virtual ~TaskRunner();

};



// A SequencedTaskRunner is a subclass of TaskRunner that provides
// additional guarantees on the order that tasks are started, as well
// as guarantees on when tasks are in sequence, i.e. one task finishes
// before the other one starts.
//
// Summary
// -------
// Non-nested tasks with the same delay will run one by one in FIFO
// order.
//
// Detailed guarantees
// -------------------
//
// SequencedTaskRunner also adds additional methods for posting
// non-nestable tasks.  In general, an implementation of TaskRunner
// may expose task-running methods which are themselves callable from
// within tasks.  A non-nestable task is one that is guaranteed to not
// be run from within an already-running task.  Conversely, a nestable
// task (the default) is a task that can be run from within an
// already-running task.
//
// The guarantees of SequencedTaskRunner are as follows:
//
//   - Given two tasks T2 and T1, T2 will start after T1 starts if:
//
//       * T2 is posted after T1; and
//       * T2 has equal or higher delay than T1; and
//       * T2 is non-nestable or T1 is nestable.
//
//   - If T2 will start after T1 starts by the above guarantee, then
//     T2 will start after T1 finishes and is destroyed if:
//
//       * T2 is non-nestable, or
//       * T1 doesn't call any task-running methods.
//
//   - If T2 will start after T1 finishes by the above guarantee, then
//     all memory changes in T1 and T1's destruction will be visible
//     to T2.
//
//   - If T2 runs nested within T1 via a call to the task-running
//     method M, then all memory changes in T1 up to the call to M
//     will be visible to T2, and all memory changes in T2 will be
//     visible to T1 from the return from M.
//
// Note that SequencedTaskRunner does not guarantee that tasks are run
// on a single dedicated thread, although the above guarantees provide
// most (but not all) of the same guarantees.  If you do need to
// guarantee that tasks are run on a single dedicated thread, see
// SingleThreadTaskRunner (in single_thread_task_runner.h).
//
// Some corollaries to the above guarantees, assuming the tasks in
// question don't call any task-running methods:
//
//   - Tasks posted via PostTask are run in FIFO order.
//
//   - Tasks posted via PostNonNestableTask are run in FIFO order.
//
//   - Tasks posted with the same delay and the same nestable state
//     are run in FIFO order.
//
//   - A list of tasks with the same nestable state posted in order of
//     non-decreasing delay is run in FIFO order.
//
//   - A list of tasks posted in order of non-decreasing delay with at
//     most a single change in nestable state from nestable to
//     non-nestable is run in FIFO order. (This is equivalent to the
//     statement of the first guarantee above.)
//
// Some theoretical implementations of SequencedTaskRunner:
//
//   - A SequencedTaskRunner that wraps a regular TaskRunner but makes
//     sure that only one task at a time is posted to the TaskRunner,
//     with appropriate memory barriers in between tasks.
//
//   - A SequencedTaskRunner that, for each task, spawns a joinable
//     thread to run that task and immediately quit, and then
//     immediately joins that thread.
//
//   - A SequencedTaskRunner that stores the list of posted tasks and
//     has a method Run() that runs each runnable task in FIFO order
//     that can be called from any thread, but only if another
//     (non-nested) Run() call isn't already happening.
class _U2Export SequencedTaskRunner : public TaskRunner
{
public:
    // The two PostNonNestable*Task methods below are like their
    // nestable equivalents in TaskRunner, but they guarantee that the
    // posted task will not run nested within an already-running task.
    //
    // A simple corollary is that posting a task as non-nestable can
    // only delay when the task gets run.  That is, posting a task as
    // non-nestable may not affect when the task gets run, or it could
    // make it run later than it normally would, but it won't make it
    // run earlier than it normally would.

    // TODO(akalin): Get rid of the boolean return value for the methods
    // below.

    bool PostNonNestableTask(Task* task);

    virtual bool PostNonNestableDelayedTask(Task* task, TimeDelta delay) = 0;

    // Submits a non-nestable task to delete the given object.  Returns
    // true if the object may be deleted at some point in the future,
    // and false if the object definitely will not be deleted.
    bool DeleteSoon(void(*deleter)(const void*), const void* object);

};


// A SingleThreadTaskRunner is a SequencedTaskRunner with one more
// guarantee; namely, that all tasks are run on a single dedicated
// thread.  Most use cases require only a SequencedTaskRunner, unless
// there is a specific need to run tasks on only a single thread.
//
// SingleThreadTaskRunner implementations might:
//   - Post tasks to an existing thread's MessageLoop (see MessageLoopProxy).
//   - Create their own worker thread and MessageLoop to post tasks to.
//   - Add tasks to a FIFO and signal to a non-MessageLoop thread for them to
//     be processed. This allows TaskRunner-oriented code run on threads
//     running other kinds of message loop, e.g. Jingle threads.
class _U2Export SingleThreadTaskRunner : public SequencedTaskRunner
{
public:
    // A more explicit alias to RunsTasksOnCurrentThread().
    bool BelongsToCurrentThread() const
    {
        return RunTasksOnCurrentThread();
    }

protected:
    virtual ~SingleThreadTaskRunner() {}
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2TaskRunner_H__
