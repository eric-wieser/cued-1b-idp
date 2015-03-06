RCC = arm-linux-gnueabi-g++
CC = g++

RM = rm -f

override CFLAGS += -g -std=gnu++0x -Wall -I/export/teach/1BRobot -I.
override LDFLAGS += -lrobot

DIRS := . util dev routines
SRCS := $(sort $(foreach d, $(DIRS), $(wildcard $(d)/*.cc)))
OBJSRCS := $(filter-out ./main.cc,$(SRCS))

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
	$(RCC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
%.robot: %.arm.robot
	echo $(OBJS_ARM)
	scp $< team@wlan-robot5.private:$@
	echo "ssh team@wlan-robot5.private ./$@" > $@
	chmod +x $@

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@
%.wifi: $(OBJS) %.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)



clean:
	$(RM) $(OBJS) $(OBJS_ARM) ./html tests/*.wifi

docs:
	doxygen

tests/dev/t_drive.wifi: LDFLAGS += -lncurses
tests/t_remote.wifi: LDFLAGS += -lncurses
tests/t_robotdrive.wifi: LDFLAGS += -lncurses

egg_stats.cc: eggdata.csv process_eggdata.py
	python process_eggdata.py
