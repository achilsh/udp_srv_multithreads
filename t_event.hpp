/**
 * @file: t_event.hpp
 * @brief: 
 * @author:  wusheng Hu
 * @version: v0x0001
 * @date: 2018-04-18
 */

#ifndef _T_EVENT_HPP_
#define _T_EVENT_HPP_ 

#include <iostream>

#ifdef __cplusplus 
    extern "C" {
#endif 

#include <unistd.h>
#include <string.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event.h>

#ifdef __cplusplus 
    }
#endif

namespace T_UDP
{
    #define MAX_RECV_LEN (1024)
    class FdEvent
    {
     public:
      FdEvent(int iFd);
      virtual ~FdEvent();
      static void ReadCallBack(int iFd, short sEvent, void *pData);
      static void WriteCallBack(int iFd, short sEvent, void *pData);
      bool AddEvent(struct event_base *pEvBase, int iEvent);
      bool DelEvent(struct event_base *pEvBase, int iEvent);
     private:
      bool DoReadProcess(int iFd);
      bool DoWriteProcess(int iFd);
      bool DoCmd(int iret);
     private:
      int m_iFd;
      struct event m_stEvent; 
     
      char *m_sRecv;
      unsigned int  m_uirBufLen;
      
      char *m_sSnd;
      unsigned int m_uiwBufLen;
      struct sockaddr_in m_rwHostAddr;
    };
}
#endif

