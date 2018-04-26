CPP_SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

OBJS = ${CPP_SOURCES:.cpp=.o}

TARGET = aesv

LOCAL_CFLAGS := -Wall -O2
CC = g++
CFLAGS = -g -std=c++11 -Wformat=0


$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
