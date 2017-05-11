VPATH = src include
TARGET ?= altitude
SRC = main.c altitude.c
OBJ =$(SRC:%.c=%.o)
CC ?= gcc 
ifeq ($(CROSS),1)
CC = i686-w64-mingw32-gcc
TARGET = altitude.exe
endif
FIXED ?=
ifeq ($(FIXED) ,1)
DEF = -D FIXED32
else
DEF = 
endif
$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@
all:$(TARGET)
%.o:%.c
	$(CC) -O2 $(DEF) -Wall -c $< -Iinclude -o $@
clean : 
	rm -f $(TARGET) $(OBJ) *.exe
altitude.o:altitude.c altitude.h
main.o:main.c altitude.h
