# makefile para generator, versión 1 
# uso de variables; reglas predefinidas

CC = gcc -pthread
OBJECTS = ../devices/sha1.o ../devices/reduction.o ../lib/domain.o ../lib/space.o

testG : testGenerator.o $(OBJECTS)
	$(CC) -o testG testGenerator.o $(OBJECTS) -lm -lssl -lcrypto

testGenerator.o: ../devices/sha1.h ../devices/reduction.h
../lib/domain.o: ../lib/domain.h
../lib/space.o: ../lib/space.h 
../devices/sha1.o: ../devices/sha1.h
../devices/reduction.o: ../devices/reduction.h ../devices/sha1.h ../lib/space.h ../lib/domain.h



.PHONY : clean
    clean :
	-rm testG $(objects) 
