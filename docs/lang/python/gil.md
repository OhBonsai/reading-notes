# 无人知晓GIL

## 资料
- [什么是GIL](https://realpython.com/python-gil/)
- [怎么写线程安全的代码](https://opensource.com/article/17/4/grok-gil)
- [RubyGil](https://ruby-china.org/topics/28415)

## 扯会儿蛋
一直以来，都有人提问Python GIL是什么。感觉很多博客也讲不清楚，或者讲的很浅。有些人来问我这个问题：Python GIL跟我讲一下。 听到就很好笑，GIL是CPython的特性,不是Python的特性。
有时候用这个问题问别人，得到的解答都是为了防止数据竞争，所以要有一个锁。然后就没有然后了。这篇文章会很长，是为了让我自己
能够彻底理解GIL是什么，如果你能够回答以下问题，**别浪费你的时间，直接关闭这个页面吧**。如果不能，看完会对你帮助很大
- GIL到底解决了啥？
- 为什么CPython选择GIL作为问题的结局方案
- 有了GIL，为什么还要有Thread的API？
- 有了GIL, 为什么还有Threading的锁
- GIL下，线程何时挂起，如何选择挑选线程来占有GIL
- 我能不能设置线程的优先级？
- GIL能保证线程安全吗
- 如果是因为Python2是90年代的语言，那为啥Python3还要GIL
- GIL的情况下，我们要注意什么？他会影响到CRUD工程师吗？
- 多线程和多进程有什么区别，多进程会遇到什么问题

- 除了锁， 并发下的内存数据安全问题
- 系统线程，系统进程，用户线程，用户进程，Posix线程，Python Multi Threed, Python Multi Process的关系

## 分析

## 答案
