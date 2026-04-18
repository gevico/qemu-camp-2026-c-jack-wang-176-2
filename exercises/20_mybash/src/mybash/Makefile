CC = gcc
CFLAGS = -Wall -I../../include
LDFLAGS = 

# 🚨 名字和路径必须严格对齐测试框架预期
TARGET := 20_mybash
BINDIR := ../../
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