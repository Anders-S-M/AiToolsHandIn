CXX=g++
CXXFLAGS= -pipe -O3 -Wall -Wextra -Wpedantic -std=c++11 -I./include -pthread

ludo: clean  build/main.o build/game.o build/move_logic.o 
	$(CXX) $(CXXFLAGS) -o build/ludo build/main.o build/game.o build/move_logic.o

build/main.o: src/main.cpp 
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o 

build/game.o: src/game.cpp include/game.h
	$(CXX) $(CXXFLAGS) -c src/game.cpp -o build/game.o

build/move_logic.o: src/move_logic.cpp include/move_logic.h
	$(CXX) $(CXXFLAGS) -c src/move_logic.cpp -o build/move_logic.o
clean:
	rm -f ./build/*
