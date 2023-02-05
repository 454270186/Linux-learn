# 动态库
1. gcc得到目标文件，得到与位置无关的代码
``` gcc -c -fpic xxx.c xxx.c xxx.c ```

2. gcc得到动态库
``` gcc -shared xxx.o xxx.o -o libxxx.so ```

3. 配置动态库路径(ldd)

4. 运行