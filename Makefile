CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
OPENMP_FLAGS = -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp
else
OPENMP_FLAGS = -fopenmp
endif

OPTIMIZE_FLAGS = -O3 -ftree-vectorize -march=native -mtune=native $(OPENMP_FLAGS)

BASE_SOURCES = core/filter.cpp tests/benchmark.cpp

.PHONY: all clean

all: benchmark_o0 benchmark_o3 libfilter.so

libfilter.so: core/filter.cpp core/filter.h
	$(CXX) $(CXXFLAGS) -O0 $(OPENMP_FLAGS) -fPIC -shared core/filter.cpp -o libfilter.so

benchmark_o0: $(BASE_SOURCES) core/filter.h
	$(CXX) $(CXXFLAGS) -O0 $(BASE_SOURCES) -o benchmark_o0

benchmark_o3: $(BASE_SOURCES) core/filter.h
	$(CXX) $(CXXFLAGS) $(OPTIMIZE_FLAGS) $(BASE_SOURCES) -o benchmark_o3

clean:
	rm -f benchmark_o0 benchmark_o3 libfilter.so
