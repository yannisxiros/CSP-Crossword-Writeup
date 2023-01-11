TARGET_EXEC := final_program

BUILD_DIR := ./target
OBJ_DIR := ./target/objs
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')

OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC = gcc
CFLAGS = $(INC_FLAGS) -g3 -Wall -Werror -Wextra

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)