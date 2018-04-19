#include "t_worktask.hpp"

namespace T_UDP
{
    ////////////
    WorkerTask::WorkerTask(FdEvent* pFdEvent ):m_UdpEvent(pFdEvent)
    {
    }

    WorkerTask::~WorkerTask()
    {
        event_base_free(m_PthreadEventBase);
        m_PthreadEventBase = NULL;
        
        if (m_UdpEvent)
        {
            delete m_UdpEvent;
            m_UdpEvent  = NULL;
        }
    }

    int WorkerTask::main()
    {
        struct event_config *ev_config = event_config_new();
        event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
        m_PthreadEventBase = event_base_new_with_config(ev_config);
        event_config_free(ev_config);
        
        if (m_PthreadEventBase == NULL)
        {
            m_bRun = false;
            return -1;
        }
        //
        if (m_UdpEvent == NULL)
        {
            m_bRun = false;
            return -1;
        }

        m_UdpEvent->AddEvent(m_PthreadEventBase,EV_READ | EV_PERSIST);
        event_base_dispatch(m_PthreadEventBase);
        m_bRun = false;
        return 0;
    }
    /////////



}
