# makefile
.SUFFIXES : .cpp .o


OBJECTS = network.o netlauncher.o main.o packetqueue.o tcpserver.o
SRCS = $(OBJECTS:.o=.cpp)


CC = g++
CXXFLAGS = -g -std=c++11 -Wall -pedantic -pthread
TARGET = tcpcamserver


$(TARGET) : $(OBJECTS)
	$(CC) -pthread -o $@  $(OBJECTS)

.cpp.o :
	$(CC) $(CXXFLAGS) -c $< -o $@

clean : 
	$(RM) -f $(OBJECTS) $(TARGET) core

