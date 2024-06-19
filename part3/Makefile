CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = 

all: ser cli

ser: server/main.cpp server/Server.cpp server/Server.hpp defines/defs.hpp defines/defs.cpp
	$(CXX) $(CXXFLAGS) -o ser defines/defs.cpp server/main.cpp server/Server.cpp $(LDFLAGS)

cli: client/main.cpp client/Client.cpp client/Client.cpp client/Client.hpp client/Sender.hpp client/Sender.cpp client/Reciever.cpp client/Reciever.hpp defines/defs.hpp defines/defs.cpp
	$(CXX) $(CXXFLAGS) -o cli defines/defs.cpp client/main.cpp client/Client.cpp client/Sender.cpp client/Reciever.cpp $(LDFLAGS)

clean:
	rm -f ser cli