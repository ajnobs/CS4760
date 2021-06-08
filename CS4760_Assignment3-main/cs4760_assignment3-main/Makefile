CC = gcc
CFLAGS = -Wall -g
TARGET_M = monitor
TARGET_C = consumer
TARGET_P = producer
OBJ_L = statMon.o
OBJ_M = monitor.o
OBJ_C = consumer.o
OBJ_P = producer.o


all: $(TARGET_M) $(TARGET_C) $(TARGET_P)

monitor: $(TARGET_M)

consumer: $(TARGET_C)

producer: $(TARGET_P)

$(TARGET_M): $(OBJ_M) $(OBJ_L)
	$(CC) $(CFLAGS) -o $@ $(OBJ_M) $(OBJ_L)

$(TARGET_C): $(OBJ_C) $(OBJ_L)
	$(CC) $(CFLAGS) -o $@ $(OBJ_C) $(OBJ_L)

$(TARGET_P): $(OBJ_P) $(OBJ_L)
	$(CC) $(CFLAGS) -o $@ $(OBJ_P) $(OBJ_L)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ $(TARGET_L)

.PHONY: clean
clean:
	$(RM) $(TARGET_M) $(TARGET_C) $(TARGET_P) $(TARGET_L) $(OBJ_M) $(OBJ_C) $(OBJ_P) $(OBJ_L) logfile *~
