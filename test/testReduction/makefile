# makefile para test reduction, versión 1 
# uso de variables; reglas predefinidas

CC = gcc -pthread
OBJECTS = ../../devices/reduction.o ../../lib/domain.o ../../lib/space.o

test : testReduction.o $(OBJECTS)
	$(CC) -o test testReduction.o $(OBJECTS) -lm

testReduction.o: ../../devices/reduction.h
../../lib/domain.o: ../../lib/domain.h
../../lib/space.o: ../../lib/space.h 
../../devices/reduction.o: ../../devices/reduction.h ../../lib/space.h ../../lib/domain.h



.PHONY : clean
    clean :
	-rm test $(objects) 
