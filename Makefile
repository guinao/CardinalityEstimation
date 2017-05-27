all : main

src = MinHashSketch

objects = sketch_utils.o

main : g++ -o a.out $(src)/main.cpp -std=c++14 -O3 -Wall


