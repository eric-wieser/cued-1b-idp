CC = arm-linux-gnueabi-g++
RM = rm -f

TARGET = a.out
CFLAGS = -g -std=gnu++0x -Wall -I/export/teach/1BRobot
LINK_OPTIONS = -lrobot

SRCS := $(wildcard *.cc)
OBJS := $(wildcard *.o)

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;
 
include .depend

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o$(TARGET) $(LINK_OPTIONS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) ./$(TARGET)

deploy: all
	scp a.out team@wlan-robot5.private:fedeggs.arm

all: $(TARGET)
