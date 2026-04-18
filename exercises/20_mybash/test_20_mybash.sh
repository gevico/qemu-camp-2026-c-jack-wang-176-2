#!/bin/bash

# 1. 安全起见，临时编译两个必定符合 ELF 要求的测试文件
echo "int main(){}" > /tmp/dummy_pie.c
gcc /tmp/dummy_pie.c -fPIE -pie -o /tmp/dummy_pie
gcc /tmp/dummy_pie.c -c -o /tmp/dummy_rel.o

# 2. 自动生成一个给你的 mybash 执行的命令脚本
cat << 'EOF' > test_cmds.mybash
myfile /tmp/dummy_pie
myfile /tmp/dummy_rel.o
mysed s/mac/linux/ "mac is opensource. unix is free os."
mytrans test_input.txt
mywc test_input.txt
EOF

# 3. 自动生成一个包含所有“必考单词”的测试文本
cat << 'EOF' > test_input.txt
code empowers individuals to be creators in
and and and and and and and and and and and
the the the the the the the the the the
skilled just just just
EOF

# 4. 找到你编译好的 mybash 并运行它！
# (根据你的 Makefile，它应该在 ./bin/mybash)
if [ -f "./bin/mybash" ]; then
    ./bin/mybash test_cmds.mybash
else
    echo "未找到 ./bin/mybash，请检查你的 Makefile 编译路径！"
    exit 1
fi

# 5. 清理临时文件（好习惯）
rm -f /tmp/dummy_pie* /tmp/dummy_rel.o test_cmds.mybash test_input.txt