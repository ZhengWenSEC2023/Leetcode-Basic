# 3.1 Linux 程序存储结构与进程结构
## 3.1.1 文件结构
- ELF文件：
  - 存储时分为**代码区(text)**，**数据区(data)**，**未初始化数据区(bss)**
  - 代码区： text segment, 存放CPU执行的机器指令，可共享，只读，这样对于频繁使用的代码，在内存中只存在一份
    - 指令包括操作码和操作对象（对象地址引用）
    - 立即数直接包含在代码里
    - 局部数据在运行时分配堆栈空间，再引用该数据的地址
    - bss区和数据区：在代码中同样引用该数据的地址
    - 例如：函数内部**const常量**和**字符串字面值**都会存在这个地方
  - 全局初始化数据区/静态数据区：initialized data segment/data segment，数据段
    - 包含被明确初始化的**全局变量**，已经初始化的**静态变量**
    - **被 const 声明的变量**以及**字符串常量**在**代码段**中申请空间
    - 例如：**函数内外已经初始化好的 static 字段**，**函数外已经初始化好的非static字段**
  - 未初始化数据区：BSS segment (Block Started by Symbol)
    - 包含**未初始化全局变量**和**未初始化静态变量**
    - 例如：**函数外未初始化的static or non static字段**，以及**函数内部未初始化的static字段**
  - 关于未初始化，应该是未以规定值之外的值初始化，比如static long a = 0, 仍然存在 bss 段，static long b = 10, 会存在 data 段
  - 一般函数内非static、const修饰常量，为运行时申请内存空间，不占用ELF磁盘存储空间
## 3.1.2 进程结构
- Linux系统下，将ELF文件加载到内存中，加载时只有一个进程，后续可变成多个进程
- 进程包括在内存中申请的空间，代码（加载的代码段，数据段，BSS段），堆，栈以及内容提供的内核进程信息结构 task_struct，打开的文件，上下文信息以及挂起的信号等
  - 代码区：加载到内存的位置由加载器决定，有操作系统支持时不用关心，移植系统时需要自己规定
  - 数据区：程序运行之初即申请空间，程序结束才释放，生命周期为整个程序运行时间
  - BSS区：同数据区
  - 栈区：由编译器自动分配释放，存放函数参数值、返回值、局部变量等
  - 堆区：由程序员分配和释放，如果程序员不释放，会由OS在程序结束后释放
- 程序分区考虑如下：
  - 代码段和数据段分开，运行时便于分开加载
  - 代码是依次执行的，由处理器的PC指针依次读入，且可以被多个程序共享，运行时可能有多次使用，与数据段放在一起会造成空间的浪费
  - 临时数据以及需要再次使用的代码在运行时放入栈中，生命周期短，可以提高资源利用率
  - 堆区可以由程序员分配和释放，提高灵活性
## 3.1.3 C变量及函数存储类型
- 变量声明格式
```
存储类型  类型修饰符  数据类型  变量名
```
- 其中：
  - 存储类型用来表示变量存储的位置，常见的有 auto, extern, register, static, 即在运行该变量在哪一个段分配内存空间
  - 类型修饰符：修饰变量的存储及表现方式：包括 long, short, signed, unsigned, void, const, volatile 等
  - 数据类型说明数据占用的内存空间
- 函数声明格式：
```
存储类型  返回数据类型  函数名（参数列表）
```
- 其中
  - 存储类型用来标识函数的作用域，主要有 extern 和 static 两个。
- auto 修饰的变量只能定义在函数体内
- extern 修饰的变量定义在其他文件的数据段中
- register 关键字只能用于局部变量，且只能是整型和字符型，说明该变量长期被使用，存放在CPU寄存器中
- static 关键字表示变量为静态变量，存在数据区，静态局部变量作用域在自身所处的 {} 中，静态全局变量作用域在整个文件中。生命周期为整个程序
- 头文件中一般不建议定义全局变量，易造成重定义问题
- 全局变量会让静态变量只能被初始化依次，如果未初始化，将被初始化为默认值
- 常量数据：字符串常量存在代码区，生存期为整个程序运行时间，而作用域为当前文件
```C
// test in single file
#include <stdio.h>
char *a = "hello";
void test(void) {
    printf("In subfunction:\n");
    char *c = "hello";
    if (a == c) {
        printf("Yes, a == c\n");
    }
    else {
        printf("No, a != c\n");
    }
}

int main(void) {
    printf("In function main:\n");
    char *b = "hello";
    char *d = "hello1";
    if (a == b) {
        printf("Yes, a == b\n"); 
    }
    else {
        printf("No, a != b\n");
    }
    test();
    if (a == d) {
        printf("Yes, a == d\n");
    }
    else {
        printf("No, a != d\n");
    }
    return 0;
}

~$ ./a.out
In function main:
Yes, a == b
In subfunction:
Yes, a == c
No, a != d
```
```C
// test in multiple files
// test.c
#include <stdio.h>
char *a = "hello";
int main() {
    char *b = "hello";
    printf("In main: ");
    if (a == b) {
        printf("Yes, a == b\n");
    }
    else {
        printf("No, a != b\n");
    }
    printf("In subfunction: ");
    test();
    return 0;
}

// file.c
#include <stdio.h>
extern char *a;
void test(void) {
    char *c = "hello";
    if (a == c) {
        printf("Yes, a == c\n");
    }
    else {
        printf("No, a != c\n");
    }
}
```
```sh
gcc -o const_test_two_files file.c test.c
In main: Yes, a == b
In subfunction: No, a != c
```
## 3.1.4 栈和堆的区别
- 管理方式不同
  - 栈：程序运行时由操作系统自动分配和释放，程序员不必关心
  - 堆：由malloc()函数分配的内存块，手动控制
- 空间大小不同：
  - 栈：向低地址扩展，是连续的内存区域，栈顶的地址和栈的最大容量是系统预先规定好的。
  - 堆：向高地址扩展，是不连续的内存区域，用链表来存储剩余的空闲地址，且由低地址向高地址扩展
- 产生碎片不同：
  - 堆：频繁 malloc() 和 free() 会造成内存空间不连续
  - 栈：一定是连续的空间
- 增长方向不同
  - 堆：向上
  - 栈：向下
- 分配方式不同
  - 栈：由 alloca() 函数完成
  - 堆：由 malloc(), free() 完成
- 分配效率不同
  - 栈：系统提供，分配专门的寄存器存放栈的地址，压栈出栈都有专门的指令
  - 堆：C函数库提供，机制复杂，效率较低
# 3.2 ANSI C 动态内存管理
## 3.2.1 内存分配的基本方式
- 静态分配：编译器在编译程序源代码时分配，例如全局变量和静态变量
- 动态分配：程序执行时调用 malloc()
- 主要区别：
  - 静态对象是有名字的对象，可以直接对其进行操作，动态变量没有名字，需要通过指针间接操作
  - 静态对象分配由编译器自动处理，动态对象分配由 malloc() 和 free() 处理
- 悬挂指针：free() 释放内存后，指针很可能仍然指向之前的内存空间，但很有可能已经被其他程序占用
- realloc() 函数：重新定义已经分配了的空间的大小。如果之前的空间后面有连续的空闲空间，则原指针地址不变；否则会在堆的下一个空闲区分配一个空间，并释放原空间
  - 亲测：若访问原空间，可以打印原来的数值，但前几个有问题，应该是被 free 了，但按理来说，访问该空间的行为是未定义的
- calloc() 函数：在 malloc() 的基础上，会将空间默认设置为 0 (用 '\0' 进行空间的初始化)
- alloca() 函数：在栈中分配 size 个字节的空间，调用该函数的函数返回时会自动释放 alloca() 的空间，失败会返回 NULL
- memcpy() 函数：将 src 地址的 n 个字节复制到 dst ，成功后返回 dst 的首指针，不对可能出现的多余目的空间进行处理
  - strncpy() 和 memcpy() 用法相似，但strncpy() 传入参数指针为 char* 类型， memcpy() 传入为 void* 类型
- memchr() 函数：在一段内存空间中查找某个字符第一次出现的位置
- memcmp() 函数：比较两个空间的前 n 个字节是否相等
```C
// defination of each function
// malloc()
void *malloc (size_t __size) __THROW __attribute_malloc__;
// calloc()
void *calloc (size_t __nmemb, size_t __size);
// alloca()
void *alloca(size_t __size) __THROW
// memcpy()
void *memcpy(void *__restrict __dest, __const void* __restrict __src, size_t __n);
// bcopy()
void bcopy(__const void *__src, void *__dest, size_t __n);
// memmove()
void *memmove(void *__dest, __const void *__src, size_t n);
// memset()
void *memset(void *__s, int __c, size_t __n) __THROW __nonnull((1));
// memchr()
void *memchr(__const void *__s, int __c, size_t __n);
// memcmp()
int memcmp(__const void *__s1, __const void *__s2, size_t n);
```
- pmap 命令可以用来查看进程内存占用情况
```bash
pmap -x $pid
# Address    Kbytes    RSS           Dirty    Mode      Mapping
# 虚拟地址    大小       常驻内存大小    状态信息   权限      加载的文件
# Mapping 列, [ anon ] 为堆， [ stack ] 为栈
```
# 3.4 Linux 进程环境及系统限制
## 3.4.1 进程与命令行选项及参数
- getopt() 函数，解析命令行参数，顺序任意，用 optstring 参数来指定命令行参数格式，每执行一次，该函数会返回查找到的命令行输入的参数字符，并更新全局变量，出错返回-1
  - 单个字符：表示该选项
  - 单个字符后接一个冒号：表示该选项后必须跟一个参数，参数必须紧跟在选项后，或以空格隔开
  - 单个字符后接两个冒号：表示该选项后可以跟一个参数，也可以不跟参数。参数必须紧跟在字符后面，不能以空格隔开
  - 用三个全局变量跟踪目前所用的命令行参数：
    - optarg: 指向当前选项参数的指针
    - optind: 再次调用 getopt() 时的下一个 argv 指针的索引
    - optopt: 存储不可知或错误选项，如果 optstring 中或选项缺少必要的参数时，该选项存储在 optopt 中，getopt() 返回 '?'
    - opterr: 是否向 stderr 输出错误信息， opterr = 0 不输出
  - 程序运行结束后，如果发现不合法的参数，会将不合法的参数全部移到 argv[] 的末尾，optind 以此为起始值，遍历到 argc 即可得到全部不合法的输入参数
- getlongopt() 函数：解析命令行参数，可以带对应的长命令行参数
## 3.4.2 进程与环境变量
- main() 函数的第三个变量会标识当前进程的环境变量列表
```C
// get value of env with name __name
extern char* getenv(__const char* __name);
// add new env with format string "NAME=VALUE"
extern int putenv(char *__string);
// set env with name=value, __replace means force overwrite (!0) or not (0)
extern int setenv(__const char *__name, __const char *__value, int __replace);
// delete env with name
extern int unsetenv(__const char *__name);
```