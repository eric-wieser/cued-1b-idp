CC = arm-linux-gnueabi-g++
RM = rm -f

TARGET = a.out
COMPILE_OPTIONS = -g -std=c++0x -Wall -I/export/teach/1BRobot
LINK_OPTIONS = -lrobot

SRCS := $(wildcard *.cc)
HEADERS := $(wildcard *.h)
OBJS = $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CC) $(COMPILE_OPTIONS) $(OBJS) -o$(TARGET) $(LINK_OPTIONS)

$(OBJS): %.o : $(HEADERS)

%.o: %.cc
	$(CC) $(COMPILE_OPTIONS) -c $< -o $@

clean:
	$(RM) $(OBJS) ./$(TARGET)

deploy: all
	scp a.out team@wlan-robot5.private:fedeggs.arm

all: $(TARGET)