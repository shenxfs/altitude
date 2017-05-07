VPATH = src
TARGET = altitude
SRC = main.c altitude.c
OBJ =$(SRC:%.c=%.o)
$(TARGET):$(OBJ)
	gcc $(OBJ) -o $@
all:$(TARGET)
%.o:%.c
	gcc -O2 -c $< -Iinclude -o $@
clean :
	rm $(TARGET) $(OBJ)
