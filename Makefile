# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -pthread -O2

# Source files
SRCS = main.c stack.c

# Header files
HEADERS = stack.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = program

# Default target
all: $(TARGET)

# Linking the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compiling the source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
