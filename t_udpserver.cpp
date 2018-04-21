#include "t_udpserver.hpp"
#include "t_threads.hpp"
#include "t_event.hpp"
#include "t_worktask.hpp"

namespace T_UDP
{
    ////////////////////////////////////////////////////////////
    UdpServer::UdpServer(const std::string& sIp, unsigned short uiPort, int threadNums)
        : m_sIp(sIp), m_uiPort(uiPort), m_bInit(false), m_iUdpFd(-1), m_WTItemNums(threadNums)
    {
        m_bInit = Init();
    }

    UdpServer::~UdpServer() 
    {
        if (m_iUdpFd > 0)
        {
            close(m_iUdpFd); m_iUdpFd = -1;
        }
        for (std::vector<WorkerTask*>::iterator it = m_WTList.begin(); it != m_WTList.end(); ++it)
        {
            if (*it != NULL)
            {
                delete  (*it);
            }
        }
        //
        m_WTList.clear();
        //
        m_bInit = false;
    }

    bool UdpServer::Init()
    {
        if (m_sIp.empty() || m_uiPort <= 0)
        {
            return false;
        }
        m_iUdpFd = socket( AF_INET,  SOCK_DGRAM | SOCK_NONBLOCK,  0);
        if (m_iUdpFd < 0)
        {
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port   = htons(m_uiPort);
        serverAddr.sin_addr.s_addr = inet_addr(m_sIp.c_str());
        //bzero(&serverAddr.sin_zero, sizeof(serverAddr.sin_zero));
        int iRet = bind(m_iUdpFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (iRet != 0)
        {
            std::cout << "bind fail, ip: " << m_sIp <<",port: " << m_uiPort << std::endl;
            return false;
        }
        m_bInit = true;

        return true;
    }

    /////////////////////////
    bool UdpServer::DupUDPFd(std::vector<int>& vDupFdList)
    {
        if (m_iUdpFd <= 0)
        {
            return false;
        }
        for (int iIndex = 0; iIndex < m_WTItemNums; ++iIndex)
        {
            int x = iIndex ? dup(m_iUdpFd): m_iUdpFd;
            vDupFdList.push_back(x);
            std::cout << "new fd: " << x << std::endl;
        }
        return true;
    }
    void UdpServer::StartWorkTask()
    {
        for(std::vector<WorkerTask*>::iterator it = m_WTList.begin();
            it != m_WTList.end(); ++it)
        {
            if ((*it) == NULL)
            {
                continue;
            }
            (*it)->Start();
        }
    }

    void UdpServer::JoinAllTaskWork()
    {
        for(std::vector<WorkerTask*>::iterator it = m_WTList.begin();
            it != m_WTList.end(); ++it)
        {
            if (*it == NULL)
            {
                continue;
            }
            (*it)->JoinWork();
        }
    }

    bool UdpServer::Run() 
    {
        if ( !m_bInit )
        {
            return false;
        }
        
        std::vector<int> iFdList;
        // 
        /**<  有效解决多线程操作同一fd时，读事件到达后多个线程被唤醒，而结果只有一个
         * 线程可以读到数据的问题,即惊群效应.*/
        DupUDPFd(iFdList);
        for (int iIndex = 0; iIndex < m_WTItemNums; ++iIndex)
        {
            FdEvent * pEvent = new FdEvent(iFdList.at(iIndex));
            WorkerTask* wtUdp = new WorkerTask(pEvent);
            m_WTList.push_back(wtUdp);
        }
        StartWorkTask();
        JoinAllTaskWork();
        return true;
    }
}
