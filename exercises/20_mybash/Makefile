PWD := $(shell pwd)
SUBDIRS := mysed myfile mywc mytrans

all: mybash

$(SUBDIRS):
	@echo "=> 正在编译子模块: $@"
	@mkdir -p $(PWD)/obj/$@
	$(MAKE) -C src/$@ OBJDIR=$(PWD)/obj/$@

# 加入 FORCE，强迫每次都执行主程序编译，防止跳过
mybash 20_mybash: $(SUBDIRS) FORCE
	@echo "=> 正在编译主程序: 20_mybash"
	@mkdir -p $(PWD)/obj/mybash
	$(MAKE) -C src/mybash OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD) TARGET=20_mybash

clean:
	@echo "=> 清理所有编译产物..."
	rm -rf $(PWD)/obj $(PWD)/bin $(PWD)/20_mybash
	@echo "=> 清理完成"

FORCE:

.PHONY: all $(SUBDIRS) mybash 20_mybash clean FORCE