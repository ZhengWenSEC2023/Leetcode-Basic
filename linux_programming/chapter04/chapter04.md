# 4.1 文件与文件流
## 4.1.1 文件与流的基本概念
- 文件分类：
  - 根据数据的存储方式
    - 文本文件： ASCII 文件，每个字节存放一个 ASCII 码字符，存储量大，速度慢，便于对字符操作。以 EOF 结束
    - 二进制文件： 数据按其在内存中的存储形式原样存放，存储量小，速度快，便于存放中间结果
  - 根据应用程序对文件的访问方式：
    - 缓冲文件操作：将在用户空间中自动为正在使用的文件开辟内存缓冲区，读写均在缓冲区中进行 （ANSI 标准 I/O 函数使用）
      - 一次性从文件中读出大量数据到缓冲区中，待文件缓冲区满后，或者确实需要更新的时候，再进行系统调用
      - 采用流实现，流的缓冲区分为**全缓冲区，行缓冲区和无缓冲区三种**
    - 非缓冲文件操作：如果需要，只能由用户在自己的程序中为每个文件设定缓冲区 （POSIX 标准系统调用 I/O 函数使用）
      - 对该文件进行任何一次读写操作，都需要使用读写文件的系统调用来处理该操作
## 4.1.2 标准流及流主要功能
- 标准流定义：系统默认为每个进程开启三个文件，分别是标准输入流 (stdin)， 标准输出流 (stdout) 和标准错误流 (stderr)
```C
extern struct _IO_FILE *stdin;      // 标准输入流，默认键盘输入
extern struct _IO_FILE *stdout;     // 标准输出流，默认显示器
extern struct _IO_FILE *stderr;     // 标准错误流，默认显示器

#ifdef __STDC__
#define stdin stdin
#define stdout stdout
#define stderr stderr
#endif
```
- 主要功能：
  - 格式化内容：实现不同输入输出格式的转换
  - 缓冲功能：将数据的读写集中，减少系统调用次数
## 4.1.3 文件流指针
```C
// defination of FILE pointer struct
typedef struct _IO_FILE FILE;
struct _IO_FILE {
    int _flags;
    char* _IO_read_ptr;           // Current read pointer
    char* _IO_read_end;           // End of get area
    char* _IO_read_base;          // Start of putback + get area
    char* _IO_write_base;         // Start of put area
    char* _IO_write_ptr;          // Current put pointer
    char* _IO_write_end;          // End of put area
    char* _IO_buf_base;           // Start of reserve area
    char* _IO_buf_end;            // End of reserve area

    ...
    int _fileno;                  // file descriptor
}
```
## 4.1.4 缓冲区类型
- 全缓冲区：默认大小为 BUFSIZ，具体大小与操作系统有关，在缓冲区满或者调用刷新函数 fflush() 后才进行 I/O 系统调用操作。用于普通磁盘文件的流通常使用全缓冲区访问
- 行缓冲区：当在遇到换行符或缓冲区满时，行缓冲区才刷新。终端使用的是行缓冲区
- 不带缓冲区：标准库不对字符进行缓存，如果用标准 I/O 函数写字符到不带缓冲区的流中，相当于调用系统调用 write()，直接将字符写入对应的文件。stderr 不带缓冲区，使得错误信息可以快速打印出来
- ANSI C 要求缓冲区具有以下特征：
  - 标准输入和标准输出设备：只有不涉及交互作用的设备时，才是全缓冲区的
  - 标准错误输出设备：标准出错没有全缓冲区
## 4.1.5 指定流缓冲区
- setbuf() 函数：将指定流的缓冲区指向给定的具有 BUFSIZ 大小的缓冲区，或关闭指定流的缓冲区
- setvbuf() 函数：给指定流指定特定大小和类型的缓冲区
  - 如果指定不带缓冲区的流，则忽略 buf 和 size 参数
  - 如果指定全缓冲区或行缓冲区，则 buf 和 size 可以有选择地指定一个缓冲区及其长度（大于等于 128 字节）
  - 如果制定该流时带缓冲区的，而 buf == NULL ，则标准 I/O 库将为该流分配
    - 适当长度是指由文件属性数据结构 (struct stat) 的成员 st_blksize 所指定的值
    - 如果系统不能为该流决定此值（如：流涉及一个设备或者一个管道），则分配长度为 BUFSIZ 的缓冲区
# 4.2 ANSI C 文件 I/O 操作
## 4.2.1 打开关闭文件
```C
// first arg is path to file, second arg is mode for opening the file
extern FILE* fopen(__const char* __restrict __filename, __const char* __restrict __modes);
// write data in buffer into file, then close file. If success, return 0, else return -1 and set global errno
extern int fclose(FILE *__stream);
// close all fp in current process. If success, return 0, else return EOF and set global errno
extern int fcloseall();
// write bytes in buffer to files and then clear buffer. If success, return 0, else return EOF and set global errno
extern int fflush(FILE *__stream);
```