/**
 * @file: t_worktask.hpp
 * @brief: 具体线程任务实现文件， 该线程任务是关注于网络io事件. 接收连接向其注册
 * 各种网络事件.是个主线程。事件发生后,具体数据的read/write则有相关连接对象来操作。
 * @author:  wusheng Hu
 * @version: v0x0001
 * @date: 2018-04-19
 */

#ifndef _T_WORKTASK_HPP_
#define _T_WORKTASK_HPP_

#include "t_event.hpp"
#include "t_threads.hpp"

#include <iostream>

namespace T_UDP
{
///////
class WorkerTask: public PthreadBase
{
    public:
     WorkerTask();
     WorkerTask(pthread_mutex_t* pInitLock, pthread_cond_t* pInitCond, int* pInitNums);
     virtual ~WorkerTask(); 

     /**
      * @brief: AddRConn 
      *  把读数据的连接对象注册到主线程上.该连接对象已经提供读写事件到达时的回
      *  调接口。主线程只负责调度
      * @param pEvent
      */
     void AddRConn(Conn* pEvent);
    protected:
     virtual int main();
     virtual int Init();
    private:
     void TmrCallBackDoing();
     static void TmrCallback(int fd, short event, void *argv);

     struct event_base *m_PthreadEventBase;
     struct event m_Tmrevent;
     int m_Tms;
};

}

#endif
