# I/O

## 遇到的各种问题

## 工具背后要看什么

### [df](https://man.linuxde.net/df)
`df -a`直接看全部



## 操作系统概念

- 索引节点inode，用来记录文件的元数据，比如 inode 编号、文件大小、访问权限、修改日期、数据的位置。持久化存储到磁盘中。所d
- 目录项dentry，用来记录文件的名字、索引节点指针以及与其他目录项的关联关系。多个关联的目录项，就构成了文件系统的目录结构.目录项是由内核维护的一个内存数据结构，所以通常也被叫做目录项缓存


### 万物皆文件的理解
其实文件就他吗的是一个interface{}, 里面实现了read() write()两个方法
- 系统调用处理VFS
- VFS对 磁盘/内存/网络存储做了虚拟化

### IO的多种类型
- 缓冲/非缓冲: 标准库是否缓存, 比如换行才输出
- 直接/非直接: 操作系统页缓存 还是直接flush
- 阻塞/非阻塞: 写函数/读函数 是否直接返回
- 同步/异步: #TODO

### IO栈
- 文件系统层: 包括虚拟文件系统和其他各种文件系统的具体实现。它为上层的应用程序，提供标准的文件访问接口；对下会通过通用块层，来存储和管理磁盘数据。
- 通用块层: 包括块设备 I/O 队列和 I/O 调度器。它会对文件系统的 I/O 请求进行排队，再通过重新排序和请求合并，然后才要发送给下一级的设备层。
- 设备层: 包括存储设备和相应的驱动程序，负责最终物理设备的 I/O 操作。


## 思路
- IO问题三板斧
    - top -> cpu iowait
    - pidstat -d 1 -> 磁盘较高的进程
    - strace -f -TT 系统读写调用频率和时间
    - lsof strace描述符文件


## 真实世界