#ifndef __U2TaskQueue_H__
#define __U2TaskQueue_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2ThreadHeaders.h"


U2EG_NAMESPACE_BEGIN


class TaskQueue
{
    typedef std::function < void() >            Task;

private:
    U2_AUTO_MUTEX;
    // 线程池
    vector<U2_THREAD_TYPE>::type    m_Pool;
    // 任务队列
    std::queue<Task>                m_Tasks;
    std::condition_variable         m_cvTask;
    // 是否关闭提交
    std::atomic<bool>               m_atmStop;

public:
    // 构造
    TaskQueue(size_t size = 4) 
        : m_atmStop(false)
    {
        size = size < 1 ? 1 : size;
        for (size_t i = 0; i < size; ++i)
        {
            m_Pool.emplace_back(&TaskQueue::schedual, this);    // push_back(std::thread{...})
        }
    }

    // 析构
    ~TaskQueue()
    {
        for (U2_THREAD_TYPE& thread : m_Pool)
        {
            thread.detach();    // 让线程“自生自灭”
            //thread.join();        // 等待任务结束， 前提：线程一定会执行完
        }
    }

    // 停止任务提交
    void shutdown()
    {
        this->m_atmStop.store(true);
    }

    // 重启任务提交
    void restart()
    {
        this->m_atmStop.store(false);
    }

    // 提交一个任务
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->std::future < decltype(f(args...)) > 
    {
        if (m_atmStop.load())
        {   // stop == true ??
            throw std::runtime_error("task queue have closed commit.");
        }

        using ReturnType = decltype(f(args...));    // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型

        auto task = std::make_shared< std::packaged_task<ReturnType()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );    // wtf !

        {
            U2_LOCK_AUTO_MUTEX;
            // 添加任务到队列
            m_Tasks.emplace([task](){   // push(Task{...})
                (*task)();
            });
        }

        m_cvTask.notify_all();    // 唤醒线程执行
        std::future<ReturnType> future = task->get_future();
        return future;
    }

private:
    // 获取一个待执行的 task
    Task get_one_task()
    {
        U2_LOCK_AUTO_MUTEX;
        m_cvTask.wait(lck, [this](){ return !m_Tasks.empty(); });    // wait 直到有 task
        Task task( std::move(m_Tasks.front()) );    // 取一个 task
        m_Tasks.pop();
        return task;
    }

    // 任务调度
    void schedual()
    {
        while (true)
        {
            if (Task task = get_one_task())
            {
                task();
            }
            else
            {
                // return;    // done
            }
        }
    }

};


U2EG_NAMESPACE_END


#endif
