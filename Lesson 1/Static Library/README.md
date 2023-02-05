# 静态库
1. 将源文件编译成目标文件
``` gcc -c xxx.c xxx.c xxx.c ```

2. 将目标文件打包成库文件
-I 后为头文件路径 -L 后为库路径
``` gcc main.c -o app -I ./include/ -l calc -L ./lib/ ```

