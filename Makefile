RCC = arm-linux-gnueabi-g++
CC = g++

RM = rm -f

CFLAGS = -g -std=gnu++0x -Wall -I/export/teach/1BRobot
LINK_OPTIONS = -lrobot

SRCS := $(wildcard *.cc) $(wildcard utils/*.cc) $(wildcard dev/*.cc)
OBJSRCS := $(filter-out main.cc,$(SRCS))

OBJS := $(OBJSRCS:.cc=.o)
OBJS_ARM := $(OBJSRCS:.cc=.arm.o)

# mark o files as secondary to prevent recompilation
.SECONDARY: $(OBJS) $(OBJS_ARM)

all: main.wifi main.robot

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;

include .depend

%.arm.o: %.cc
	$(RCC) $(CFLAGS) -c $< -o $@
%.arm.robot: $(OBJS_ARM) %.arm.o
	$(RCC) $(CFLAGS) $^ -o $@ $(LINK_OPTIONS)
%.robot: %.arm.robot
	echo $(OBJS_ARM)
	scp $< team@wlan-robot5.private:$@
	echo "ssh team@wlan-robot5.private ./$@" > $@
	chmod +x $@

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@
%.wifi: $(OBJS) %.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINK_OPTIONS)


tests/drivetest.wifi: LINK_OPTIONS += -lncurses

clean:
	$(RM) $(OBJS) $(OBJS_ARM)