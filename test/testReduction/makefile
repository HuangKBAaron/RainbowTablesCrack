# makefile para test reduction, versión 1 
# uso de variables; reglas predefinidas

CC = gcc
OBJECTS = ../../devices/reduction.o ../../lib/keyspace.o ../../lib/util.o

test : testReduction.o $(OBJECTS)
	$(CC) -o test testReduction.o $(OBJECTS) -lm

testReduction.o: ../../devices/reduction.h
../../lib/util.o: ../../lib/util.h
../../lib/keyspace.o: ../../lib/keyspace.h ../../lib/util.h
../../devices/reduction.o: ../../devices/reduction.h ../../lib/keyspace.h



.PHONY : clean
    clean :
	-rm test $(OBJECTS) 
