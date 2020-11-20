a: bin/test.o bin/matrix.o
	g++ bin/test.o bin/matrix.o -o a

bin/test.o: test/test.cpp
	g++ -o bin/test.o -c test/test.cpp

bin/matrix.o: src/matrix.cpp src/matrix.h
	g++ -o bin/matrix.o  -c src/matrix.cpp 

clean:
	rm bin/*.o