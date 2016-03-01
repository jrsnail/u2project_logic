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
    // �̳߳�
    vector<U2_THREAD_TYPE>::type    m_Pool;
    // �������
    std::queue<Task>                m_Tasks;
    std::condition_variable         m_cvTask;
    // �Ƿ�ر��ύ
    std::atomic<bool>               m_atmStop;

public:
    // ����
    TaskQueue(size_t size = 4) 
        : m_atmStop(false)
    {
        size = size < 1 ? 1 : size;
        for (size_t i = 0; i < size; ++i)
        {
            m_Pool.emplace_back(&TaskQueue::schedual, this);    // push_back(std::thread{...})
        }
    }

    // ����
    ~TaskQueue()
    {
        for (U2_THREAD_TYPE& thread : m_Pool)
        {
            thread.detach();    // ���̡߳���������
            //thread.join();        // �ȴ���������� ǰ�᣺�߳�һ����ִ����
        }
    }

    // ֹͣ�����ύ
    void shutdown()
    {
        this->m_atmStop.store(true);
    }

    // ���������ύ
    void restart()
    {
        this->m_atmStop.store(false);
    }

    // �ύһ������
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->std::future < decltype(f(args...)) > 
    {
        if (m_atmStop.load())
        {   // stop == true ??
            throw std::runtime_error("task queue have closed commit.");
        }

        using ReturnType = decltype(f(args...));    // typename std::result_of<F(Args...)>::type, ���� f �ķ���ֵ����

        auto task = std::make_shared< std::packaged_task<ReturnType()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );    // wtf !

        {
            U2_LOCK_AUTO_MUTEX;
            // ������񵽶���
            m_Tasks.emplace([task](){   // push(Task{...})
                (*task)();
            });
        }

        m_cvTask.notify_all();    // �����߳�ִ��
        std::future<ReturnType> future = task->get_future();
        return future;
    }

private:
    // ��ȡһ����ִ�е� task
    Task get_one_task()
    {
        U2_LOCK_AUTO_MUTEX;
        m_cvTask.wait(lck, [this](){ return !m_Tasks.empty(); });    // wait ֱ���� task
        Task task( std::move(m_Tasks.front()) );    // ȡһ�� task
        m_Tasks.pop();
        return task;
    }

    // �������
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
