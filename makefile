VPATH = src
TARGET = altitude
SRC = main.c altitude.c
OBJ =$(SRC:%.c=%.o)
FIXED ?=
ifeq ($(FIXED) ,1)
DEF = -D FIXED32
else
DEF = 
endif
$(TARGET):$(OBJ)
	gcc $(OBJ) -o $@
all:$(TARGET)
%.o:%.c
	gcc -O2 $(DEF) -c $< -Iinclude -o $@
clean : 
	rm -f $(TARGET) $(OBJ)
