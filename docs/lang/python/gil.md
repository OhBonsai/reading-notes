# 无人知晓的CPython GIL

## 资料
- [什么是GIL](https://realpython.com/python-gil/)
- [怎么写线程安全的代码](https://opensource.com/article/17/4/grok-gil)
- [RubyGil](https://ruby-china.org/topics/28415)

- [fork thread](https://unix.stackexchange.com/questions/364660/are-threads-implemented-as-processes-on-linux)
- [fork thread](https://unix.stackexchange.com/questions/364660/are-threads-implemented-as-processes-on-linux)
- [thread are fun](https://www.ibm.com/developerworks/library/l-posix1/index.html)
- [linux posix thread](https://www.cs.utexas.edu/~witchel/372/lectures/POSIX_Linux_Threading.pdf)

## 你真的了解GIL?
一直以来，都有人提问Python GIL是什么。感觉很多博客也讲不清楚，或者讲的很浅。
有些负责面试的同志来问我这个问题：Python GIL跟我讲一下。
听到就很好笑，GIL是CPython的特性,不是Python的特性，但凡看过官方文档都不至于忘了CPython这个事情。我并不觉得这是茴香豆有几种写法的问题，
而是对问题没有深刻认识而作出的引人发笑的提问。这也是我判断面试人员Python水平的一个重要根据，真的很有效。向我提问的三四个面试者，对于我接下来
的问题，我在面试过程稍微带入反问一下，果然完全不会:smile:。


有时候用这个问题问别人，得到的解答都是为了防止数据竞争，所以要有一个锁。然后就没有然后了。但是这篇文章会很长，是为了让我自己
能够彻底理解GIL是什么，如果你能够回答以下问题，**别浪费你的时间，直接关闭这个页面吧**。如果不能，我相信看完会对你帮助很大！
- GIL到底解决了啥？
    - Thread Pitfalls
        - Race conditions
        - Thread safe code
        - Mutex Deadlock
- 为什么CPython选择GIL作为问题的结局方案
- 有了GIL，为什么还要有Thread的API？
- 有了GIL, 为什么还有Threading的锁
- Cpython的多线程能够跑在多核机器上
- GIL下，线程何时挂起，如何选择挑选线程来占有GIL
- 我能不能设置线程的优先级？
- GIL能保证线程安全吗
- 如果是因为Python2是90年代的语言，那为啥Python3还要GIL
- GIL的情况下，我们要注意什么？他会影响到CRUD工程师吗？
- 多线程和多进程有什么区别，多进程会遇到什么问题， 多进程有哪些通信方法，python multiproessing使用了哪些通信方法

一些扩展的问题
- 除了锁，并发下的内存数据安全问题有哪些解决方法
- posix multi-thread 与 fork exec wait系统调用的关系
- 多线程在一个进程上， 那么多线程是作用在多核机上会作用在多个核上吗
- 系统线程，系统进程，用户线程，用户进程，Posix线程，Python Multi Threed, Python Multi Process的关系

本篇文章对涉及一些对CPython源码的分析，不过不要害怕只是一些逻辑，并没有太高级的语言技巧。

## Python的多线程机制



## 答案

