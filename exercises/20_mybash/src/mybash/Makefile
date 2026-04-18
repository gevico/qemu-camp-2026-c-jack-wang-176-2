CC = gcc
CFLAGS = -Wall -I../../include

# 🚨 关键修改 1：根据报错信息，可执行文件名必须是 mybash
TARGET := mybash
# 🚨 关键修改 2：输出到练习题根目录下的 bin 文件夹
BINDIR := ../../bin
OBJDIR := ./obj

LINKLIBS := -L../../obj/mysed -L../../obj/mywc -L../../obj/mytrans -L../../obj/myfile
LDLIBS := -lmysed -lmywc -lmytrans -lmyfile -lelf

SRCS := main.c
OBJS := $(OBJDIR)/main.o

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIBS) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(BINDIR)/$(TARGET)