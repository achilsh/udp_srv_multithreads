#include "t_worktask.hpp"

namespace T_UDP
{
    ////////////
    WorkerTask::WorkerTask():PthreadBase(), m_Tms(3)
    {
    }

    WorkerTask::WorkerTask(pthread_mutex_t* pInitLock, pthread_cond_t* pInitCond, int* pInitNums)
        :PthreadBase(pInitLock, pInitCond, pInitNums), m_Tms(3)
    {
    }

    WorkerTask::~WorkerTask()
    {
        event_base_free(m_PthreadEventBase);
        m_PthreadEventBase = NULL;
    }

    int WorkerTask::Init() 
    {
        struct event_config *ev_config = event_config_new();
        event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
        m_PthreadEventBase = event_base_new_with_config(ev_config);
        event_config_free(ev_config);
        std::cout << "event base init, addr: " << m_PthreadEventBase << std::endl;
        if (m_PthreadEventBase == NULL)
        {
            m_bRun = false;
            return -1;
        }
        return 0;
    }
    
    void WorkerTask::TmrCallBackDoing()
    {
        //std::cout << "timer time out callback...." << std::endl;
        struct timeval tm_val  = {.tv_sec = m_Tms,  .tv_usec= 0};
        // event_add(&m_Tmrevent, &tm_val);
    }

    void WorkerTask::TmrCallback(int fd, short event, void *arg)
    {
        WorkerTask* pTask = (WorkerTask*)arg;
        pTask->TmrCallBackDoing();
    }

    int WorkerTask::main()
    {
        //must make sure an event added in event_base. so we can add timer event
        //in event_base. 
        event_assign(&m_Tmrevent, m_PthreadEventBase, -1, EV_PERSIST, TmrCallback, (void*)this);
        struct timeval tm_val  = {.tv_sec = m_Tms,  .tv_usec= 0};
        event_add(&m_Tmrevent, &tm_val);
        std::cout << "add timer event by " << m_Tms <<"s" << std::endl;
        
        RegistePthreadToPool();
        //
        event_base_dispatch(m_PthreadEventBase);
        std::cout << "thread exit " << std::endl;
        m_bRun = false;
        return 0;
    }

    void WorkerTask::AddRConn(Conn* pEvent)
    {
        pEvent->AddEvent(m_PthreadEventBase, EV_READ | EV_PERSIST);
    }
    /////////
}
