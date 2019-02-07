## 程序编码

### 汇编/反汇编

```
gcc -Og -S mstore.c
gcc -Og -c mstore.c
objdump -d mstore.o
gcc -Og -o prog main.c mstore.c
objdump -d prog
gcc -Og -o prog main.c mst
```

### 机器级编码
- 指令集架构
- 机器级程序使用的内存是虚拟地址

处理器将指令执行搞得像线性一样
- 程序计数器： 给出下一条指令的内存的地址
- 整数寄存器：包含16个命名的位置，分别存储64位的值。这些寄存器可以存储地址或整数数据。
- 条件码寄存器：保存最近的执行的算数或逻辑指令的状态信息。
- 向量寄存器：存放一个或者多个整数或浮点数值

```
mstore.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <multstore>:
   0:	53                   	push   %rbx
   1:	48 89 d3             	mov    %rdx,%rbx
   4:	e8 00 00 00 00       	callq  9 <multstore+0x9>
   9:	48 89 03             	mov    %rax,(%rbx)
   c:	5b                   	pop    %rbx
   d:	c3                   	retq
```

机器代码和它的反汇编表示的特性
- x86-64的指令长度从1到15个字节不等，常用的指令以及操作数较少的指令所需的指令更加的少

生成实际可执行的代码需要对一组目标代码文件运行链接器，而这一组目标代码文件中必须含有一个main函数


### 数据格式
最开始是16位的，所以intel汇编把16位作为一个字。汇编的指令一般都有一个字符的后缀
来表示操作数的大小。例如movb, movw, movl,  movq
```
-----------------------------------------------------------
| c声明 ｜ Inter数据类型 ｜  汇编代码后缀 ｜ 大小（字节） ｜
-----------------------------------------------------------
| char  ｜    Byte       ｜  b            ｜    1         ｜
-----------------------------------------------------------
| short ｜    Word       ｜  w            ｜    2         ｜ 
-----------------------------------------------------------
| int   ｜  Double Word  ｜  l            ｜    4         ｜
-----------------------------------------------------------
| long  ｜  Four Word    ｜  q            ｜    8         ｜
-----------------------------------------------------------
| char* ｜  Four Word    ｜  q            ｜    8         ｜
-----------------------------------------------------------
| float ｜  单精度       ｜  s            ｜    4         ｜
-----------------------------------------------------------
| doubl ｜  双精度       ｜  i            ｜    8         ｜
-----------------------------------------------------------
```

### 访问信息
1. CPU包含16个存储64位的通用目的寄存器
2. 寄存器以%r开头。%r
3. 寄存器命名很有讲究，可以通过命名操作寄存器所用大小，互不干扰。
   - %rax  64bit
   - %eax  32bit
   - %ax   16bit
   - %al   8bit
4. 如果只用了低位字节，用了低位字节，保持剩下的字节不变

### 操作指示符
大多数指令有一个或多个操作符。
- 立即数： 用来表示常数值, 用`$`后面更一个标准c表示。比如$-577, $0x1f
- 寄存器： 用来表示某个寄存器的某几个。用R[ra]来表示它的值。相当于数组R，用寄存器标示符作为索引
- 内存引用：根据计算出来的地址访问某个内存位置，因为将内存看成一个很大的字节数组，用符号Mb[Addr]表示存储在内存中从地址addr开始的b个字节值的饮用，为了简便，我们通常省略下标b.

```
Imm: 立即数
Reg：寄存器
Mem: 存储器
s: 比例数 1, 2, 4, 8

# 立即数寻址
$Imm = Imm

# 寄存器寻址
ra = R[ra]

# 绝对寻址
Imm = M[Imm]

# 间接寻址
(ra) = M[R[ra]]

# 比例变址寻址
Imm(rb, ri, s) = Mem[Imm + Reg[rb] + Reg[ri] * s]

```

### 数据传送指令

#### MOV
x86-64加一个限制，传送指令的两个操作数不能都指向内存位置，即不能将一个值从内存位置复制到另外一个内存位置需要两条指令。


```
-----------------------------------------------------------
|      指令            ｜ 效果   ｜     描述              ｜
-----------------------------------------------------------
|   MOV  [S]rc  [D]es  ｜D <- S  ｜     传送              ｜
-----------------------------------------------------------
|   movb               |         |    传送字节            ｜  
|   movw               |         |    传送字              ｜  
|   movl               |         |    传送双字            ｜  
|   movq               |         |    传送四字            ｜  
|   movabsq I, R       | R <- I  |    传送绝对的四字      ｜  
----------------------------------------------------------- 
```
数据传送如何影响寄存器的值 
``` 
1   movabsq $0x0011223344556677, %rax  %rax = 0011223344556677
2   movb    $-1, %al                   %rax = 00112233445566ff
3   movw    $-1, %ax                   %rax = 001122334455ffff
4   movl    $-1, %eax                  %rax = 00000000ffffffff
5   movq    $-1, %raxl                 %rax = ffffffffffffffff



1   movabsq $0x0011223344556677, %rax  %rax = 0011223344556677
2   movb    $0xaa, %dl                 %dl  = AA
3   movb    $dl, %al                   %rax = 00112233445566AA
4   movsbq  $dl, %rax                  %rax = ffffffffffffffAA
5   movzbq  $dl, %rax                  %rax = 00000000000000AA
```


### 数据传送示例
```
# C code
long exchange(long *xp, long y)
{
		long x = *xp;
		*xp = y;
		return x;)
}

# ASM
exchange:
	movq (%rdi), %rax  ;Get x at xp, Set as return value
	movq %rsi, (%rdi)  ;Store y at xp
	ret                ;Return
```


### 压入和弹出栈数据
```
------ N 
|    |
------
|    |
------
|    |
------
|    |
------ 0 TOP, 栈顶在低位



-----------------------------------------------------------
|   指令  ｜    效果                     ｜  描述         ｜
-----------------------------------------------------------
| pushq S | R[%rsp] <- R[%rsp] - 8       | 先把栈顶指针－8|
|         | M[R[%rsp]] <- S              | 把4字压入栈    |
-----------------------------------------------------------
| popq  D | D <- M[R[%rsp]]              | 先把4字弹出栈  |
|         | R[%rsp] <- R[%rsp] + 8       | 把栈顶指针+8   |
-----------------------------------------------------------

```


### 算数与逻辑操作
```
-----------------------------------------------------------
|   指令     ｜    效果                  ｜  描述         ｜
-----------------------------------------------------------
| leaq S, D  |   D <- &S                 | 加载有效地址   ｜
-----------------------------------------------------------
| INC D      |   D <- D + 1              | Add 1          |
| DEC D      |   D <- D - 1              | Sub 1          |
| NEG D      |   D <- -D                 | Negate         |
| NOT D      |   D <- ~D                 | Complement     |
-----------------------------------------------------------
| ADD S, D   |   D <- D + 1              | Add 1          |
| SUB S, D   |   D <- D + 1              | Add 1          |
| IMUL S, D  |   D <- D + 1              | Add 1          |
| XOR S, D   |   D <- D + 1              | Add 1          |
| OR S, D    |   D <- D + 1              | Add 1          |
| AND S, D   |   D <- D + 1              | Add 1          |
-----------------------------------------------------------
| SAL k, D   |   D <- D << k             | Left Shift     |
| SHL k, D   |   D <- D << k             | Left Shift     |
| SAR k, D   |   D <- D >> k             | Arith RS       |
| SHR k, D   |   D <- D >> k             | Logical RS     |
-----------------------------------------------------------

```

leaq指令的妙用
```
long scale(long x, long y, long z) {
		long t = x + 4 * y + 12 * z;
		return t
}

# ASM
scale:
	leaq  (%rdi, %rsi, 4), %rax  ;x + 4 * y
	leaq  (%rdx, %rdx, 2), %rdx  ;z + 2 * z = 3 * z
    leaq  (%rax, %rdx, 4), %rax  ;z + 2 * z = 3 * z

```

## 控制

### 条件码
除了整数寄存器，cpu还维护一组单个位的条件码(condition code)寄存器,这个寄存器描述最近的算术或
逻辑操作的属性，可以检测这些寄存器的状态来执行分支指令。最常用的条件码有：
-CF: Carry Flag. The most recent operation generated a carry out of the most significant bit. Used to detect overflow for unsigned operations.
-ZF: Zero Flag. The most recent operation yielded zero.
-SF: Sign Flag. The most recent operation yielded a negative value.
-OF: Overflow Flag. The most recent operation caused a two’s-complement overflow—either negative or positive.

```
-----------------------------------------------------------
|   指令     ｜    基于                  ｜  描述         ｜
-----------------------------------------------------------
| CMP  S1,S2  |   S2 - S1                | 比较           |
-----------------------------------------------------------
| TEST S1,S2  |   S2 & S1                | 测试           |
-----------------------------------------------------------
``

### 访问条件码
条件码通常不会直接读取，常用的使用的方法有三种：
- 可以根据条件码的某种组合，将一个字节设置位0或者1
- 可以条件跳转到程序的某个其他的部分
- 可以有条件的传送数据
setl -> set less -> 小于时设置
setb -> set below -> 低于时设置

### 跳转指令
正常执行的情况下，指令按照他们出现的顺序一条一条执行。跳转会导致程序执行切换到一个全新的位置，在
汇编代码中，这些跳转的目的通常用一个`label`标号来表示。
```
 movq $0, %rax        ; Set %rax to 0
 jmp .L1              ; Goto .L1
 movq (%rax), %rdx    ; Null pointer dereference(skipped)
.L1:
 popq %rdx            ; Jump target
```
- 直接跳转: 用标签来跳转 `jmp .L1`
- 间接跳转: `*`后面跟着一个操作数指示符。

JMP 用标志位来判断
```
------------------------------------------------------------------------
| 指令         |同义词|  跳转条件         |  描述                      |
------------------------------------------------------------------------
|jmp Label     |      |   1               | Direct jump                |
|jmp *Operand  |      |   1               | Indirect jump              |
|je Label      |jz    |   ZF              | Equal / zero               |
|jne Label     |jnz   |   ~ZF             | Not equal / not zero       |
|js Label      |      |   SF              | Negative                   | 
|jns Label     |      |   ~SF             | Nonnegative                |
|jg Label      |jnle  | ~(SF ^ OF) & ~ZF  | Greater (signed >)         |
|jge Label     |jnl   | ~(SF ^ OF)        | Greater or equal(signed >=)|
|jl Label      |jnge  |  SF ^ OF          | Less (signed <)            |
|jle Label     |jng   | (SF ^ OF) | ZF    | Less or equal (signed <=)  |
|ja Label      |jnbe  | ~CF & ~ZF         | Above (unsigned >)         |
|jae Label     |jnb   | ~CF               | Above or equal(unsigned >=)|
|jb Label      |jnae  |  CF               | Below (unsigned <)         |
|jbe Label     |jna   | CF | ZF           | Below or equal(unsigned <=)|
------------------------------------------------------------------------
```

### 跳转指令的编码
调准指令的编码有两种
- 编码为目标地址
- 编码为间隔地址


### 用条件控制来实现条件分支

C语言的`if-else`语句转化为汇编
```
if (test-expr)
	then-statement
else
	else-statement



```

### 用条件传送来实现条件分支

实现条件操作的传统方法是通过控制的条件转移，当条件满足时，程序沿着一条执行路径执行，而当条件不满足时，就走另一条路径。
这种机制简单而通用，但是在现在的处理器上，它可能会非常抵消。
另外以重替代的策略时使用数据的条件转移。这种方法计算一个条件操作的两种结果。然后在根据条件是否满足从中选择一个。

```
# c diff
long absdiff(long x, long y)
{
		long result;
		if (x < y )
				result = y - x;
		else
				result = x -y ;
		return result;
}


# asm in c
long cmovdiff(long x, long y)
{
		long rval = y - x;
		long eval = x - y;：
		long ntest = x >= y;
		if (ntest) rval = eval;
		return rval;
}


# asm
; x in %rdi, y in %rsi
absdiff:
	movq %rsi, %rax
	subq %rdi, %raxa      ;rval = y -x
	movq %rdi, %rdx       

	subq %rsi, %rdx       ;eval = x - y
	cmpq %rsi, %rdi       ;compare x:y
	cmovge %rdx, %rax     ;if >=, rval = eval
	ret                   ;return
```
为什么在某系情况下条件数据传送代码比基于条件控制转移的代码性能要好，处理器通过流水线来获取高性能。
在流水线中，一个指令的处理要经过一系列的阶段，每个阶段执行所需操作的一小部分，例如从内存中取指令，
确定指令内省，从内存读数据，执行算术运算，像内存写数据。以及更新程序计数器。这种方法通过重叠
连续指令的步骤来获取高性能。例如在取一条指令的同时，执行它前面一条指令的算术运算。要做到这一点，要求能够
事先确定要执行的指令需略，这样才能保持流水线上充满了待执行的指令。当机器遇到条件跳转，只能靠猜测获取
接下来要执行哪些指令。如果猜对了，那么流水线上就是满的。如果猜错了。拿就废弃之前的工作。一个错误的预测
会浪费15~30个时钟周期。

### 循环

#### Do-while
```
do
    body-statement
	while (test-expr);


loop:
    body-statement
	t = test - expr;
	if (t)
		goto loop;
```


#### While
```
while(test-expr)
		body-statement



loop:
    t = test-expr;
	if (t)
		body-statement;
		goto loop

```


#### SWITCH
开关语句可以根据一个整数索引值进行多重分支。在处理句有多种可能结果的测试时。这个语句特别有用。可以通过跳转表
这种数据结构使得SWITCH更加高效。**跳转表**是一个数组，表项I是一个代码段的地址。使用跳转表内的数组有一个好处时
执行SWITCH语句的时间与分支数量无关。

```
void switch_eg(long x ,long n , long *dest){
		long val = x;
		switch(n) {
				case 100:
						val *= 13;
						break;
				case 102:
						val += 10;
				case 103:
						val += 11;
						break;
				case 104:
				case 106:
						val *= val;
						break;
				default:
						val = 0;
		}
		*dest = val
}


void switch_eg_impl(long x, long n, long *dest)
{
		static void *jt[7] = {

				&&loc_A, &&loc_def, &&loc_B,
				&&loc_C, &&loc_D, &&loc_def,
				&&loc_d
		};

		unsigned long index = n - 100;
		long val;

		if (index > 6)
				goto loc_def;

loc_A:
		val = x * 13;
		goto done;
loc_B:
		x = x + 10;
loc_C:
		val = x + 11;
		goto done;
loc_D:
		val = x * x;
		goto done;
loc_def:
		val = 0;
done:
		*dest = val;

}


;x in %rdi, n in %rsi , dest in %rdx
switch_eg:
  subq $100, %rsi               ; compute index = n - 100
  cmpq $6, %rsi                 ; compare index:6
  ja   .L8                      ; if >, goto loc_def
  jmp *.L4(,%rsi, 8)            ; Goto *jt[index]
.L3:                            ; loc_A
  leaq (%rdi, %rdi, 2), %rax    ; 3 * x
  leaq (%rdi, %rax, 4), %rdi    ; val = 4 * (3 * x) + x
  jmp .L2                       ; Goto done
.L5:                            ; loc_B
  addq $10, %rdi                ; x = x + 10
.L6:                            ; loc_C
  addq $11, %rdi                ; val = x + 11
  jmp .L2                       ; Goto done
.L7:                            ; loc_D
  imulq %rdi, %rdi              ; val = x * x
  jmp .L2                       ; goto done
.L8:                            ; loc_def:
  movl $0, %edi                 ; val = 0
.L2:
  movq %rdi , (%rdx)            ; *dest = val
  ret



   .section        .rodata ; Read only data
   .align 8  
.L4
    .quad  .L3
    .quad  .L8
    .quad  .L5
    .quad  .L6
    .quad  .L7
    .quad  .L8
    .quad  .L7
```



## 过程

**如何对待错误？**

过程是软件中的以重很重要的抽象，它提供了以重封装代码的方式。用一组指定的参数和一个可选的返回值实现了某个功能
然后，可以在程序中不同的地方调用这个函数。设计狼好的软件用过程作为抽象机制，隐藏某个行为的具体实现，同时有提
供清晰简洁的接口定义，说明要计算的是那些之，过程对程序状态产生什么样的影响。不同的变成语言中，过程的形式多样：
函数/方法/子例程/处理函数。但是它们有一些共有特性。
要提供对过程的及其及支持，必须要处理许多不同的属性，为了讨论方便，假设过程P调用过程Q,Q执行后返回到P.这些动作包括
下面一个或多个机制。
- 传递控制： 在进入过程Q的时候，程序计数器必须被设置为Q的代码的其实地址，然后再返回时，要把程序计数器设置为P
中调用Q后面那条指令的地址
- 传递数据： P必须能够像Q提供又给或者多个参数， Q必须能够像P返回一个值。
- 分配和释放内存： 在开始时，Q可能需要为局部变量分配空间。而在返回前，又必须释放这些存储控件


### 运行时栈
在过程P调用过程Q的例子厚葬，可以看到当Q在执行时，P以及所有在向上追溯到P的调用连中断的过程，都是暂时被挂起的。
当Q运行时，它只需要为局部变量分配新的存储控件。

### 传递控制
通过CALL来将控制从函数P到函数Q
```
------------------------------------------------------------------------
|     指令                         |           描述                    |
------------------------------------------------------------------------
| call               Lable         |  过程调用                         |
| call               *Operand      |  过程调用                         |
| ret                              |  从过程中返回                     |
------------------------------------------------------------------------

``


### 数据传送
当调用一个过程时，除了要把控制传递给他，并在过程中反hi在传递回来。过程调用还可能包括把数据作为参数传递，而从
过程返回还有可能包括或返回一个值。X86-64中，大部分过程见的数据传送都是通过寄存器实现的。
当过程P调用过程Q时，P的代码必须首先把参数赋值到适当的寄存器。类似的，当Q返回到P时，P的代码可以访问寄存器%rax
中的返回值。

X86-64中，通过寄存器最多传递六个整形(整数/指针)参数。寄存器的使用是由特殊顺序的。寄存器使用的名字取决于要传
递数据类型的大小.
```
-------------------------------------------------------------------------
|                 |                   参数数量                          |
| 操作数大小（位) |------------------------------------------------------
|                 |    1   |    2   |    3   |    4   |    5   |    6   |
-------------------------------------------------------------------------
|      64         |  %rdi  |  %rsi  |  %rdx  |  %rcx  |  %r8   |  %r9   |
-------------------------------------------------------------------------
|      32         |  %edi  |  %esi  |  %edx  |  %ecx  |  %r8d  |  %r9d  |
-------------------------------------------------------------------------
|      16         |  %di   |  %si   |  %dx   |  %cx   |  %r8w  |  %r9w  |
-------------------------------------------------------------------------
|      8          |  %dil  |  %sil  |  %dl   |  %cl   |  %r8b  |  %r9b  |
-------------------------------------------------------------------------
```
如果有一个函数大于6个整形参数，超过6个部分要通过栈来传递。假设够乘P调用过程Q，有N个整形参数，且N>6。那么P的代码
分配的栈帧必须能够容纳7到N号参数的存储控件。如图3-25所示。要把参数1~6复制到对应的寄存器。把参数7~N放到栈上，而
参数7位于栈顶，通过栈传递参数，所有的数据大小都向8的倍数对其。参数到位以后。程序就可以执行CALL指令将控制转移到过程
Q了。

```
void proc(long a1, long &a1p, int a2, int *a2p, short a3, short *a3p, char a4, char *a4p){
		*a1p += a1;
		*a2p += a2;
		*a3p += a3;
		*a4p += a4;
}

proc:
	movq  16(%rsp), %rax
	addq  %rdi, (%rsi)
    addl  %edx, (%rcx)
	addw  %r8w, (%r9)
	movl  8(%rsp), %edx
	addb  %dl, (%rax)
	rett
```

### 栈上的局部存储
有些时候，局部数据必须存放在内存中，常见的情况包括：
- 寄存器不足够存放所有的本地数据
- 对一个局变量使用地址运算符'&',因此必须能够为它产生一个地址
- 某些局部变量时数组或者结构,因此必须通过素组或者结构引用被访问到.
```
long swap_add(long *xp, long *yp){
		long x = *xp;
		long y = *yp;
		*xp = y;
		*yp = x;
		return x + y;
}


long caller()
{
		long arg1 = 534;
		long arg2 = 1057;
		long sum = swap_add(&arg1, &arg2);
		long diff = arg1 - arg2;
		return sum * diff;
}

caller:
	subq   $16, %rsp       ; 在栈上申请16bytes
	movq   $534, (%rsp)    ; 将534存储到栈上
	movq   $1057, 8(%rsp)  ; 存储1057到接下去八个字节
	leaq   8(%rsp), %rsi   ; 取1057 arg2的地址,存到rsi
	movq   %rsp, %rdi      ; 取534 arg1的地址,存到rdi
	call   swap_add        ; 调用
	movq   (%rsp), %rdx    ; 获取arg1
	subq   8(%rsp), %rdx   ; arg2 - arg1 存到 rdx
	imulq  %rdx, %rax      ; 计算compute diff
	addq   $16, %rsp       ; 将栈指针回到16byts
	ret                    ; return
```

```
--------------------------------------------------------------
|                  返回地址                   32             |
--------------------------------------------------------------
|                  x1                         32             |
--------------------------------------------------------------
|                  x2   20    |      x3      |  x4   |       |
--------------------------------------------------------------
|                  &x4                                       |
--------------------------------------------------------------
|                                                    |  a    | <- 栈指针%rsp
--------------------------------------------------------------
void proc(long a1, long *a1p, int a2, int *a2p, short a3, short *a3p, char a4, char *a4p){
		*a1p += a1;
		*a2p += a2;
		*a3p += a3;
		*a4p += a4;
}


long call_proc()
{
		long x1 = 1;
		int x2 = 2;
		short x3= 3;
		char x4 = 'a';

		proc(x1, &x1, x2, &x2, x3, &x3, x4, &x4);
		return (x1+x2)*(x3-x4);
}

all_proc:
.LFB1:
	subq	$32, %rsp            ; 申请32byte的栈空间
	movq	$1, 24(%rsp)         ; store long 1 到 24-32
	movl	$2, 20(%rsp)         ; store int 2 到 20-24
	movw	$3, 18(%rsp)         ; sotre short int 到 18-20
	movb	$97, 17(%rsp)        ; sotre char a 到 17-18
	leaq	17(%rsp), %rax       ; create *a 放到 rax
	movq	%rax, 8(%rsp)        ; 将*a的值当做arg8 8-15
	movl	$97, (%rsp)          ; 将a的值当做arg7 0-7
	leaq	18(%rsp), %r9        ; 将&x3 放到arg6
	movl	$3, %r8d             ; 将x3 放到arg5
	leaq	20(%rsp), %rcx       ; 将&x2 放到arg4
	movl	$2, %edx             ; 将x2 放到arg3
	leaq	24(%rsp), %rsi       ; 将&x1 放到arg2
	movl	$1, %edi             ; 将x1 放到arg1
	call	proc                 
	movslq	20(%rsp), %rdx       ; 将&x3 放到arg6
	addq	24(%rsp), %rdx       ; 将&x3 放到arg6
	movswl	18(%rsp), %eax       ; 将&x3 放到arg6
	movsbl	17(%rsp), %ecx       ; 将&x3 放到arg6
	subl	%ecx, %eax           ; 将&x3 放到arg6
	cltq
	imulq	%rdx, %rax           ; 将&x3 放到arg6
	addq	$32, %rsp            ; 将&x3 放到arg6
	.cfi_def_cfa_offset 8
	ret                          ; Return
	
```

### 寄存器中的局部存储空间
寄存器是唯一被所有过程共享的资源.虽然在给定时刻只有一个过程是活动的.但必须确保当一个过程调用
另外一个过程时,被调用者不会覆盖调用者稍后会使用的寄存器值.为止X86-64采用了一组统一的寄存器使用
惯例.所有过程必须遵循
根据惯例,寄存器%rbx, %rbp和%r12~%r15被划分为被调用者保存寄存器.当过程P调用过程Q时,Q必须保存这些
寄存器的值,保证他们的值在Q返回到P时与Q被调用时是一样.过程Q保存一个寄存器的不便.要不然就把这个值
压到栈中,然后结束的时候在弹出栈恢复到原来的位置.
```
long Q(long v)
{
		return v + 3;
}


long P(long x, long y)
{
		long u = Q(y);
		long v = Q(x);
		return u + v;
}

.LFB1:
	pushq	%rbp
	pushq	%rbx
	movq	%rdi, %rbp
	movq	%rsi, %rdi
	call	Q
	movq	%rax, %rbx
	movq	%rbp, %rdi
	call	Q
	addq	%rbx, %rax
	popq	%rbx
	popq	%rbp
	ret
```


### 递归过程
寄存器禾栈的模型可以递归调用它们的自身,每个过程在栈中都有自己的私有空间.因此多个
未完成调用的局部变量都不会相互影响.此外,栈的原则很自然地就提供了适当的策略,当过程
被调用时分配局部存储.当返回时释放内存

```
long rfact(long n)
{
		long result;
		if (n <= 1)
				result = 1;
		else
				result = n * rfact(n-1);
		return result;
}

.LFB0:
	pushq	%rbx
	movq	%rdi, %rbx
	cmpq	$1, %rdi
	leaq	-1(%rdi), %rdi
	call	rfact
	imulq	%rbx, %rax
	jmp	.L2
.L3:
	movl	$1, %eax
.L2:
	popq	%rbx
	ret

```


## 数组分配与访问
C语言的素组是一个将标量数据聚集成更大数据类型的方式,C语言实现数组的方式非常简单.因此
很容易翻译为机器代码,C语言的一个不同寻常的特点是可以产生指向数组中元素的指针,并对
这些指针进行运算,在机器代码中,这些指针会被翻译成地址来计算.优化编译器非常善于简化
数组索引所使用的地址计算,不过这是C代码和它到机器代码的翻译之间的对应关系有些难以理解
C语言指针不表达长度....首元素的地址就是数组.

### 基本原则
对于数据类型T和整形常熟N,声明如下:
T A[N];

### 指针运算
```
------------------------------------------------------------------------------------------
| 表达式        | 类型    |  值        |                     汇编代码                    |
------------------------------------------------------------------------------------------
| E             | int*    |  xe        |           movq %rdx , %rax                      |
------------------------------------------------------------------------------------------
| E[0]          | int     | M[xe]      |           movq (%rdx) , %rax                    |
------------------------------------------------------------------------------------------
| E[i]          | int     | M[xe+4i]   |           movq (%rdx,%rcx,4), %rax              |
------------------------------------------------------------------------------------------
| &E[2]         | int*    |  xe+8      |           leaq 8(%rdx) , %rax                   |
------------------------------------------------------------------------------------------
| E+i-1         | int*    |  xe+4i-4   |           leaq-4(%rdx, %rcx, 4) , %rax          |
------------------------------------------------------------------------------------------
| *(E+i-3)      | int     |M[xe+4i-12] |           movl-12(%rdx, %rcx, 4), %eax          |
------------------------------------------------------------------------------------------
| &E[i]-E       | long    | i          |           mov %rcx,  %rax                       |
------------------------------------------------------------------------------------------
```

### 嵌套的数组


### 定长数组
C语言编译器能够定长多维数组上的操作代码，这里我们展示优化等级设置为-01时GCC采用的一些优化。假设
我们用如下方式将数据类型fix_matrix声明位16*16的整型数组：
```
#define N 16
typedef int fix_matrix[N][N]

int fix_prod_ele(fix_matrix A, fix_matrix B, long i, long k) {

		long j;
		int result = 0;
		for (j = 0; j< N; j++) {
				result += A[i][j] * B[j][k];
		}
		return result;
}


int fix_prod_ele_opt(fix_matrix A, fix_matrix B, long i, long k ) {
		int *Aptr = &A[i][0];
		int *Bptr = &B[0][k];
		int *Bend = &B[N][k];
		int result = 0;

		do {
				result +=  *Aptr * *Bptr;
				Aptr ++;
				Bptr += N;
		} while (Bptr != Bend);
		return result;
}ypedef int fix_matrix[N][N]
```

### 变长数组
历史上如果要申请边长数组，必须使用malloc或者calloc这样的函数为这些数组分配存储空间。而且不得不显示的编码。不过现在可以在分配时候
才计算出数组长度，比如
```
int var_ele(long n, int A[n][n], long i, long j) {
		return A[i][j];
}


var_ele:
	imulq %rdx , %rdi
	leaq (%rsi, %rdi, 4), %rax
	movl (%rax, %rcx, 4), %eax
	ret
```


## 异质的数据结构
C语言提供了两种不同类型的对象组合到一起创建数据类型的机制：
- 结构
- 联合

### 结构
C语言的struct声明创建了一个数据类型，将可能不同类型的对象聚合到一个对象中。用名字来引用结构的各个组成部分。类似于数组的实现。结构的所有组成部分都存放在内存的一段连续的区域。而指向结构的指针就是结构第一个字节的地址。编译器维护关于每个结构类型的信息。指示每个字段的字节编译。它以这些偏移作为内存引用指令的位移。从而才生对结构元素的引用


### 联合
联合提供了一种方式，能够规避C语言的类型系统，允许多种类型来用一个对象。联合声明的语法和结构的语法是一样的，只不过语义相差比较大，它们用不同的字段来引用相同的内存块

```
struct S3 {
		char c;
		int i[2];
		double v;
}


union U3 {
		char c;
		int i[2];
		double v;
}
```
联合用于表示互斥的结构，联合内存的大小是联合内最大的大小。用来表示一些
明显互斥的机构。


## 集合控制与数据

### 指针
指针是C语言的一个核心特色，它们以一种同意方式，对不同数据结构中的元素产生了引用。指针有一些
基本概念其实非常简单。再次，我们重点介绍一些指针和它们映射到机器代码的关键原则
- 每个指针都对应于给类型，这个类型表明该指针只想的哪一类的对象，一下面的指针申明为例：
```
int *ip;
char **cpp;
```
变量IP[是一个指向int类型的指针，而CPP指针是指向指针指向的对象本身就是一个指向char类型的指针。通常，
如果对象类型为T，那么指针的类型为T*, 特殊的void *类型表示通用指针，比如说malloc函数返回一个通用指针，
然后通过强制类型转换或者赋值操作那样隐式强制类型转换。指针类型不是机器代码中的一部分，它们是一个C怨言
提供的一种抽象，帮助程序是避免寻址错误

- 每个指针都有一个值，这个值是某个指定类型的对象的地址，特殊的NULL(0)表示该指针没有指向任何地方。
- 指针用'&'运算符来创建。这个于是暖夫可以用到任何lvalue类的C表达式上，lvalue意指可以出现在赋值语句左边
的表达式，这样的例子包括变量一级结构，联合和数组的元素。我们已经看到因为leaq指令是设计用来计算内存引用
的地址，&运算符的机器代码实现常用的这条指令计算表达式的值
- *操作用于间接引用指针，其结果是要给值。他的类型与该指针的类型一致，间接引用使用内存引用来实现的。要么
是存储到一个指定的地址，要么是从指定的地址读取。
- 数组和指针精密联系： 一个素组的名字可以向一个指针比那辆一样引用。数组引用与指针运算符和间接引用有一个效果
数组引用和指针运算都需要用对象大小对偏移量进行伸缩。当我们写表达式p + i.这里的指针P的值是p.得到的地址计算为P + l * I
- 将指针从要给类型强制转换成另外一种类型，只改变的类型，而不改变它的值。强制类型转化的也给效果是改变指针运算的伸缩。
例如，如果P是要给char *类型的指针。它的值为P。
- 指针可以指向函数，它提供了要给很强的存储与代码传递引用的功能，这些引用可以被程序其他部分调用。例如如果我们有一个函数
用下面的原型定义
```
int fun(int x, int *p);
```
然后我们可以声明要给指针fp, 将它赋值为这个函数，代码如下
```
int (*fp)(int, int *);
fp = fun;
```
然后用这个指针来调用这函数；
```
int y = 1;
int result = fp(3, &y)
```















