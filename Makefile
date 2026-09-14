CC = gcc
#Wall:warning for all 
# -g: add the debug information
CFLAGS = -Wall -g -I./hal -I./device -I./common
LDFLAGS = $(shell pkg-config --libs libgpiod)
release:CFLAGS = -Wall -O2
TARGET = myapp
# wildcard: find matching files and assign the results to a Makefile variable 
APP_SRCS = $(wildcard app/*.c)
HAL_SRCS = $(wildcard hal/*.c)
DEVICE_SRCS = $(wildcard device/*.c)
SRCS = $(APP_SRCS) $(HAL_SRCS) $(DEVICE_SRCS)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
release:clean	$(TARGET)
	@echo "Release build complete\n"
clean:
	rm -f $(TARGET)	$(OBJS) $(DEPS)
