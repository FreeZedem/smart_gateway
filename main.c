#include "hello.h"
int main(int argc, char const *argv[])
{
    hello();
    return 0;
}




/*
预处理
    去掉空格注释, 处理哪些预处理指令,得到预处理后的源代码
    gcc -E hello.c -o hello.i
    gcc -E main.c -o main.i
编译
    把预处理后的代码, 生产汇编代码
    gcc -S hello.i -o  hello.s
    gcc -S main.i -o  main.s
汇编
    把汇编代码, 转换成二进制文件(目标文件)
    gcc -c hello.s -o hello.o
    gcc -c main.s -o main.o
链接
    把多个目标文件, 一些库文件, 链接成可执行文件
    动态链接
        gcc hello.o main.o -o hello
    
    静态链接
        gcc -static hello.o main.o -o hello 

makefile:
    定义一系列的规则

一个规则
目标: 依赖
    命令
    ...

*/
