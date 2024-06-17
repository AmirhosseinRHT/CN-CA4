CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = 

all: ser cli

ser: server/main.cpp server/Server.cpp server/Server.hpp
	$(CXX) $(CXXFLAGS) -o ser server/main.cpp server/Server.cpp $(LDFLAGS)

cli: client/Client.cpp client/Client.cpp client/Client.hpp
	$(CXX) $(CXXFLAGS) -o cli client/main.cpp client/Client.cpp $(LDFLAGS)

clean:
	rm -f ser cli