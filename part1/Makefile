CXX = g++
CXXFLAGS = -std=c++17 -w
LDFLAGS = 

all: ser cli

ser: server.cpp
	$(CXX) $(CXXFLAGS) -o ser server.cpp $(LDFLAGS)

cli: client.cpp
	$(CXX) $(CXXFLAGS) -o cli client.cpp $(LDFLAGS)

clean:
	rm -f ser cli