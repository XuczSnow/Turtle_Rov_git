# C语言编程规范（ref. Google）

## 1. 编码问题

一般来说，采用ASCII码是最通用的形式，包含中文时尽量在文件中采用较为通用的UTF-8编码，这是目前来看兼容性最强的方式，GBK编码在部分软件上显示异常或者编译异常。

当然，根据你的习惯的编译器设置，如果都支持，请使用UTF-8编码。

## 2. 一些需要注意的事项

### 2.1. 头文件

1. 头文件应独立

    头文件应该可以被独立包含，定义插入文本的头文件应以`.inc`结尾。

2. 头文件保护

    所有头文件都应该使用#define 来防止头文件被多重包含, 命名格式当是:`<PROJECT>_<PATH>_<FILE>_H`（外部），`__<PROJECT>_<PATH>_<FILE>_H`（内部）（此处原著为`<PROJECT>_<PATH>_<FILE>_H_`，参照相关单片机库，修改为上述定义）。

3. 前置声明

    避免在`.c`文件中对函数进行前置声明。

4. 内联函数

    内联函数应在10行以内，并且不能由递归、循环及swtich。

5. `#include`路径及顺序

    应按照项目的目录树结构进行排列，并遵从以下的顺序：

    1. 本文件的头文件
    2. C系统文件
    3. 其他库的`.h`文件
    4. 本项目内`.h`文件
    5. 特定平台的条件编译放在最后

### 2.2. 目录树

目录树的构建应该采用跟文件结构类似的形式，一般来说，个人比较喜欢参照STM32Cube生成的目录形式`<CATEGORIE>/<PATH>`，例如`Drivers/CMSIS`。

### 2.3. 作用域

1. 静态变量

    鼓励在`.c`文件中定义静态变量（不需要外部引用），只允许POD(Plain Old Data):类型的静态变量，并且需要被明确的初始化（不允许使用函数返回值等初始化）。

2. 局部变量

    局部变量在使用时初始化，输入for、while的变量应在条件中（或内部）声明以限制其作用域，但是一些复杂的数据结构除外。

3. 全局变量和常量

### 2.4. 函数

1. 参数顺序

    一般来说，输入参数在先，后跟输出参数，这符合一般人的阅读逻辑。在单片机编程中会有很多操作对象（结构体），是一些参数的集合，可能会涉及到输入输出的混合，放于参数的开头。当然，首要的原则是符合阅读的顺序，而不是添加的顺序。

2. `const`关键字

    在函数的输入参数中增加const关键字修饰，以避免在函数编程的过程中被意外地修改，也可以明确地标识此参数为输入参数；但是，在部分编译器中，可能不会提示理会`const`关键字。

3. 简短的函数

    最好编写不超过40行的函数，函数仅完成单一的功能。

### 2.5. 类型转换

1. 类型转换
    C语言类型的转化在不同的编译环境下不同，这是一个令人头痛的问题；仅在单片机中，当无符号类型与其他类型相遇时，会全部转化为无符号类型，所以尽量使用显式的（强制的）类型转换，并且在宏定义中的参数指明类型，例如`#define EXAMPLE 1u`。

### 2.6. 堆栈分配

1. 单片机堆栈分配

    *单片机运行内存空间有限，堆栈分配往往是一个棘手的问题，在uC/OS-III操作手册中提及栈空间分配的问题，原文大意如下：*

2. 输入输出

    在正式发布的程序中，避免使用`printf`,`scanf`这样的格式化输入输出函数，会增加堆栈程序的使用，容易产生溢出或在中断中使用造成异常，并且效率较低。
    建议的替代，使用单独的文本宏定义或者常量，可能会占用更多的时间

### 2.7. 前置自增自减

从编译结果来看，前置自增自减效率更高，在可能的情况下使用前置，毕竟大多数情况应用于循环，自增自减会被运行很多次。

```c
    0x08021EC4 B580      PUSH          {r7,lr}
    0x08021EC6 B090      SUB           sp,sp,#0x40
    0x08021EC8 2000      MOVS          r0,#0x00
    0x08021ECA 900A      STR           r0,[sp,#0x28]
    0x08021ECC 900F      STR           r0,[sp,#0x3C]
    146:   i++; 
    0x08021ECE 980E      LDR           r0,[sp,#0x38]
    0x08021ED0 3001      ADDS          r0,r0,#0x01
    0x08021ED2 900E      STR           r0,[sp,#0x38]
    147:   ++i; 
```

### 2.8. 数据类型

尽量使用C99中定义的整形定义，详情参见`stdint.h`，例如`uint8_t`。

### 2.9. 宏

尽量使用内联函数、枚举量、常量来代替宏，除非一些常见的使用方式

### 2.10. sizeof

尽可能用sizeof(varname) 代替sizeof(type)

## 3. 命名约定

### 3.1. 通用规则

要有描述性，少用缩写，除非一些定义或约定俗成，一般来说为`<PROJECT>_<DESCRIPTION>`，并使用`_`分割，不推荐匈牙利命名法，过于复杂。

### 3.2. 文件命名

全小写，并使用`_`分割单词。

### 3.3. 类型命名

首字母大写并不包含下划线，适用于结构体名、枚举名、类型定义；在C语言中，有时会需要同时用到结构体名和结构体类型名，可以在结构体类型名后加`_Type`作为区分。

### 3.4. 变量及常量命名

变量名及结构体变量名，采用小写字母加`_`分割语义块；一些特殊关键字修饰的常量采用相应的小写字幕开头修饰。

```c
例如，const 采用'k_'开头，register 采用'r_'开头，static 采用's_'开头，volatile 采用'v_'开头，指针类型采用'p_'开头，大写字母开头的结构体或者枚举量则可以不带'_'
```

### 3.5. 函数命名

函数采用大小写混合加`_`分割语义块。

### 3.6. 枚举及宏命名

枚举及宏采用全大写及`_`分割语义块。

## 4. 注释

### 4.1. 注释风格

建议采用`/* */`来进行注释，并进行分段；大段代码的注释，采用`#if 0`及`#endif`并注明原因。

对于显而易见的内容不要啰啰嗦嗦，保持合适的风格即可。

### 4.2. 文件注释

在每个文件开头注明一些信息，比如法律公告、作者信息、文件内容介绍

### 4.3. 函数注释

1. 函数声明注释

    函数注释只需要描述函数，而不需要说明如何实现，一般包含以下一些内容：
    - 函数的输入输出、返回值描述
    - 是否存在使用上的隐患
    - 其他一些需要注意的事项

2. 函数实现注释

    对函数实现中、巧妙的, 晦涩的, 有趣的, 重要的地方增加解释性的注释即可。

### 4.4. 变量注释

全局变量或者结构体成员往往需要解释说明含义及用途，至于之前或者之后，根据个人习惯。

### 4.5. 注释编写原则

写好注释，清晰易懂。

### 4.6. TODO注释

对那些临时的, 短期的解决方案, 或已经够好但仍不完美的代码使用TODO 注释。

## 5. 格式

### 5.1. 行长度

每一行字符数不超过 80，根据实际情况进行调整，但不宜太长，原则是一个屏幕可以看完。

### 5.2. 缩进

使用空格进行缩进（将Tab转化为空格），根据习惯使用2或4个空格。

### 5.3. 函数的声明与调用

1. 函数声明或定义格式：
    - 使用好的参数名
    - 如果返回类型和函数名在一行放不下, 分行，并且不要缩进
    - 左圆括号总是和函数名在同一行，之间永远没有空格
    - 圆括号与参数间没有空格
    - 左大括号总在最后一个参数同一行的末尾处, 不另起新行
    - 右大括号总是单独位于函数最后一行, 或者与左大括号同一行
    - 右圆括号和左大括号间总是有一个空格
    - 所有形参应尽可能对齐，如果放不下酌情换行，中间换行保持对其，第一个函数换行保持4个空格的缩进
    - 如果本身有一定的结构，遵循其本身的结构

2. 函数调用格式：
    - 与声明或定义的格式相同
    - 函数参数复杂时，直接创建临时变量传递参数

### 5.4. 条件语句

不在圆括号内使用空格，另外：

- 所有情况下if 和左圆括号间都有个空格. 右圆括号和左大括号之间也要有个空格
- 如果能增强可读性, 简短的条件语句允许写在同一行. 只有当语句简单并且没有使用else 子句时使用，如果语句有else 分支则不允许
- 单行语句不需要使用大括号，但如果语句中某个if-else 分支使用了大括号的话, 其它分支也必须使用

### 5.5. 循环和switch语句

switch 语句可以使用大括号分段, 以表明cases 之间不是连在一起的，switch 应该总是包含一个default 匹配，以避免错误或者warning。

在单语句循环里, 括号可用可不用. 空循环体应使用{} 或continue，例如`while (condition) continue;`

### 5.6. 指针和引用表达式

句点或箭头前后不要有空格。指针/地址操作符(*, &) 之后不能有空格。例如：

```c
x = *p;
p = &x;
x = r.y;
x = r->y;
```

类似于`int *p,i`的语句。不加空格以避免被认为是两个指针，实际来说是一个指向`int`类型的指针和一个`int`类型的变量。

### 5.7. 布尔表达式

如果一个布尔表达式超过标准行宽, 断行方式要统一，逻辑操作符可以位于行尾，类似于我们学习的数学表达式分行的方式。可以考虑插入额外的圆括号，增加可读性。

### 5.8. 函数返回值

不要增加额外的圆括号。

### 5.9. 预处理指令

置于行首，不要缩进。

### 5.10. 水平留白与垂直留白

恰当但不要多余，并于现有规则一致。

## 6. 其他

1. 在IDE中，将warning视作error处理。
