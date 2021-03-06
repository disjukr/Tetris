CC = g++ -std=c++0x
SRCS = main.cc Tetris.cc Randomizer.cc \
	Time.cc Screen.cc Console.cc Keyboard.cc
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)
TARGET = Tetris

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f *.o *.d $(TARGET)

%.o: %.cc
	$(CC) -c $< -o $@
	$(CC) -M $< > $(@:.o=.d)

-include $(DEPS)
