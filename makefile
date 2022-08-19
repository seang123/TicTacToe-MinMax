CC = g++ 

CFLAGS = -g -Wall -std=c++11


TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) -O2 $(CFLAGS) -o $(TARGET) $(TARGET).cpp
