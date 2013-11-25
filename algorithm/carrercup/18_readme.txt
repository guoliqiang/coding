Deadlock Conditions
1.   (资源独占)Mutual Exclusion: Only one process can use a resource at a given time.
2.   (请求保持)Hold and Wait: Processes already holding a resource can request new ones.
3.   (不可剥夺)No Preemption: One process cannot forcibly remove another process’ resource.
4.   (循环等待)Circular Wait: Two or more processes form a circular chain where each process is wait-
    ing on another resource in the chain.

Deadlock Prevention
Deadlock prevention essentially entails removing one of the above conditions, but many of
these conditions are difficult to satisfy. For instance, removing #1 is difficult because many
resources can only be used by one process at a time (printers, etc). Most deadlock preven-
tion algorithms focus on avoiding condition #4: circular wait.
If you aren’t familiar with these concepts, please read http://en.wikipedia.org/wiki/Deadlock.


18.1
What’s the difference between a thread and a process?

线程和进程的区别是什么？

这是一道出现频率极高的面试题，考察基本概念。

进程可以认为是程序执行时的一个实例。进程是系统进行资源分配的独立实体， 且每个进程拥有独立的地址空间。一个进程无法直接访问另一个进程的变量和数据结构， 如果希望让一个进程访问另一个进程的资源，需要使用进程间通信，比如：管道，文件， 套接字等。

一个进程可以拥有多个线程，每个线程使用其所属进程的栈空间。 线程与进程的一个主要区别是，同一进程内的多个线程会共享部分状态， 多个线程可以读写同一块内存(一个进程无法直接访问另一进程的内存)。同时， 每个线程还拥有自己的寄存器和栈，其它线程可以读写这些栈内存。

线程是进程的一个特定执行路径。当一个线程修改了进程中的资源， 它的兄弟线程可以立即看到这种变化。

以下是分点小结：
进程是系统进行资源分配的基本单位，有独立的内存地址空间；
线程是CPU调度的基本单位，没有单独地址空间，有独立的栈，局部变量，寄存器， 程序计数器等。

创建进程的开销大，包括创建虚拟地址空间等需要大量系统资源；
创建线程开销小，基本上只有一个内核对象和一个堆栈。

一个进程无法直接访问另一个进程的资源；同一进程内的多个线程共享进程的资源。
进程切换开销大，线程切换开销小；进程间通信开销大，线程间通信开销小。
线程属于进程，不能独立执行。每个进程至少要有一个线程，成为主线程

18.2
How can you measure the time spent in a context switch?

你如何测量一次上下文切换所需时间？

这是一个棘手的问题，让我们先从可能的解答入手。

上下文切换(有时也称为进程切换或任务切换)是指CPU 的控制权从一个进程或线程切换到另一个。 （参考资料） 例如让一个正在执行的进程进入等待状态(或终止它)，同时去执行另一个正在等待的进程。 上下文切换一般发生在多任务系统中，操作系统必须把等待进程的状态信息载入内存， 同时保存正在运行的进程的状态信息(因为它马上就要变成等待状态了)。

为了解决这个问题，我们需要记录两个进程切换时第一条指令和最后一条指令的时间戳， 上下文切换就是这两个时间戳的差。
来看一个简单的例子，假设只有两个进程：P1和P2。

P1正在执行，而P2在等待。在某个时刻，操作系统从P1切换到P2。假设此时， P1执行到第N条指令，记录时间戳Time_Stamp(P1_N)，当本来在等待的P2 开始执行第1条指令，说明切换完成，记录时间戳Time_Stamp(P2_1)。因此， 上下文切换的时间为：Time_Stamp(P2_1) – Time_Stamp(P1_N)

思路非常简单。问题在于，我们如何知道上下文切换是何时发生的？ 进程的切换是由操作系统的调度算法决定的。我们也无法记录进程中每个指令的时间戳。

另一个问题是：许多内核级线程也做上下文切换，而用户对于它们是没有任何控制权限的。

总而言之，我们可以认为，这最多只能是依赖于底层操作系统的近似计算。 一个近似的解法是记录一个进程结束时的时间戳，另一个进程开始的时间戳及排除等待时间。

如果所有进程总共用时为T，那么总的上下文切换时间为： T – (所有进程的等待时间和执行时间)

18.6
You are given a class with synchronized method A, and a normal method C. If you
have two threads in one instance of a program, can they call A at the same time? Can
they call A and C at the same time?
i) If you have two threads in one instance of a program, can they call A at the same time?
Not possible; read the above paragraph.

ii) Can they call A and C at the same time?
Yes. Only methods of the same object which are declared with the keyword synchronized
can’t be interleaved.

