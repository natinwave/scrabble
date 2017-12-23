all: the_world

the_world: CharArr.o Scrabble.o Trie.o main.o
	g++ CharArr.o Scrabble.o Trie.o main.o -o the_world

CharArr.o: CharArr.cpp
	g++ -c CharArr.cpp
	
Scrabble.o: Scrabble.cpp
	g++ -c Scrabble.cpp
	
Trie.o: Trie.cpp
	g++ -c Trie.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o the_world
