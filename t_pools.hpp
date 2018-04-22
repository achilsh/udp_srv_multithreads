/**
 * @file: t_pools.hpp
 * @brief: 线程池容器实现文件.
 * @author:  wusheng Hu
 * @version: v0x0001
 * @date: 2018-04-22
 */

#ifndef __T_THREAD_POOLS_HPP__
#define __T_THREAD_POOLS_HPP__

#include <pthread.h>
#include <vector>
#include <iostream>

namespace T_UDP
{

    template <typename T>
    class PthreadPools
    {
     public:
      PthreadPools(const int iPoolSize);
      virtual ~PthreadPools();
      
      /**
       * @brief: StartAllThreads 
       * 启动线程池中所有线程.
       * @return 
       */
      bool StartAllThreads();
      T* GetIndexThread(const int iIndex); 
     private:

      /**
       * @brief: WaitThreadRegiste 
       *   等待线程池所有线程都创建完成.
       */
      void WaitThreadRegiste();

      /**
       * @brief: DeleteThreads 
       *  等待线程池中所有线程都退出。
       *  并删除线程对象资源
       */
      void DeleteThreads();
      pthread_mutex_t m_InitLock;
      pthread_cond_t  m_InitCond;
      int m_iReadyThreadNums;

      std::vector<T*> m_vPoolList; 
      int m_iPoolSize;
    };

    ///implement for pthreadpools
    template<typename T>
    PthreadPools<T>::PthreadPools(const int iPoolSize): m_iPoolSize (iPoolSize)
    {
        pthread_mutex_init(&m_InitLock, NULL);
        pthread_cond_init(&m_InitCond, NULL);
        m_iReadyThreadNums = 0;
    }

    template<typename T>
    PthreadPools<T>::~PthreadPools ()
    {
        DeleteThreads();
    }

    template<typename T>
    bool PthreadPools<T>::StartAllThreads()
    {
        if (m_iPoolSize <= 0)
        {
            return false;
        }
        
        for (int i = 0; i < m_iPoolSize; ++i)
        {
            T* pThread = new T(&m_InitLock, &m_InitCond, &m_iReadyThreadNums);
            m_vPoolList.push_back(pThread);
        }

        for (int i = 0; i < m_iPoolSize; ++i)
        {
            m_vPoolList.at(i)->Start();
        }

        WaitThreadRegiste();
        std::cout << "all threads start done ..." << std::endl;
    }
    
    template<typename T>
    void PthreadPools<T>::WaitThreadRegiste()
    { 
        pthread_mutex_lock(&m_InitLock);
        while( m_iReadyThreadNums < m_iPoolSize)
        {
            pthread_cond_wait(&m_InitCond, &m_InitLock);
        }
        pthread_mutex_unlock(&m_InitLock);
    }

    template<typename T>
    void PthreadPools<T>::DeleteThreads()
    {
        if (m_iPoolSize <=0)
        {
            return ;
        }

        for (int i = 0; i < m_iPoolSize; ++i)
        {
            m_vPoolList.at(i)->JoinWork();
            delete m_vPoolList[i]; 
        }
        m_vPoolList.clear();
    }

    template<typename T>
    T* PthreadPools<T>::GetIndexThread(const int iIndex) 
    {
        if (iIndex + 1 > m_iPoolSize)
        {
            return NULL;
        }
        if (iIndex < 0)
        {
            return NULL;
        }
        return  m_vPoolList.at(iIndex);;
    }
}

#endif
