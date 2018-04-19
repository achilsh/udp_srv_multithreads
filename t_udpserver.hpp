#ifndef _t_udpserver_hpp_
#define _t_udpserver_hpp_

#include <vector>
#include <string>

#ifdef __cplusplus
    extern "C" {
#endif 

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 #include <strings.h>
#include <unistd.h>

#ifdef __cplusplus 
    }
#endif

namespace T_UDP
{
    ///
    class WorkerTask;
    class UdpServer 
    {
        public:
         UdpServer(const std::string& sIp, unsigned short uiPort,int threadNums);
         virtual ~UdpServer();
         bool Run();
        private:
         bool DupUDPFd(std::vector<int>& vDupFdList);
         bool Init(); 
         void StartWorkTask();
         void JoinAllTaskWork();
        private:
         std::string m_sIp;
         unsigned short m_uiPort;
         bool m_bInit;
         int m_iUdpFd;
         std::vector<WorkerTask*> m_WTList;
         int m_WTItemNums;
    };
}
#endif
