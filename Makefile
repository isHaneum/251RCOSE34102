CC      := gcc
CFLAGS  := -std=c11 -Wall -Iinclude
SRCS    := src/process.c src/ready_queue.c src/waiting_queue.c src/scheduler.c src/gantt.c src/main.c
TARGET  := scheduler

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(SRCS)
    $(CC) $(CFLAGS) $^ -o $@

clean:
    rm -f $(TARGET)