all: make
	
make: main.cpp main_seq.cpp
	g++ -std=c++14 -pthread main.cpp -Og -o parallel
	g++ -std=c++14 main_seq.cpp -o parallel_seq
.PHONY: clean
clean:
	rm -rf parallel parallel_seq 