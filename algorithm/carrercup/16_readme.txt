16.1
Explain the following terms: virtual memory, page fault, thrashing.

解释下列术语：虚拟内存，缺页中断，抖动。

虚拟内存是计算机系统内存管理的一种技术。它使得应用程序认为它拥有连续的可用的内存 （一个连续完整的地址空间），而实际上，它通常是被分隔成多个物理内存碎片， 还有部分暂时存储在外部磁盘存储器上，在需要时进行数据交换。 与没有使用虚拟内存技术的系统相比，使用这种技术的系统使得大型程序的编写变得更容易， 对真正的物理内存（例如RAM）的使用也更有效率。

http://zh.wikipedia.org/wiki/%E8%99%9A%E6%8B%9F%E5%86%85%E5%AD%98

缺页中断一个页(Page)是一个固定容量的内存区块，是物理内存和外部存储(如硬盘等) 传输的单位。当一个程序访问一个映射到地址空间却实际并未加载到物理内存的页（page）时， 硬件向软件发出的一次中断（或异常）就是一个缺页中断或叫页错误（page fault）。

http://zh.wikipedia.org/wiki/%E9%A1%B5%E7%BC%BA%E5%A4%B1_(%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%A7%91%E5%AD%A6)

抖动在分页存储管理系统中，内存中只存放了那些经常使用的页面， 而其它页面则存放在外存中，当进程运行需要的内容不在内存时， 便启动磁盘读操作将所需内容调入内存，若内存中没有空闲物理块， 还需要将内存中的某页面置换出去。也就是说，系统需要不断地在内外存之间交换信息。 若在系统运行过程中，刚被淘汰出内存的页面，过后不久又要访问它， 需要再次将其调入。而该页面调入内存后不久又再次被淘汰出内存，然后又要访问它。 如此反复，使得系统把大部分时间用在了页面的调入/换出上， 而几乎不能完成任何有效的工作，这种现象称为抖动。

参考资料：《操作系统教程》 http://en.wikipedia.org/wiki/Thrash_(computer_science)

16.2
What is a Branch Target buffer? Explain how it can be used in reducing bubble cycles
in cases of branch misprediction.

Branch misprediction occurs when the CPU mispredicts the next instruction to be executed.
The CPU uses pipelining which allows several instructions to be processed simultaneously.
But during a conditional jump, the next instruction to be executed depends on the result of
the condition. Branch Prediction tries to guess the next instruction. However, if the guess is
wrong, we are penalized because the instruction which was executed must be discarded.
Branch Target Buffer (BTB) reduces the penalty by predicting the path of the branch, comput-
ing the target of the branch and caching the information used by the branch. There will be
no stalls if the branch entry found on BTB and the prediction is correct, otherwise the penalty
will be at least two cycles.
http://hi.baidu.com/dbfr2011818/item/a3ae23ea70592ce1fb42ba0b

16.3
Describe direct memory access (DMA). Can a user level buffer / pointer be used by
kernel or drivers?

Direct Memory is a feature which provides direct access (read/write) to system memory with-
out interaction from the CPU. The “DMA Controller” manages this by requesting the System
bus access (DMA request) from CPU. CPU completes its current task and grants access by as-
serting DMA acknowledgement signal. Once it gets the access, it reads/writes the data and
returns back the system bus to the CPU by asserting the bus release signal. This transfer is
faster than the usual transfer by CPU. Between this time CPU is involved with processing task
which doesn’t require memory access.
By using DMA, drivers can access the memory allocated to the user level buffer / pointer.

16.4
Write a step by step execution of things that happen after a user presses a key on the
keyboard. Use as much detail as possible

1.   The keyboard sends a scan code of the key to the keyboard controller (Scan code for
     key pressed and key released is different).
2.   The keyboard controller interprets the scan code and stores it in a buffer.
3.   The keyboard controller sends a hardware interrupt to the processor. This is done by
     putting signal on “interrupt request line”: IRQ 1.
4.   The interrupt controller maps IRQ 1 into INT 9.
5.   An interrupt is a signal which tells the processor to stop what it was doing currently
     and do some special task.
6.   The processor invokes the “Interrupt handler”. CPU fetches the address of “Interrupt
     Service Routine” (ISR) from “Interrupt Vector Table” maintained by the OS (Processor
     use the IRQ number for this).
7.   The ISR reads the scan code from port 60h and decides whether to process it or pass
     the control to program for taking action.
 

16.6
Discuss how would you make sure that a process doesn’t access an unauthorized part
of the stack.

NATIVE CODE
One threat to the stack is malicious program input, which can overflow a buffer and over-
write stack pointers, thus circumventing the intended execution of the program.
If the interviewer is looking for a simple method to reduce the risk of buffer overflows in
native code, modern compilers provide this sort of stack protection through a command
line option. With Microsoft’s CL, you just pass /GS to the compiler. With GCC, you can use
-fstack-protector-all.
For more complex schemes, you could set individual permissions on the range of memory
pages representing the stack section you care about. In the Win32 API, you’d use the Virtu-
alProtect API to mark the page PAGE_READONLY or PAGE_NOACCESS. This will cause the
code accessing the region to go through an exception on access to the specific section of
the stack.
Alternately, you could use the HW Debug Registers (DRs) to set a read or write breakpoint
on the specific memory addresses of interest. A separate process could be used to debug
the process of interest, catch the HW exception that would be generated if this section of the
stack were accessed.
However, it’s very important to note that under normal circumstances, threads and processes
are not means of access control. Nothing can prevent native code from writing anywhere
within the address space of its process, including to the stack. Specifically, there is nothing
to prevent malicious code in the process from calling VirtualProtect and marking the stack
sections of interest PAGE_EXECUTE_READWRITE. Equally so, nothing prevents code from
zeroing out the HW debug registers, eliminating your breakpoints. In summary, nothing can
fully prevent native code from accessing memory addresses, including the stack, within its
own process space.
MANAGED CODE
A final option is to consider requiring this code that should be “sandboxed” to run in a man-
aged language like Java or C# / .NET. By default, the virtual machines running managed code
in these languages make it impossible to gain complete access to the stack from within the
process.
One can use further security features of the runtimes to prevent the code from spawning ad-
ditional processes or running “unsafe” code to inspect the stack. With .NET, for example, you
can use Code Access Security (CAS) or appdomain permissions to control such execution.

16.7
What are the best practices to prevent reverse engineering of DLLs?
Best practices include the following:
»»   Use obfuscators.
»»   Do not store any data (string, etc) in open form. Always compress or encode it.
»»   Use a static link so there is no DLL to attack.
»»   Strip all symbols.
»»   Use a .DEF file and an import library to have anonymous exports known only by their
     export ids.
»»   Keep the DLL in a resource and expose it in the file system (under a suitably obscure
     name, perhaps even generated at run time) only when running.
»»   Hide all real functions behind a factory method that exchanges a secret (better, proof of
     knowledge of a secret) for a table of function pointers to the real methods.
»»   Use anti-debugging techniques borrowed from the malware world to prevent reverse
     engineering. (Note that this will likely get you false positives from AV tools.)
»»   Use protectors.
16.8
A device boots with an empty FIFO queue. In the first 400 ns period after startup,
and in each subsequent 400 ns period, a maximum of 80 words will be written to the
queue. Each write takes 4 ns. A worker thread requires 3 ns to read a word, and 2 ns to
process it before reading the next word. What is the shortest depth of the FIFO such
that no data is lost? 

THEORY
First, note that writes do not have to be evenly distributed within a period. Thus a likely worst
case is 80 words are written at the end of the first period, followed by 80 more at the start of
the next.
Note that the maximum write rate for a full period is exactly matched by a full period of pro-
cessing (400 ns / ((3 ns + 2 ns)/process) = 80 processed words/period).
As the 2nd period in our example is fully saturated, adding writes from a 3rd period would
not add additional stress, and this example is a true worst case for the conditions.
A SAFE QUEUE DEPTH
For an estimate of maximum queue size, notice that these 160 writes take 640 ns (160 writes
* 4 ns / write = 640 ns), during which time only 128 words have been read (640 ns / ((3 ns + 2
ns) / word) = 128 words). However, the first read cannot start until the first write has finished,
which fills an extra slot in the queue.
Also, depending on the interactions between read and write timing, a second additional slot
may be necessary to ensure a write does not trash the contents of a concurrently occurring
read. Thus, a safe estimate is that the queue must be at least 34 words deep (160 - 128 + 1 +
1 = 34) to accommodate the unread words

As an interesting note, if the problem had required only 2 ns of the 5 ns processing time to
complete a read, the optimal queue depth would decrease to 33.

