SRCS = hw1.cpp
EXT = .cpp
OBJS = $(SRCS:$(EXT)=.o)
EXEC = hw1
CXX = g++
#CXXFLAGS = -Wall -g -D _DEBUG
CXXFLAGS = -std=c++11

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %$(EXT)
	$(CXX) $(CXXFLAGS) -c $< -ggdb

cleanall: clean cleanexe

clean:
	rm -f $(OBJS)

cleanexe:
	rm -f $(EXEC)