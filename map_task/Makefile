CXX = g++
CFLAGS = -O2 -Wall -Wextra -Wpedantic

SRCS = map_ram.cpp
OBJS =
TARGET = map_ram.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(SRCS)

run: all
	python3 measure_ram_consumption.py

clean:
	rm -f $(TARGET) memory+time.html
