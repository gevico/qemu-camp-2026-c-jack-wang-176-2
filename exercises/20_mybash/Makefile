PWD := $(shell pwd)

SUBDIRS := \
	mysed \
	myfile \
	mywc \
	mytrans

# 默认目标改为 20_mybash
all: 20_mybash

$(SUBDIRS):
	@echo "=> 正在编译子模块: $@"
	@mkdir -p $(PWD)/obj/$@
	$(MAKE) -C src/$@ OBJDIR=$(PWD)/obj/$@

# 目标改为 20_mybash
20_mybash: $(SUBDIRS)
	@echo "=> 正在编译主程序: 20_mybash"
	@mkdir -p $(PWD)/obj/mybash
	# 强行把可执行文件名设为 20_mybash，输出路径设为当前根目录
	$(MAKE) -C src/mybash OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD) TARGET=20_mybash

clean:
	@echo "=> 清理所有编译产物..."
	rm -rf $(PWD)/obj $(PWD)/bin $(PWD)/20_mybash
	@echo "=> 清理完成"

.PHONY: all $(SUBDIRS) 20_mybash clean