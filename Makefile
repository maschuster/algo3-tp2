all: src/main.cpp src/algoritmos.cpp
	mkdir -p build 
	g++ -o build/main src/main.cpp -O2 -std=c++14

clean:
	rm build/main