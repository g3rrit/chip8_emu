TARGET_EXEC ?= chip9

CC = clang

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LDFLAGS ?= -lSDL2

CFLAGS ?= $(INC_FLAGS) -MMD -MP -g -Werror -Wall -Wpedantic -std=c99

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean tidy

clean:
	$(RM) -r $(BUILD_DIR)

tidy:
	clang-format -i --style=file src/*.c src/*.h

-include $(DEPS)

MKDIR_P ?= mkdir -p

