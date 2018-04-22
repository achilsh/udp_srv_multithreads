#include "t_event.hpp"

#define ADDWRITEEVENT (1)

namespace T_UDP
{
    Conn::Conn(int iFd):m_iFd(iFd), m_uirBufLen(0), m_uiwBufLen(0) 
    {
        m_sRecv = new char[MAX_RECV_LEN];
        m_sSnd = new char[MAX_RECV_LEN];
    }

    Conn ::~Conn()
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
        struct event_base *pEvBase = event_get_base(&m_stREvent);
        int iOrigEvent = event_get_events(&m_stREvent);
        DelEvent(pEvBase, &m_stREvent, iOrigEvent);
        DelEvent(pEvBase, &m_stWEvent, iOrigEvent);
    }

    void Conn::ReadCallBack(int iFd, short sEvent, void *pData)  
    {
        std::cout << "test 惊群效果, 单客户端，服务多线程，一次测试打印出现多次,则存在惊群效应" << std::endl;
        Conn* pEv = (Conn*)pData;
        if (pEv == NULL)
        {
            return ;
        }
        pEv->DoReadProcess(iFd);
    }

    bool Conn::DoCmd(int iret)
    {
        //if req other remote node with udp.
        return true;
    }
    bool Conn::DoReadProcess(int iFd)
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
        struct event_base *pEvBase = event_get_base(&m_stREvent);
        if (pEvBase == NULL)
        {
            return false;
        }
        ///////////////////////////////////////////////////////
        m_uirBufLen = 0;

#ifdef ADDWRITEEVENT
        AddEvent(pEvBase, EV_WRITE);
#else
        DoWriteProcess(iFd);
#endif
        return true;
    }

    bool Conn::DoWriteProcess(int iFd)
    {
        if (iFd <=0)
        {
            return false;
        }
        bool bRet = false;
        int iRet  = ::sendto(iFd, m_sSnd, m_uiwBufLen, 0, (const struct sockaddr *)&m_rwHostAddr, 
                             sizeof(struct sockaddr_in));
        if (iRet < 0)
        {
            return bRet;
        }
        if (iRet != m_uiwBufLen)
        {
            return bRet;
        }
    #ifdef  ADDWRITEEVENT
        struct event_base *pEvBase = event_get_base(&m_stWEvent);
        bRet = DelEvent( pEvBase, &m_stWEvent, EV_WRITE );
    #endif
        m_uiwBufLen = 0;
        return bRet;
    }

    void Conn::WriteCallBack(int iFd, short sEvent, void *pData)
    {
        //send response to client; 
        Conn* pEv = (Conn*) pData;
        pEv->DoWriteProcess(iFd);
    }


    bool Conn::AddEvent(struct event_base *pEvBase, int iEvent)
    {
        if (pEvBase == NULL)
        {
            std::cout << "event base is empty" << std::endl;
            return false;
        }
        if (iEvent & EV_READ)
        {
            event_assign(&m_stREvent, pEvBase, m_iFd, iEvent, ReadCallBack, this);
            event_add(&m_stREvent, NULL);
            std::cout << "set EV_READ flag into r_event" << std::endl;
        } 
        else if (iEvent & EV_WRITE)
        {
            event_assign(&m_stWEvent, pEvBase, m_iFd, iEvent, WriteCallBack, this);
            event_add(&m_stWEvent, NULL);
        }
        else 
        {
            //unknown events.
        }
        return true;
    }

    bool Conn::DelEvent(struct event_base *pEvBase, struct event* pEvent, int iEvent)
    {
        if (pEvBase == NULL || pEvent == NULL)
        {
            return false;
        }

        //
        int iOrigEvent = event_get_events(pEvent);
        int iNewEvent =  iOrigEvent & (~iEvent);
        std::cout << "begin to  del write event "  << std::endl;
        if (event_del(pEvent))
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
