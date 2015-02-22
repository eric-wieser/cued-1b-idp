CC = arm-linux-gnueabi-g++
RM = rm -f

TARGET = a.out
CFLAGS = -g -std=gnu++0x -Wall -I/export/teach/1BRobot
LINK_OPTIONS = -lrobot

SRCS := $(wildcard *.cc) $(wildcard utils/*.cc)
OBJS := $(filter-out main.o,$(SRCS:.cc=.o))

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;
 
include .depend

%.out: $(OBJS) %.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINK_OPTIONS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) ./$(TARGET)

%.robot: %.out
	scp $< team@wlan-robot5.private:$@

all: $(TARGET)
