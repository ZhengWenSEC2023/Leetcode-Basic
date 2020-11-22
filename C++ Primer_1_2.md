# 第一章

- 获得程序返回值：
    ```bash
    echo $?
    ```

- endl作用：
    endl为一个**操作符**(manipulator)，效果为结束当前行，并将与设备相关联的缓冲区（buffer）中的内容刷新到设备中。缓冲刷新的操作可以保证到目前为止程序产生的所有输出都真正写入输出流中，而不是仅仅停留在内存中等待写入。

- 用 ``` while (std::cin >> val)  ```做循环终止条件：
    cin 为一个 istream对象，当用作循环条件的时候，检测流的状态。如果流有效，检测成功。如果遇到文件结束符（eof），或者遇到一个无效输入，istream状态会变成无效，从而条件为假。（unix系统中，命令行中文件结束符是 ctrl + D）

- 文件重定向
    ```bash
    ./a.out <infile >outfile
    ```
    从infile中读取输入字符串，结果通过标准输出，打印到outfile中。

- 缓冲区
    是一个存储区域。 IO设施通常将输入和输出保存在缓冲区中，读写缓冲区的动作与程序无关。可以显式地刷新输出缓冲，以便强制将缓冲区中的数据写入输出设备。默认情况下，读取cin会刷新cout, 程序非正常终止也会刷新cout。

# 第二章
- 内置类型的机器实现：
    大多计算机用2的整数次幂个比特作为块来处理内存，可寻址的最小内存块称为“字节”（byte），存储的基本单元称为“字”，通常由几个字节组成。一般**字=4~8字节**
- 基础类型转换：**切勿混用有符号和无符号类型(size_t)**
    - 无符号类型被赋值超出他的表示范围的值的时候，结果是初始值对无符号类型表示数值总数取模 (unsigned char = -1 //实际为 unsigned char = (1-1 % 256))
    - 有符号类型被赋值超出他的表示范围的值的时候，结果**未定义**，程序可能继续工作，可能崩溃，也可能产生垃圾数据。
    - 非布尔值用作条件判断的时候，会自动转换成布尔值
        ```C++
        if (42)
        -->
        if (1)
        ```
    - 负数signed被赋值为unsigned的时候，unsigned会自动取模。
    - unsigned 用在条件表达式中，如果表达式中含有负值，负值会转换为unsigned. (s.size() < n) (int n = -1;)
        ```C++
        unsigned u = 10;
        int i = -42;
        cout << u + i << endl; //输出4294967264
        ```
        ```C++
        unsigned u = 10; unsigned i = -42;
        cout << u + i << endl; //输出4294967264
        ```
        ```C++
        for (unsigned i = 10; i >= 0; i++) {
            cout << i ;
        }//死循环
        ```
    - 字面值常量：
      - 整形：
        - 整形默认字面值是十进制，默认有符号数，int, long, long long中最小的
        - 八进制(0开头)和十六进制(0x或0X开头)或为有符号或无符号。
        - short没有对应字面值
        - 字面值不会是负数，比如-42是将42取负
      - 浮点型：
        - 小数或者科学计数法 3.14, 0e0, 0E1
        - 默认为double
      - 字符和字符串
        - 字符字面值用‘’表示，‘A’
        - 字符串字面值用“”表示，是C风格的数组，const char[], 末尾编译器自动加一个结束符‘\0’
        - 如果两个字符串字面值仅由空格，缩进和换行符分割，实际上是一个整体。
        - 泛化转义序列： \ + 后面带着的至多3个八进制数字，或者\x跟着后面所有数字。
            ```
            “c1\12300”  “c2\x12341”
               ^~~~        ^~~~~~~ 
            ```
    - 初始化
      - 初始化 != 赋值
          初始化是在创建对象的时候给定初值，赋值是擦去原来存的值，给定一个新值。
      - 列表初始化
          用{}对来初始化
          ```C++
          long double ld = 3.14
          int a{ld}, b{ld}; //报错 (g++没有)
          int c(ld), d(ld); //可行
          ```
      - 默认初始化
          内置变量类型默认初始化取决于定义位置：
          - 函数内部未定义且不可访问
          - 函数外部默认为0；
    - 声明和定义
      - C++的分离式编译支持在别的文件定义变量而在此使用
      - 一个文件如果想使用别处定义的名字，必须包含对其的声明，用extern而不可定义。
      - 声明规定变量的名字和类型
      - 定义规定变量的名字和类型，并申请存储空间，也可能为变量赋值。
      ```C++
      extern int i //声明
      int j        //声明并定义j
      extern double pi = 3,14 //定义
      ```
      - 函数体内部如果试图初始化extern标记的变量，会报错。
      - 变量的定义必须出现且只能出现在一个文件中，其他用到该变量的文件必须声明，且不可重复定义。
    - 引用：对象的别名
      - 必须初始化，与被引变量始终绑定在一起。
      - 引用不是对象，所以不能定义引用的引用。
      - 引用的初值必须是一个对象，必须有实际内存空间，不能与字面值或者表达式的计算结果绑定在一起。
      - 除开两种特殊情况(P55, P534)其他情况下，引用必须与被引类型相同。
    - 指针：存放对象地址的对象
      - 指针本身为一个对象，允许赋值和拷贝，而且可以先后指向几个不同的对象。
      - 定义时无需赋初值。
      - 引用不是对象，不能定义指向引用的指针。
      - 除开两种特殊情况(P56, P534)其他情况下，指针必须与指向对象类型相同。
      - 指针值的状态：
        1. 指向一个对象
        2. 指向紧邻对象所占空间的下一个位置
        3. 空指针，没有指向任何对象
        4. 无效指针，除开上述三种情况的指针
      - 指针不能用int赋值
      - **指针相等用作条件判断的时候，一个指针指向当前对象，另一个指针指向当前对象的下一个地址，也可能会出现指针相等的情况**
      - **void*** 指针
        - 可以存放任意对象的地址，但不清楚内存中具体是什么对象，所以只能用来比较，作为函数的输入输出，或者赋值，但不能访问所在的对象，也不能 ++ -- (分编译器)
        - 将void* 赋值给其他指针时，应该先将void*进行强制转换
  - 对对象定义的复合类型需要从右往左阅读
    ```C++
    int i = 42;
    int *p;
    int *&r = p  //从右往左：&r是个引用，* 表示对指针的引用，int 表示指针的类型
    // int &*r = p // 非法：引用不存在地址
    ```
  - const 关键字
    - const关键字用来表示不能被改变的常量，必须初始化，否则报错
    - 可以正常拷贝，初始化，类型转换，就是不能改变本身的值
    - 默认情况下，const定义的变量仅在本文件中有效
    - 若某个文件中存在一个常量需要文件间共享，可以在定义和声明的时候加上extern关键字
    ```C++
    // file 1
    extern const int c = 10;
    // file 2
    extern const int c;     // 10
    ```
    - 对常量的引用（常量引用）
    ```C++
    const int c = 1024;
    const int &r = c;
    ```
      - 常量引用不需要被引表达式一定是一个对象，只要表达式可以转换成引用类型即可
      ```C++
      int i = 42;
      const int &ri = i     // 正确
      const int &r2 = 42    // 正确，此时编译器创建了一个 const int temp = 42, 之后r2引用temp
      // int &r4 = 42       // 错误
      // ri = 10;           // 错误
      i = 10；              // 正确
      ```
    - 指向常量的指针
    ```C++
    const double pi = 3.14
    double *ptr = pi        // 错误，普通指针不能指向常量
    const double *cptr = pi // 正确
    *cptr = 42;             // 错误，常量指针不能赋值。
    ```
      - 指向常量的指针可以指向一个非常量对象（**指针的类型必须与其所指的对象类型一致的例外之一**）
      - const 指针：必须初始化，将const标识符放在 * 号后面。*const
      - 指针本身是个常量并不意味着不能通过指针修改指向对象的值，只是指针的指向不可变，取决于指向的对象是否有const修饰
      ```C++
      int errNumb = 0;
      int *const curErr = &errNumb;  // 可以通过指针修改 errNumb 的值 (顶层const)
      const double pi = 3.14;
      const double *const cpip = &pi; //不可通过指针修改 pi 的值      (顶层 + 底层const)
      // 从右往左读, const表示是个常量， * 表示是个指针，const double 表示指向的类型，这是个指向const double的const指针
      const double *pip              // pip 是一个指向const double 的指针 (底层const)
      pip = pi;
      ```
      - 赋值时，顶层const不影响赋值的结果，但必须具有同样的底层const资格。非const可以转const说的是顶层const。
        ```C++
        int i = 0;
        int *const p1 = &i;   // 不能改变 p1 的值，顶层const
        const int ci = 42;    // 不能改变 ci 的值，顶层const
        const int *p2 = &ci;  // 允许改变 p2 的值，底层const
        const int *const p3 = p2; // 靠右为顶层，靠左为底层
        const int &r = ci;    // 用于声明引用的都是底层const

        i = ci;               // ci 为顶层const, 可以拷贝
        p2 = p3;              // p3 和 p2 指向的类型一样，都是const int, 可以拷贝

        // int *p = p3;          // 错误，p3 有底层 const 的定义，而 p 没有，底层const资格不同
        // int *p = p2;       // 错误，底层const不同，相当于常量转非常量。
        p2 = p3               // 正确，底层const资格相同
        p2 = &i;              // 正确，int * 可以转换为 const int *;  （非常量转常量）
        // int &r = ci;       // 错误，普通的int& 不能绑定到 const int 上 
        const int &r2 = i;    // 正确，const int& 可以绑定到一个普通int上
        ```
    - constexpr 关键字
      - 常量表达式，指值不会改变且**在编译过程中就可得到计算结果的表达式**.
      - 字面值，用常量表达式初始化的const对象等
      - 允许将变量声明为constexpr类型，以便**由编译器来验证变两个值是否是一个常量表达式**
      ```C++
      constexpr int mf = 20;
      constexpr int limit = mf + 1;
      constexpr int sz = size() // 只有size是一个constexpr函数的时候，本句才编译通过
      ```
    - constexpr 函数
      - 返回类型以及所有形参类型都是字面值类型，而且函数体中有且只有一条return语句
      ```C++
      constexpr int new_sz() { return 42; }         // 是常量表达式函数
      constexpr int scale(size_t cnt) { return new_sz() * cnt; } //当且仅当 cnt 是常量表达式的时候，scale是常量表达式函数
      int arr[scale(2)]         // 是常量表达式
      int i = 2;                // 不是常量表达式
      int a2[scale(i)]          // 不是常量表达式，但编译通过，因为允许constexpr函数不返回常量表达式，此时 i = 2 可以在编译的时候就确定
      ```
    - 字面值类型（注意和字面值区分）
      - 可以被constexpr关键字修饰的类型
      - 算数类型，引用和指针都属于字面值类型，**字面值常量类**，枚举类型都是字面值类型
      - constexpt修饰指针和引用的时候，初始值必须是0或nullptr, 或者是存储于某个固定地址中的对象
    - 指针和constexpr
      - constexpr 只对指针有效
      ```C++
      constexpr int *np = nullptr;   // 指向整数的常量指针。 相当于 int *const np = nullptr;
      int j = 0;
      constexpr int i = 42; 
      // i, j 必须定义在函数体外，否则内存不固定，不可以用constexpr指针指向。（static变量除外）
      constexpr const int *p = &i;   // p 是常量指针，指向整形常量 i
      constexpr int *p1 = &j;        // p1 是常量指针，指向整数 j  
      ```
    - **constexpr 可以用来初始化数组**
      ```C++
      constexpr unsigned sz = 42;
      int *pz[sz];
      string strs[get_size()]; // 当且仅当 get_size() 是 constexpr
      ```
    - 类型别名：typedef, alias declaration
      ```C++
      typedef double base, *p;  // base = double, p = double*
      using base = double;      // base = double
      ```
      - 与复合类型混用的时候需要注意**不可以直接将表达式替换回原来的类型进行理解**，而应该把typedef后的类型当做一个整体 x.
      ```C++
      typedef char *pstring;
      const pstring cstr = 0;   // cstr是指向char的常量指针，相当于 char *const cstr 
      const pstring *ps         // ps 是一个指针，指向的对象是 char *const. 
      ```
    - auto关键字 可以一行定义多个变量，但每个变量类型必须一致
    ```C++
    auto i = 0, *p = &i // success
    auto sz = 0, pi = 3.14 // fail
    ```
      - auto 关键字推断的时候会忽略顶层const, 但底层const会保留，若希望是个顶层const, 需要用 const auto i = ci;
    - decltype 类型指示符
      - 选择并返回操作数的数据类型
      ```C++
      decltype(f()) sum  = x;
      ```
      - 与 auto 不同， decltype会保留顶层const, 引用以及指针。注意推断解引用的指针时，推断的类型为引用
      - 若推断变量加了括号，推断出来的类型为引用
      ```C++
      int i = 42, *p = &i, &r = i;
      decltype(r + 0) b             // 正确，r + 0 返回int
      // decltype((i)) b               // 错误，(i)为表达式，返回引用
      // decltype(r) b              // 错误，r本身为引用，所以b此处为引用
      // decltype(*p) b             // 错误，*p推断类型为引用
      ```
    - struct成员变量初始化可以用等号，花括号，但不可以用圆括号
      ```C++
      struct my_struct {
          int a{0};
          int b = 0;
          // int c(0); 此处不可以，但如果是结构体外的变量，这样是可以的
      };
      ```