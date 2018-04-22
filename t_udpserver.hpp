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

#include "t_pools.hpp"

namespace T_UDP
{
    class WorkerTask;
    class Conn;
    class UdpServer 
    {
        public:
         UdpServer(const std::string& sIp, unsigned short uiPort,int threadNums);
         virtual ~UdpServer();

         /**
          * @brief: Run 
          *  启动udp服务,该服务是多个fd被多个线程分摊接收和发送数据.
          *  线程由线程池管理.
          * @return true: succ, false: error
          */
         bool Run();
        private:

         /**
          * @brief: DupUDPFd 
          * 将本地bind()的fd 复制成多个fd,用于多线程一起接收数据.
          *
          * @param vDupFdList
          *       被复制的fd列表
          *
          * @return succ: true, err: false
          */
         bool DupUDPFd(std::vector<int>& vDupFdList);
         bool Init(); 
        
         /**
          * @brief: StartWorkTask 
          *   创建并启动多个线程任务
          */
         void StartWorkTask();
        private:
         std::string m_sIp;
         unsigned short m_uiPort;
         bool m_bInit;
         int m_iUdpFd;
         PthreadPools<WorkerTask>* m_pWTList; //任务池/线程池
         std::vector<Conn*> m_pEventList;     //连接池
         int m_WTItemNums;  //线程池中线程个数
    };
}
#endif
