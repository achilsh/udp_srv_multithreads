/**
 * @file: t_event.hpp
 * @brief: 连接对象的实现文件，把对网络数据的接收、发送等接口抽象成一个连接对象.
 *        提供读写事件的设置，读写事件就绪时回调接口的实现.
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
    class Conn 
    {
     public:
      Conn(int iFd);
      virtual ~Conn();

      /**
       * @brief: AddEvent 
       *  对外提供向该连接对象添加读、写事件接口
       * @param pEvBase
       *     事件集合，就是新事件添加到的地方。一般由主线程去管理.
       * @param iEvent
       *     要添加的新事件.
       *
       * @return 
       */
      bool AddEvent(struct event_base *pEvBase, int iEvent);

      /**
       * @brief: DelEvent 
       *  对外提供  将事件从事件集合删除接口.
       * @param pEvBase
       * @param pEvent
       * @param iEvent
       *
       * @return 
       */
      bool DelEvent(struct event_base *pEvBase, struct event* pEvent, int iEvent);
     private:
      static void ReadCallBack(int iFd, short sEvent, void *pData);
      static void WriteCallBack(int iFd, short sEvent, void *pData);
      bool DoReadProcess(int iFd);
      bool DoWriteProcess(int iFd);
      bool DoCmd(int iret);
     private:
      int m_iFd;
      struct event  m_stREvent; 
      struct event  m_stWEvent;
     
      char *m_sRecv;
      unsigned int  m_uirBufLen;
      
      char *m_sSnd;
      unsigned int m_uiwBufLen;
      struct sockaddr_in m_rwHostAddr;
    };
}
#endif

