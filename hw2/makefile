SRCS = server.cpp file_system.cpp
SRCS2 = client.cpp
EXT = .cpp
OBJS = $(SRCS:$(EXT)=.o)
OBJS2 = $(SRCS2:$(EXT)=.o)
EXEC = server
EXEC2 = client
CXX = g++
#CXXFLAGS = -Wall -g -D _DEBUG
CXXFLAGS = -std=c++11 -pthread

all: $(EXEC) $(EXEC2)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(EXEC2): $(OBJS2)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS2)

%.o: %$(EXT)
	$(CXX) $(CXXFLAGS) -c $< -ggdb

cleanall: clean cleanexe

clean:
	rm -f $(OBJS)

cleanexe:
	rm -f $(EXEC)