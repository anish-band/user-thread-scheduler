CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I include

all: uthread bench_uthread bench_pthread

uthread: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o uthread

bench_uthread: bench/bench_uthread.cpp
	$(CXX) $(CXXFLAGS) bench/bench_uthread.cpp -o bench_uthread

bench_pthread: bench/bench_pthread.cpp
	$(CXX) $(CXXFLAGS) bench/bench_pthread.cpp -o bench_pthread -pthread

.PHONY: clean
clean:
	rm -f uthread bench_uthread bench_pthread
