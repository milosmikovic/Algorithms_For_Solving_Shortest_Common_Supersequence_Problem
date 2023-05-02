CXX = g++
CXXFLAGS = -std=c++2a -O3

SCS : SCS.o
	$(CXX) $(CXXFLAGS) -o SCS SCS.o
SCS.o : SCS.cpp
	$(CXX) $(CXXFLAGS) -c SCS.cpp

testInstanceGenerator : testInstanceGenerator.o
	$(CXX) $(CXXFLAGS) -o testInstanceGenerator testInstanceGenerator.o
testInstanceGenerator.o : testInstanceGenerator.cpp
	$(CXX) $(CXXFLAGS) -c testInstanceGenerator.cpp

clean:
	rm -f *.o SCS testInstanceGenerator Results*.txt TestInstances*.txt