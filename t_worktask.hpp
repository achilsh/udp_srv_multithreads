/**
 * @file: t_worktask.hpp
 * @brief: 
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
     WorkerTask(FdEvent* pFdEvent);
     virtual ~WorkerTask(); 
    protected:
     virtual int main();
     virtual int Init();
    private:
     struct event_base *m_PthreadEventBase;
     FdEvent* m_UdpEvent;
};

}

#endif
