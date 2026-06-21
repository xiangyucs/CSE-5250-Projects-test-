all: myexp

logger.o: logger.cpp logger.h CSE5250intrin.h CSE5250intrin.cpp
	g++ -c logger.cpp

CSE5250intrin.o: CSE5250intrin.cpp CSE5250intrin.h logger.cpp logger.h
	g++ -c CSE5250intrin.cpp

myexp: CSE5250intrin.o logger.o main.cpp
	g++ -I../common logger.o CSE5250intrin.o main.cpp -o myexp

clean:
	rm -f *.o myexp *~
