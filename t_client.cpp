#include "t_client.hpp"

namespace T_CLIENT
{
    TestOneClient::TestOneClient(const std::string& sRemoteIp, int iRemotePort, int iSendTimes)
        :m_sRemoteIp(sRemoteIp), m_iRemotePort(iRemotePort), m_iTestNums(iSendTimes), m_iFd(-1)
    {
    }

    TestOneClient::~TestOneClient()
    {
    }

    int  TestOneClient::Init()
    {
        m_iFd = socket( AF_INET,  SOCK_DGRAM , 0 );
        if (m_iFd < 0)
        {
            std::cout << "sockect() call fail" << std::endl;
            return -1;
        }

        m_stDestAddr.sin_family = AF_INET;
        m_stDestAddr.sin_addr.s_addr = inet_addr(const_cast<char*> (m_sRemoteIp.c_str()));
        m_stDestAddr.sin_port=htons(m_iRemotePort);
        return 0;
    }
    //
    int TestOneClient::main()
    {
        /***
        if (false == Init())
        {
            Stop();
            std::cout << "init TestOneClient fail, so stop it " << std::endl;
            return false;
        }
        ***/

        for (int i = 0; i < m_iTestNums; ++i)
        {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            snprintf(buf, sizeof(buf), "%d'client is test!", i);
            int iRet = ::sendto(m_iFd, buf, strlen(buf)+1, 0, (const struct sockaddr*)&m_stDestAddr, sizeof(m_stDestAddr));
            if (iRet < 0)
            {
                std::cout << "err send pid: " << GetThreadId()  <<" , send data to srv fail, " << strerror(errno) << std::endl;
                break;
            }
            std::cout <<"succ send pid: " << GetThreadId()  << ",send data: " << buf << std::endl;

            memset(buf,0,sizeof(buf));
            struct sockaddr_in rcvNode;
            socklen_t size  = sizeof(rcvNode);
            iRet = ::recvfrom(m_iFd, buf, sizeof(buf), 0, (struct sockaddr*)&rcvNode, &size);
            if (iRet < 0)
            {
                std::cout << "err recv pid: " << GetThreadId()  <<", recvfrom udp srv response fail " << std::endl;
                break;
            }
            std::cout << "succ recv pid: " << GetThreadId()  << "recv data: " << buf << std::endl;
        }
        this->Stop();
        return 0;
    }

    /***********************************************************************
    *
    * 
    **********************************************************************/
    TestClient::TestClient(int argc, char **argv) :m_bOk(false), m_iClientNums(0),m_iReqNums(0) 
    {
        if (ParseCmd(argc, argv) == false)
        {
            Usage(argv[0]);
            return ;
        }
        m_bOk = true;
    }

    TestClient::~TestClient()
    {
    }

    bool TestClient::ParseCmd(int argc, char **argv)
    {
        if (argc <5)
        {   
            std::cout << "client run param less than 5" << std::endl;
            return false;
        }

        if (strlen(argv[1]) == 0)
        {
            std::cout << "remote ip is empty" << std::endl;
            return false;
        }
        m_sRemoteIp.assign(argv[1]);

        int iPort = ::atoi(argv[2]);
        if (iPort <= 0)
        {
            std::cout << "remote port less than 0" << std::endl;
            return false;
        }
        m_iRemotePort = iPort;
        
        m_iClientNums = ::atoi(argv[3]);
        if (m_iClientNums <= 0)
        {
            std::cout << "client nums less than 0 " << std::endl;
            return false;
        }

        m_iReqNums = ::atoi(argv[4]);
        if  (m_iReqNums <= 0)
        {
            std::cout << "req nums for each client less than 0 " << std::endl;
            return false;
        }
        return true;
    }

    void TestClient::Usage(const char *pBinName)
    {
        std::cout <<"Usage: " << pBinName << "  remoteIp  " << "   remotePort  " << 
            " clientnums  " << " reqnum_in_one_client" <<std::endl; 
    }

    //
    int TestClient::main()
    {
        std::vector<TestOneClient*> vTestClientNode;
        for (int i = 0;  i < m_iClientNums; ++i)
        {
            TestOneClient* pNode = new TestOneClient(m_sRemoteIp, m_iRemotePort, m_iReqNums);
            vTestClientNode.push_back(pNode);
        }

        for (std::vector<TestOneClient*>::iterator it = vTestClientNode.begin(); it != vTestClientNode.end(); ++it)
        {
            (*it)->Start();
        }

        for (std::vector<TestOneClient*>::iterator it = vTestClientNode.begin(); it != vTestClientNode.end(); )
        {
            (*it)->JoinWork();
            delete (*it);
            it = vTestClientNode.erase(it);
        }
        return 0;
    }

}


//////////////////////////////////////////////
/////////////////////////////////////////////
int main(int argc, char **argv)
{
    T_CLIENT::TestClient client(argc, argv);
    client.main();
    
    return 0;
}
