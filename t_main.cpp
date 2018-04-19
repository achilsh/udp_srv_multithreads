#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "t_udpserver.hpp"

using namespace std;
using namespace T_UDP;

class TestMain
{
    public:
     TestMain(int argc, char **argv);
     virtual ~TestMain();
     int main();
    private:
     void Usage(const char *pBinName);
     bool ParseCmd(int argc, char **argv);
     //
    private:
     std::string m_sIp;
     int m_iPort;
     int m_threadNums;
     bool m_bOk;
};

TestMain::TestMain(int argc, char **argv): m_bOk(false)
{
    if (ParseCmd(argc, argv) == false)
    {
        Usage(argv[0]);
        return ;
    }
    m_bOk = true;
}

TestMain::~TestMain()
{ } 

bool TestMain::ParseCmd(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "run param less than 4" << std::endl;
        return false;
    }

    if (strlen(argv[1]) == 0)
    {
        std::cout << "udp ip is empty" << std::endl;
        return false;
    }
    m_sIp.assign(argv[1]);

    int iPort = ::atoi(argv[2]);
    if (iPort <= 0)
    {
        std::cout << "udp srv port less than 0" << std::endl;
        return false;
    }
    m_iPort = iPort;
    
    m_threadNums = ::atoi(argv[3]);
    if (m_threadNums <=0 )
    {
        std::cout << "thread nums less than 0 " << std::endl;
        return false;
    }
    return true;
}

int TestMain::main()
{
     if (false == m_bOk)
     {
         return -1;
     }
     UdpServer udpSrv(m_sIp, m_iPort, m_threadNums);
     if (false == udpSrv.Run())
     {
         std::cout <<  "run udp srv fail" << std::endl;
         return -2;
     }
     return 0;
}

void TestMain::Usage(const char *pBinName)
{
    std::cout << "Usage: " << pBinName  << "  Udpsrv_ip  Udpsrv_port  Udp_threads" << std::endl;
    return ;
}


///////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    TestMain test(argc, argv);
    test.main();
    return 0;
}
