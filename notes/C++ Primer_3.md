# 第三章

- 头文件不应该包含 using 声明
- string 类用字面值初始化的时候，不包含字面值最后的空字符。
- .size() 返回 无符号整数类型**size_type**(与机器特性无关)。
```C++
string s1("hiya"); // 直接初始化
string s2 = "hiya"; // 拷贝初始化
string s3 = string("hiya"); // 从新建临时变量进行拷贝初始化
```
- cin读取字符串的时候，会过滤掉前面的空白，从第一个真正的字符开始读取，一直到下一处空白。
- getline(cin, line) 读取的时候，保留所有空白，直到换行符为止，换行符在line中会被抛弃，最后所得中不含换行符。
- vector 默认初始化： 内置类型初始化为0，其他类型调用该类默认构造函数
- vector 花括号初始化的时候会默认认为括号中的元素都是初值，除非真的不是
  ```C++
  vector<int> v1(10);       // {0,0,0,0,0,0,0,0,0,0};
  vector<int> v2{10};       // {10}
  vector<int> v3(10, 1);    // {1,1,1,1,1,1,1,1,1,1};
  vector<int> v4{10, 1};    // {10, 1}
  vector<string> v5{"hi"}   // {"hi"}
  // vector<string> v6("hi")   // 错误
  vector<string> v7{10}     // {"", "", "", ..., ""}
  vector<string> v8{10, "hi"} // {"hi", "hi", ..., "hi"}
  ```
- vector 范围 for 循环，不能在循环过程中增加或减少元素数量，否则访问会出问题
- vector 的 c_str()函数返回 const char* 类型，但如果后续操作改变了原string的值，可能让之前的const char*失去作用。如果后续想用，应该拷贝一份
```C++
std::string a = "12";
const char* cp = a.c_str();
std::cout << cp << std::endl;           // 12
a = "123123123132123123123131231233";
std::cout << cp << std::endl;           // 
```
- 迭代器类型： 
  ```C++
  vector<int>::iterator it;              // 可读写, 可用于非 const vector
  vector<int>::const_iterator it_const;  // 仅可读, 可用于任何vector
  ```
  - 在非const vector里，用 .cbegin() 和 .cend() 获得常量迭代器
  - auto在非const vector返回为 vector< int >::iterator, 在const vector里返回为vector< int >::const_iterator
  - 迭代器相减是 **difference_type** 类型的带符号整数
- 数组列表初始化，多余的元素初始化为0或默认构造
  - 复杂声明：
    ```C++
    int *ptrs[10] // 含有十个整形指针的数组
    int (*Parray)[10] = &arr //指向含有十个整数的数组的指针，从内向外：(*Parray)说明是指针，再从右向左，[10]表示指向的是大小为10的数组，int表示数组中存的是整数
    int (&arrRef)[10] = arr  // 引用一个含有是个整数的数组
    int *(&arrRef)[10] = ptrs // 引用一个大小是[10]的数组，数组里是10个int指针
    ```
  - 数组下标类型为**size_t**
  - 数组也可以用范围循环，因为维度是类型的一部分
  - 编译器会将用得到数组名字的地方自动转换为指向数组头的指针（第一个元素）
    ```C++ 
    string nums[] = {"one", "two", "three"};
    string *p = &nums[0];
    string *p2 = nums;  //等价于 p2 = &nums[0]
    ```
    - 使用数组作为一个auto的时候，返回指针类型而不是数组类型。而使用decltype的时候，推断得数组
    ```C++
    int ia[] = {0, 1, 2};
    auto ia2(ia); // 相当于 auto ia2(&ia[0]);
    decltype(ia) ia3 = {1, 2, 3};
    ```
  - 用库函数**begin**和**end**获取数组的首尾指针，定义在 iterator 头文件中
    ```C++
    int ia[] = {0, 1, 2};
    int *beg = begin(ia);  // 指向0
    int *last = end(ia);    // 指向 2 下一个元素
    ```
  - 指针相减返回 **ptrdiff_t**的标准类型
  - 内置数组的下标可以为负值，执行取下标操作的时候，数组名自动转换为指向数组头的指针，并对该指针进行操作。 而标准库的类下标只能为正数(**size_t**)
    ```C++
    int ia[] = {0, 2, 4, 6, 8};
    int *p = ia[2];
    int j = p[1];     // j = 6;
    int k = p[-2];    // k = 0;
    ```
- 使用auto遍历多维数组的时候，除最内层循环外，其余应该传引用值，防止auto将数组名推断成指针变量，从而导致内层循环遍历失败。
```C++
for (const auto & row : ia) 
    for (auto col : row) 
        cout << col << endl;
```
- 左值和右值：当对象被用作右值的时候，用的是对象的值（内容），放对象被用作左值的时候，用的是对象的身份（在内存中的位置）。需要用到右值的地方可以用左值来代替，但是不能把右值当成左值只用。当一个左值被当成右值使用时，实际使用的是他的内容。