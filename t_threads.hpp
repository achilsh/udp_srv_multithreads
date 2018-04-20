/**
 * @file: t_threads.hpp
 * @brief: 
 * @author:  wusheng Hu
 * @version: v0x0001
 * @date: 2018-04-18
 */
#ifndef _T_THREADS_HPP_
#define _T_THREADS_HPP_ 

#ifdef __cplusplus
    extern "C" {
#endif

#include <pthread.h>

#ifdef __cplusplus
    }
#endif



namespace T_UDP
{

class PthreadBase
{
    public:
     PthreadBase();
     virtual ~PthreadBase();
     void Start();
     void Stop();
     void JoinWork();
     pthread_t GetThreadId()
     {
         return m_iPid;
     }
    protected:
     static void* Entry(void* pData);
     virtual int main() = 0;
     virtual int Init() = 0;
     void SetRun()
     {
         m_bRun = true;
     }
     bool IsRun() 
     {
         return m_bRun;
     }

     pthread_t m_iPid;
     bool m_bRun;
};

///////
}
#endif
