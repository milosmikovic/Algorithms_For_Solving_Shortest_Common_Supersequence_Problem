CXX = g++
CXXFLAGS = -std=c++17 -O3

Shortest_common_supersequence : Shortest_common_supersequence.o
	$(CXX) $(CXXFLAGS) -o Shortest_common_supersequence Shortest_common_supersequence.o
Shortest_common_supersequence.o : Shortest_common_supersequence.cpp
	$(CXX) $(CXXFLAGS) -c Shortest_common_supersequence.cpp

clean:
	rm -f *.o Shortest_common_supersequence