RCC = arm-linux-gnueabi-g++
CC = g++

RM = rm -f

TARGET = a.out
CFLAGS = -g -std=gnu++0x -Wall -I/export/teach/1BRobot
LINK_OPTIONS = -lrobot

SRCS := $(wildcard *.cc) $(wildcard utils/*.cc) $(wildcard dev/*.cc)
OBJSRCS := $(filter-out main.cc,$(SRCS))

OBJS := $(OBJSRCS:.cc=.o)
OBJS_ARM := $(OBJSRCS:.cc=.arm.o)

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;

include .depend

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@
%.wifi: $(OBJS) %.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINK_OPTIONS)

%.arm.o: %.cc
	$(RCC) $(CFLAGS) -c $< -o $@
%.arm.robot: $(OBJS_ARM) %.o
	$(RCC) $(CFLAGS) $^ -o $@ $(LINK_OPTIONS)
%.robot: %.arm.robot
	scp $< team@wlan-robot5.private:$@
	echo "ssh team@wlan-robot5.private ./$@" > $@
	chmod +x $@

clean:
	$(RM) $(OBJS) ./$(TARGET)


all: $(TARGET)
