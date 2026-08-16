CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

uthread: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o uthread

.PHONY: clean
clean:
	rm -f uthread
