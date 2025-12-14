CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread -O2

INCLUDES := -Iinclude
SRC      := src/thread_pool.cpp

TEST_BASIC       := tests/test_basic.cpp
TEST_CONCURRENCY := tests/test_concurrency.cpp

all:
	@echo "Available targets:"
	@echo "  make test_basic"
	@echo "  make test_concurrency"

test_basic:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(TEST_BASIC) -o test_basic

test_concurrency:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(TEST_CONCURRENCY) -o test_concurrency

test_shutdown:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) tests/test_shutdown.cpp -o test_shutdown

clean:
	rm -f test_basic test_concurrency

.PHONY: all clean test_basic test_concurrency
