# src/mybash/Makefile

CC = gcc
CFLAGS = -Wall -g -fsanitize=address -I../../include

# 可执行文件名
TARGET := 20_mybash
# 最终输出到 BINDIR（比如 ./bin/）
BINDIR ?= .
# 中间文件目录（比如 ./obj/src/mybash/）
OBJDIR ?= ./obj
LINKLIBS ?= -L../../obj/mysed -L../../obj/mywc -L../../obj/mytrans -L../../obj/myfile
LDLIBS := -lmysed -lmywc -lmytrans -lmyfile -lelf
# 源文件
SRCS := main.c
OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))
LDFLAGS = -fsanitize=address

# 最终二进制路径
TARGET_PATH := $(BINDIR)/$(TARGET)

all: $(TARGET_PATH)

$(TARGET_PATH): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LINKLIBS) $(LDLIBS)
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)/*.o $(TARGET_PATH)