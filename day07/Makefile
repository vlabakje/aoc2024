CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++23

all: one two

one: one.cpp
	$(CXX) $(CXXFLAGS) -o one one.cpp

two: two.cpp
	$(CXX) $(CXXFLAGS) -o two two.cpp

clean:
	rm -f one
