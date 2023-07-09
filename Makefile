CFLAGS=-g -std=c++17 -Wall -pedantic

all: build-test run-test

build-test: main.cpp
	$(CXX) $(CFLAGS) $? $(LDFLAGS) -o strings

run-test:
	./strings

clean:
	rm strings
