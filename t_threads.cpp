#include "t_threads.hpp"

namespace T_UDP
{
    PthreadBase::PthreadBase() :m_iPid(-1), m_bRun(false)
    {
    }

    PthreadBase::~PthreadBase()
    {
    }
    void PthreadBase::Start()
    {
        int iRet = pthread_create(&m_iPid, NULL, Entry, this);
        if (iRet != 0)
        {
            return ;
        }
    }

    void PthreadBase::Stop()
    {
        m_bRun = false;
    }

    void* PthreadBase::Entry(void* pData)
    {
        PthreadBase* pThis =  (PthreadBase*)pData;
        if (pThis == NULL)
        {
            return NULL;
        }

        if (pThis->Init() < 0)
        {
            return NULL;
        }

        pThis->SetRun();
        while(pThis->IsRun())
        {
            pThis->main();
        }
    }

    void PthreadBase::JoinWork()
    {
       pthread_join(m_iPid, NULL); 
    }

}
