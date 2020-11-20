all: src/main.cpp src/algoritmos.cpp
	mkdir -p build 
	g++ -o build/main src/main.cpp -O2 -std=c++14

test:
	cd notebooks && python3.7 tests.py

clean:
	rm build/main