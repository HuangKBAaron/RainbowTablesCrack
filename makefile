# makefile para generator, versión 1 
# uso de variables; reglas predefinidas

CC = gcc -pthread
OBJECTS = devices/reduction.o lib/hashTable3.o lib/spclib.o lib/keyspace.o tools/generator.o 

generate : ui/main_generator.o $(OBJECTS)
	$(CC) -o generate ui/main_generator.o $(OBJECTS) -lm -lcrypto

ui/main_generator.o: tools/generator.h 
lib/hashTable3.o: lib/hashTable3.h
lib/keyspace.o: lib/keyspace.h
lib/spclib.o: lib/spclib.h
devices/reduction.o: devices/reduction.h lib/keyspace.h
tools/generator.o: tools/generator.h devices/reduction.h lib/hashTable3.h lib/spclib.h



.PHONY : clean
    clean :
	-rm generate $(OBJECTS) 
