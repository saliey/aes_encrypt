CPP_SOURCES = $(wildcard aes256e/*.cpp)
HEADERS = $(wildcard aes256e/*.hpp)

OBJS = ${CPP_SOURCES:.cpp=.o}

TARGET = aes256ev

LOCAL_CFLAGS := -Wall
CC = g++
CFLAGS = -g -std=c++11 -Wformat=0


$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
