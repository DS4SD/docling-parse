
CC = /opt/local/bin/g++-mp-4.9

FLAGS = -g3

LIBS = -lm

INCLUDE = -I./ \
	-I../../css/ \
	-I../../html/ \

all: test

main.o: main.cpp
	time $(CC) $(INCLUDE) $(FLAGS) -c main.cpp

test: main.o 
	time $(CC) $(INCLUDE) $(FLAGS) main.o -o test $(LIBS) 

clean:
	rm main.o
	rm test
	rm *~
