# makefile para generator, versión 1 
# uso de variables; reglas predefinidas

CC = gcc -pthread
OBJECTS = devices/sha1.o devices/reduction.o lib/hashTable3.o lib/spclib.o lib/domain.o lib/space.o tools/generator.o 

generate : ui/main_generator.o $(OBJECTS)
	$(CC) -o generate ui/main_generator.o $(OBJECTS) -lm

ui/main_generator.o: tools/generator.h 
lib/hashTable3.o: lib/hashTable3.h
lib/domain.o: lib/domain.h
lib/space.o: lib/space.h 
lib/spclib.o: lib/spclib.h devices/sha1.h
devices/sha1.o: devices/sha1.h
devices/reduction.o: devices/reduction.h devices/sha1.h lib/space.h lib/domain.h lib/space.h

tools/generator.o: tools/generator.h devices/reduction.h devices/sha1.h lib/hashTable3.h lib/spclib.h lib/space.h



.PHONY : clean
    clean :
	-rm generate $(objects) 
