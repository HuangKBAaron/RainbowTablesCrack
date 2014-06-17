# makefile para test reduction, versión 1 
# uso de variables; reglas predefinidas

CC = gcc
OBJECTS = ../../lib/util.o

test : testUtil.o $(OBJECTS)
	$(CC) -o test testUtil.o $(OBJECTS)

testUtil.o: ../../lib/util.h
../../lib/util.o: ../../lib/util.h



.PHONY : clean
    clean :
	-rm test $(OBJECTS) 
