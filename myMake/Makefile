# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -g

# Executable name
EXEC = mymake2

# Object files
OBJS = graph_operations.o graph_utils.o mymake2.o

# Header files
HEADERS = graph.h

# Default target
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Individual source files
graph_operations.o: graph_operations.c $(HEADERS)
	$(CC) $(CFLAGS) -c graph_operations.c

graph_utils.o: graph_utils.c $(HEADERS)
	$(CC) $(CFLAGS) -c graph_utils.c

mymake2.o: mymake2.c $(HEADERS)
	$(CC) $(CFLAGS) -c mymake2.c

# Phony target for cleaning
.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
