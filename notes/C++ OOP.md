- inline function:
  函数在class本体（头文件）内定义完成，自动成为inline function，inline函数和宏一样，预定义阶段完成替换，节省函数每次调用的额外开支，但会占用额外存储空间。**是否变成inline function由编译器自己决定，inline只是一个建议**

  在后面出现的，不在函数本体之中，可以加inline.

- public: 
  外界可以访问
- private:
  只有自己类可以访问
- constructor: (ctor, 构造函数)
  构造函数名称与类的名称相同，可以拥有参数，参数可以有默认实参，没有返回值。只有构造函数可以有初始化列表。用初始化列表相当于初始化，如果在大括号里面用等于，相当于赋值，相当于放弃了初始化的机会，效率会差一些。
  ```C++
  class complex
  {
  public:
    complex (double r = 0, double i = 0) : re(r), im(i) {}
    // complex() : re(0), im(0) {}
    // 此时这个函数不能被定义，因为上面的函数有默认值，r = 0, i = 0, 此时定义complex c1 的时候，编译器会找不用传参的构造函数，而两个函数此时都满足，出现二义性，不会编译。
    complex& operator += (const complex&);
    double real() const { return re; }
    double imag() const { return im; }
  private:
    double re, im;
    friend complex* __doapl (complex*, const complex&);
  }
  ```
- 编译器会将重载函数改名，所以不存在两个名称完全相同的函数。
- 构造函数的ctors放在private中，此时构造函数不能被调用，在Singleton设计模式中有这样的应用。此时只允许存在一个A对象。将该对象放在类的static类里，外界调用只能通过本类定义的getInstance()函数。
  ```C++
  class A {
  public:
    static A% getInstance();
    setup() {...}
  private:
    A();
    A(const A& rhs);
    ...
  };
  
  A& A::getInstance() 
  {
      static A a;
      return a;
  }
  ```
- 在函数后面加const：该函数不可以改变该对象中的数据内容。
  ```C++
  double real() const { return re; }
  ```
  不改变对象中的数据的函数一定要加const，如果不加的话，下面的例子编译不通过：
  ```C++
  const complex c1(2, 1);
  cout << c1.real();    //此时编译器认为real()函数会改变该对象的值，而对象本身为常量，所以编译不通过。
  cout << c1.imag();
  ```
- 参数传递：pass by value vs. pass by reference (to const)
  - pass by value 会将传递的对象整体压入函数栈。
  - pass by reference 会将对象的地址传进去。
  - **参数传递最好传引用，速度会快。**
  - const 引用传递不可以改变被传递参数的值。
- 返回值传递：return by value vs. return by reference (to const)
  ```C++
  complex& operator += (const complex&);
  ```
  **在可以的情况下，尽量传reference**
- friend: 友元
  ```C++
  friend complex& __doapl(complex*, const complex&);
  ```
  - 友元可以自由获得private的成员。友元会打破封装。
  - 相同class的各个objects互为 friends 友元。
  ```C++
  class complex
  {
  public:
    complex(double r = 0, double i = 0) : re(r), im(i) {}
    int func (const complex& param) {
        return param.re + param.im;
    }
  private: 
    double re, im;
  }
  ```

- **类设计准则：**
  1. 数据要放在private里
  2. 函数参数要用reference来传递
  3. 函数返回值在可以的情况下要用reference来返回
  4. 类中的函数该加const要加
  5. 构造函数要用initialize list进行初始化
- 如果返回值本身有地址，可以返回引用，否则不可以。
- 操作符重载
  - 编译器看见二元运算符，会将这个操作符作用在前面的对象上：
  ```C++
  inline complex& __doapl(complex* ths, const complex& r) {
      ths->re += r.re;
      ths->im += r.im;
      return *ths;
  }
  inline complex& complex::operator+=(const complex& r) {
      return __doapl(this, r);
  }
    c2 += c1
  //^~ 相当于
  // c2.operator+=(c1)
  // 调用时相当于：
  inline complex& complex::operator+= (this, const complex& r){
  //                                   ^~~~ 编译器自动添加，不一定是第一个位置，取决于编译器，这个 this 是调用者的地址，此时this是c2的地址。
      return __doapl(this, r);
  }
  // 返回&可以连续赋值， 所以不可以返回void
  c3 += c2 += c1;
  // c1 先加到 c2上，再将 c2 加到 c3身上
  ```
  - 所有成员函数一定带有一个隐藏参数，this，this指向调用者
- 函数前面有 类名::说明是成员函数，如果没有，说明是全域函数。
- 非成员函数的操作符重载：
  ```C++
  inline complex operator + (const complex& x, const complex& y) {
      return complex (real(x) + real(y), imag(x), imag(y));
  }
  inline complex operator + (const complex& x, double y) {
      return complex(real(x) + y, imag(x));
  }
  inline complex operator + (double x, const complex& y) {
      return complex(x + real(y), imag(y));
  }
  // 这些函数不可以return by reference, 因为这些返回值是local object, typename() (typename + ()) 会创建一个临时对象。
  // complex(), complex(4, 5), 生命值仅仅存在于本行
  ```

  **应该放在类头文件中**，与成员函数区别在于**没有this指针**，编译器在找对应函数的时候，可能先去找成员函数操作符重载，也可能会找非成员函数操作符重载。
  ```C++
  // 单元运算符定义
  inline complex operator + (const complex& x)
  {
      return x;
  }
  inline complex operator - (const complex& x) {
      return complex(-real(x), -imag(x));
  }
  cout << -c1;
  ```
  **非成员操作符重载将类外对象作为左边的对象：**
  ```C++
  inline bool operator == (double x, const complex& y) {
      return x == real(y) && imag(y) == 0;
  }
  complex c;
  cout << (0 == c);
  ```
  - <<操作符的重载 (output operator), 只能写成全局重载
  因为操作符默认作用在左边的操作数上，但左边的是cout, 没有定义成员函数可以接受complex类型。返回值是ostream，可以进行链式编程
  ```C++
  #include <iostream>
  ostream& operator << (ostream& os, const complex& x) {
      return os << "(" << real(x) << ', ' << imag(x) << ')';
  }
  ```
- 编译器自带的拷贝赋值函数会将对应对象忠实的拷贝过去，所以带着指针的对象不行，只能将指针拷贝过来。
- 带指针的类：
  ```C++
  class String{
  public:
    // big three
    String(const char* cstr = 0);   // 构造函数
    String(const String& str);      // 拷贝构造函数，接收的是自己这种东西
    String& operator=(const String& str); // 拷贝赋值，接收的类是自己这种东西
    ~String();
    char* get_c_str() const { return m_data; }
  private: 
    char* m_data;
  }

  inline String::String(const char* cstr = 0) {  // 构造函数，传入一个其他对象
      if (cstr) {
          m_data = new char[strlen(cstr) + 1];
          strcpy(m_data, cstr);
      }
      else {  // 未指定初值
          m_data = new char[1];
          *m_data = '\0';
      }
  }

  inline String::~String() {
      delete[] m_data;
  }

  inline String::String(const String* str) {  // 拷贝构造函数，传入一个自身的对象
      m_data = new char[strlen(str.m_data) + 1];
      strcpy(m_data, str.m_data);
  }

  inline String& String::operator=(const String& str) {  // 拷贝赋值函数，重载等号，传入一个自身的对象
    if (&str == this) {
        return *this;
    }
    // 如果没有这一块，当发生自我赋值的时候，会将原有空间杀掉，自己不在存在char* 
    delete[] m_data;
    m_data = new char[strlen(str.m_data) + 1];
    strcpy(m_data, str.m_data);
    return *this;
  }

  #include <iostream>
  ostream& operator << (ostream& os, const String& str) {
      os << str.get_c_str();
      return os;
  }
  ```
- class with pointer members 必须要有 copy ctor 和 copy op=， 否则会出现内存泄漏，默认赋值只能将指针拷贝过去
- **变量生命周期**：
  1. stack object在作用域结束之后自动消失
  1. static local object 的生命在作用域结束之后仍然存在，直到程序结束
  2. global object 写在任何大括号之外，在main函数结束之后才消失
  1. heap objects的生命周期在delete之后才会结束，不delete就内存泄露
- new 会先分配memory在调用ctor. 
  ```C++
  Complex* pc = new Complex(1, 2);
  --> 
  void* mem = operator new (sizeof(Complex)); // 分配内存
  pc = static_cast<Complex*>(mem);            // 转型
  pc->Complex::Complex(1 ,2);                 // 调用构造函数
  // Complex::Complex(pc, 1, 2)               // this为调用这个函数的指针。成员函数：谁调用成员函数，谁就是this。

  delete ps;
  -->
  String::~String(ps);                        // 析构函数
  operator delete(ps);                        // 释放内存，内部调用free(ps)
  ```
- 数组new如果不搭配数组delete, 会造成内存泄漏。
  如果不搭配array delete, 只会对地址空间第一个元素调用析构函数，即**数组第0个元素**，内存泄漏情况取决于数组对象中有没有指针，如果有指针，会造成其他对象内存泄露，如果没有指针，不会出现问题。
- 判断是否返回reference: 如果目的端本来就存在，就可以返回reference