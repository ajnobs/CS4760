CC = gcc
CFLAGS = -Wall -g
TARGET_M = master
TARGET_C = bin_adder
OBJ_M = master.o
OBJ_C = bin_adder.o

all: $(TARGET_M) $(TARGET_C)

master: $(TARGET_M)

child: $(TARGET_C)

$(TARGET_M): $(OBJ_M)
	$(CC) $(CFLAGS) -o $@ $(OBJ_M)

$(TARGET_C): $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $(OBJ_C)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET_M) $(TARGET_C) $(OBJ_M) $(OBJ_C) *.log *~
