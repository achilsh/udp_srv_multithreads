#include "t_event.hpp"

namespace T_UDP
{
    FdEvent::FdEvent(int iFd):m_iFd(iFd), m_uirBufLen(0), m_uiwBufLen(0) 
    {
        m_sRecv = new char[MAX_RECV_LEN];
        m_sSnd = new char[MAX_RECV_LEN];
    }

    FdEvent::~FdEvent()
    {
        if (m_iFd > 0)
        {
            ::close(m_iFd);
            m_iFd = -1;
        }
        if (m_sRecv)
        {
            delete []m_sRecv;
            m_sRecv = NULL;
        }
        if (m_sSnd)
        {
            delete [] m_sSnd;
            m_sSnd = NULL;
        }

        //del  event
        struct event_base *pEvBase = event_get_base(&m_stEvent);
        int iOrigEvent = event_get_events(&m_stEvent);
        DelEvent(pEvBase, iOrigEvent);
    }

    void FdEvent::ReadCallBack(int iFd, short sEvent, void *pData)  
    {
        FdEvent* pEv = (FdEvent*)pData;
        if (pEv == NULL)
        {
            return ;
        }
        pEv->DoReadProcess(iFd);
    }

    bool FdEvent::DoCmd(int iret)
    {
        return true;
    }
    bool FdEvent::DoReadProcess(int iFd)
    {
        if (iFd <= 0)
        {
            return false;
        }

        struct sockaddr_in addr;
        socklen_t size = sizeof(m_rwHostAddr);
        int iRet = recvfrom(m_iFd, m_sRecv + m_uirBufLen, MAX_RECV_LEN - m_uirBufLen, 0, 
                            (struct sockaddr *) &m_rwHostAddr,  &size);
        if (iRet < 0)
        {
            return false;
        }
        std::cout << "recv data: " <<  m_sRecv  << std::endl;

        DoCmd(iRet);

        memcpy(m_sSnd + m_uiwBufLen, m_sRecv, m_uirBufLen +  iRet);
        m_uiwBufLen += m_uirBufLen +  iRet;
        struct event_base *pEvBase = event_get_base(&m_stEvent);
        if (pEvBase == NULL)
        {
            return false;
        }
        AddEvent(pEvBase,EV_WRITE|EV_PERSIST);
        m_uirBufLen = 0;
        return true;
    }

    bool FdEvent::DoWriteProcess(int iFd)
    {
        if (iFd <=0)
        {
            return false;
        }
        int iRet  = ::sendto(iFd, m_sSnd, m_uiwBufLen, 0, (const struct sockaddr *)&m_rwHostAddr, 
                             sizeof(struct sockaddr_in));
        if (iRet < 0)
        {
            return -1;
        }
        if (iRet != m_uiwBufLen)
        {
            return -2;
        }

        m_uiwBufLen = 0;
        return DelEvent( event_get_base(&m_stEvent), EV_WRITE );
    }

    void FdEvent::WriteCallBack(int iFd, short sEvent, void *pData)
    {
        //send response to client; 
        FdEvent* pEv = (FdEvent*) pData;
        pEv->DoWriteProcess(iFd);
    }


    bool FdEvent::AddEvent(struct event_base *pEvBase, int iEvent)
    {
        if (pEvBase == NULL)
        {
            return false;
        }
        if (iEvent & EV_READ)
        {
            event_set(&m_stEvent, m_iFd, iEvent, ReadCallBack, this);
            event_base_set(pEvBase, &m_stEvent);
            event_add(&m_stEvent, NULL);
        } 
        else if (iEvent & EV_WRITE)
        {
            event_set(&m_stEvent, m_iFd, iEvent, WriteCallBack, this);
            event_base_set(pEvBase, &m_stEvent);
            event_add(&m_stEvent, NULL);
        }
        else 
        {
            //unknown events.
        }
        return true;
    }

    bool FdEvent::DelEvent(struct event_base *pEvBase, int iEvent)
    {
        if (pEvBase == NULL)
        {
            return false;
        }
        //
        int iOrigEvent = event_get_events(&m_stEvent);
        int iNewEvent =  iOrigEvent & (~iEvent);
        if (event_del(&m_stEvent))
        {
            return false;
        }
        if (iNewEvent == 0)
        {
            return true;
        }
        return AddEvent(pEvBase, iNewEvent);
    }
}
