calc : calc.cpp
	g++ calc.cpp -o calc

test : testcase.cpp calc.cpp
	g++ testcase.cpp -o test

.PHONY: clean

clean:
	-rm calc test
