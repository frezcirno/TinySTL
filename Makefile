TESTCASES=array_test \
	swisstable_test

CC=clang
CXX=clang++
GTEST=$(shell pkg-config --cflags --libs gtest) -lgtest_main
CFLAGS=-std=c++17 -Wall -g -Iinclude

run: $(TESTCASES)

.PHONY: clean
clean:
	rm -f $(TESTCASES)

.PHONY: $(TESTCASES)
$(TESTCASES): %: test/%.cpp
	$(CXX) -o $@ $(CFLAGS) $< $(GTEST)
	./$@
