CC := gcc
RM := rm
MV := mv
ROOT_DIR := .
LIB_DIR := $(ROOT_DIR)/lib
SRC_DIR := $(ROOT_DIR)/src

SRCEXT := c
BUILDEXT := o

SOURCES := $(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst %.$(SRCEXT),%.$(BUILDEXT),$(SOURCES))
TARGET := xrainbow_crack
INSTALL_PATH := /usr/bin/

CFLAGS += -g -Wall -DDEBUG -I$(LIB_DIR) -I$(SRC_DIR) -I$(ROOT_DIR)
LDFLAGS += -pthread -lm -lcrypto

all: ${TARGET}

$(TARGET): $(OBJECTS)
	@echo " $(CC) -o $@ $(OBJECTS) $(LDFLAGS)"; $(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	@echo " $(CC) -c $(CFLAGS) -o $@ $<"; $(CC) -c $(CFLAGS) -o $@ $<

clean:
	@echo " Cleaning ...";
	@echo " $(RM) -rf $(OBJECTS) $(TARGET)"; $(RM) -rf $(OBJECTS) $(TARGET)

install:
	@echo " Installing ...";
	@echo " $(MV) $(TARGET) $(INSTALL_PATH)"; $(MV) $(TARGET) $(INSTALL_PATH)

uninstall:
	@echo " Uninstalling ...";
	@echo " $(RM) -rf $(INSTALL_PATH)$(TARGET)"; $(RM) -rf $(INSTALL_PATH)$(TARGET)