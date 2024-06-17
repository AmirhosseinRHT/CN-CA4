CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = 

all: ser cli

ser: server/main.cpp server/Server.cpp server/Server.hpp defs.hpp defs.cpp
	$(CXX) $(CXXFLAGS) -o ser defs.cpp server/main.cpp server/Server.cpp $(LDFLAGS)

cli: client/Client.cpp client/Client.cpp client/Client.hpp defs.hpp defs.cpp
	$(CXX) $(CXXFLAGS) -o cli defs.cpp client/main.cpp client/Client.cpp $(LDFLAGS)

clean:
	rm -f ser cli