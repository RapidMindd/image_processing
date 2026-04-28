CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic

BASE_SOURCES = core/filter.cpp tests/benchmark.cpp

.PHONY: all clean

all: benchmark_o0 benchmark_o2 libfilter.so

libfilter.so: core/filter.cpp core/filter.h
	$(CXX) $(CXXFLAGS) -O0 -fPIC -shared core/filter.cpp -o libfilter.so

benchmark_o0: $(BASE_SOURCES) core/filter.h
	$(CXX) $(CXXFLAGS) -O0 $(BASE_SOURCES) -o benchmark_o0

benchmark_o2: $(BASE_SOURCES) core/filter.h
	$(CXX) $(CXXFLAGS) -O2 $(BASE_SOURCES) -o benchmark_o2

clean:
	rm -f benchmark_o0 benchmark_o2 libfilter.so
