CXX = g++
CXXFLAGS = -std=c++17 -O3

SCS : SCS.o
	$(CXX) $(CXXFLAGS) -o SCS SCS.o
SCS.o : SCS.cpp
	$(CXX) $(CXXFLAGS) -c SCS.cpp

clean:
	rm -f *.o SCS Results*.txt