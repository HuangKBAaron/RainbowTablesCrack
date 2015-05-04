CC = gcc -pthread
OBJECTS = src/charset.o

xrainbow_crack: src/x_rainbow_crack.o $(OBJECTS)
	$(CC) -o xrainbow_crack src/x_rainbow_crack.o $(OBJECTS) -lm

x_rainbow_crack.o: src/x_rainbow_crack.h src/charset.h
src/charset.o: src/charset.h



install:
	mv xrainbow_crack /usr/bin/

uninstall:
	rm /usr/bin/xrainbow_crack
	rm $(OBJECTS)

clean:
	-rm xrainbow_crack $(OBJECTS)