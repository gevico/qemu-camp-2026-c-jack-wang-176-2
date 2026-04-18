CC = gcc
# 已经确认代码健康，关闭 ASan 以免污染标准输出，影响测试框架的字符串匹配
CFLAGS = -Wall -I../../include
LDFLAGS = 

TARGET := 20_mybash
BINDIR ?= .
OBJDIR ?= ./obj
LINKLIBS ?= -L../../obj/mysed -L../../obj/mywc -L../../obj/mytrans -L../../obj/myfile
LDLIBS := -lmysed -lmywc -lmytrans -lmyfile -lelf

SRCS := main.c
OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))

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