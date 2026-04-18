PWD := $(shell pwd)
SUBDIRS := mysed myfile mywc mytrans

all: mybash

$(SUBDIRS):
	@echo "=> 正在编译子模块: $@"
	@mkdir -p $(PWD)/obj/$@
	$(MAKE) -C src/$@ OBJDIR=$(PWD)/obj/$@

# 🚨 关键修改：将 TARGET 统一为 mybash，BINDIR 指向当前目录下的 bin
mybash: $(SUBDIRS) FORCE
	@echo "=> 正在编译主程序: mybash"
	@mkdir -p $(PWD)/obj/mybash
	@mkdir -p $(PWD)/bin
	$(MAKE) -C src/mybash OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD)/bin TARGET=mybash

clean:
	@echo "=> 清理所有编译产物..."
	rm -rf $(PWD)/obj $(PWD)/bin
	@echo "=> 清理完成"

FORCE:

.PHONY: all $(SUBDIRS) mybash clean FORCE