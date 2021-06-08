CC = gcc
CFLAGS = -Wall -g
TARGET_O = oss
TARGET_U = user
OBJ_O = oss.o
OBJ_U = user.o
UTIL = utility.o
T+O = $(TARGET_O) $(TARGET_U) $(OBJ_O) $(OBJ_U) $(UTIL)
JUNK = *.log *~


all: $(TARGET_O) $(TARGET_U)

.PHONY: clean
clean:
	$(RM) $(T+O) $(JUNK)

.PHONY: refresh
refresh:
	$(RM) $(T+O) $(JUNK)
	$(CC) $(CFLAGS) -c utility.c -o $(UTIL)
	$(CC) $(CFLAGS) -c oss.c -o $(OBJ_O)
	$(CC) $(CFLAGS) $(OBJ_O) $(UTIL) -o $(TARGET_O)
	$(CC) $(CFLAGS) -c user.c -o $(OBJ_U)
	$(CC) $(CFLAGS) $(OBJ_U) $(UTIL) -o $(TARGET_U)


$(TARGET_O): $(UTIL) $(OBJ_O)
	$(CC) $(CFLAGS) $(OBJ_O) $(UTIL) -o $@

$(TARGET_U): $(UTIL) $(OBJ_U)
	$(CC) $(CFLAGS) $(OBJ_U) $(UTIL) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<
