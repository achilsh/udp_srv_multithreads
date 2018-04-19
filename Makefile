INC_SVR_COMM = -I.  -I/usr/local/include
LIB_SVR_COMM = -L/usr/local/lib -levent -lpthread

INC_ALL=$(INC_SVR_COMM) 
LIB_ALL=$(LIB_SVR_COMM)

BINARY = MultiThreadUdpSrv  TestClient

all:$(BINARY)


.SUFFIXES: .o .cpp
CXX = g++
CC = gcc

CXXFLAGS= -g -Wall  -ggdb3

%.o:%.cpp
	$(CXX) $(CFLAGS) -c $^ $(INC_ALL)

MultiThreadUdpSrv:t_main.o  t_udpserver.o  t_event.o  t_threads.o  t_worktask.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB_ALL)

TestClient:t_client.o t_threads.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB_ALL)
clean:
	rm -f *.o *~ $(BINARY)

strip:
	strip $(BINARY)
