12.1
If you were integrating a feed of end of day stock price information ( open, high, low,and closing price) 
for 5,000 companies, how would you do it? You are responsible for the development, rollout and ongoing
monitoring and maintenance of the feed. Describe the different methods you considered and why you would
recommend your approach. The feed is delivered once per trading day in a comma-separated format via an FTP
site. The feed will be used by 1000 daily users in a web application.


如果你要为5000家公司的股价信息整合摘要，你会怎么做？ 你要负责摘要的开发，部署，监控和维护。描述你能想到的
不同方法， 及为什么你会推荐这些方法。摘要以逗号分隔的格式经由FTP进行交付，每个交易日一次。 每日有1000个用
户在web应用程序中使用这些摘要信息。


假设我们有一些脚本在每日结束时通过FTP获取数据。我们把数据存储在哪？ 我们怎样存储数据有助于我们对数据进行分析？

方案一
将数据保存在文本文件中。这样子的话，管理和更新都非常麻烦，而且难以查询。 保存无组织的文本文件是一种非常低效的方法。

方案二
使用数据库。这将带来以下的好处：

数据的逻辑存储
提供了非常便捷的数据查询方式
例子：return all stocks having open > N AND closing price < M (返回开盘价大于N且收盘价小于M的所有股票)

优势：
使得维护更加简单
标准数据库提供了回滚，数据备份和安全保证等功能。我们不需要重复造轮子。
方案三

如果要求不是那么宽泛，我们只想做简单的分析和数据分发，那么XML是另一个很好的选择。 我们的数据有固定的格式和大小：公司名称，开盘价，最高价，最低价和收盘价。 XML看起来应当如下所示：

<root>
  <date value=“2008-10-12”>
    <company name=“foo”>
      <open>126.23</open>
      <high>130.27</high>
      <low>122.83</low>
      <closingPrice>127.30</closingPrice>
    </company>
    <company name=“bar”>
      <open>52.73</open>
      <high>60.27</high>
      <low>50.29</low>
      <closingPrice>54.91</closingPrice>
    </company>
  </date>
  <date value=“2008-10-11”> . . . </date>
</root>
优势：

便于数据分发。这就是为什么XML是共享及分发数据的一个标准模型。
我们有高效的解析器来提取出想要的数据
我们可以往XML文件中追加新数据
不足之处是数据查询比较麻烦(这点比不上数据

12.2
How would you design the data structures for a very large social network (Facebook,LinkedIn, etc)?
Describe how you would design an algorithm to show the connection, or path,
between two people (e.g., Me –> Bob –> Susan –> Jason –> You).

你会怎样给一个非常大型的社交网站设计数据结构（比如Facebook，LinkedIn）？ 设计一个算法来找到任意两个
人之间的联系，比如：我 –> Bob –> Susan –> Jason –> 你

方法：

首先，我们先不去考虑数据规模。先从简单的入手。

我们可以把每个人看作一个结点，如果两个人之间是朋友，则这两个结点间有一条边， 这样一来我们就可以构建
出一个图。假设我们将“人”这个类设计如下：
class Person {
    Person[] friends;
    // Other info
}
如果要找到两个人之间的联系，即两个人之间间隔着哪些人。我们就从其中的一个人开始，
做广度优先搜索(BFS)。（做双向的BFS会更快）

但是。。。数据规模太大了！

如果我们去处理Orkut或是Facebook上的数据，单台机器根本无法完成这个任务。 因此，我们考虑用多台机器来
处理这个问题。这样一来，Person这个类就需要修改了。 在Person类中，我们存储朋友的ID，然后按照以下方式找到朋友：

对于每个朋友id：int machine_index = lookupMachineForUserID(id);
转到标号为machine_index的机器去
Person friend = lookupFriend(machine_index);
对于这个问题，要考虑的优化和问题非常多，这里只列出一些。

优化：减少机器间的跳转次数
机器间的跳转是非常耗时的，因此我们不随机的跳转，而是进行批处理： 比如一个人，他其中的5个朋友在
同一台机器上，那么跳转到那台机器后，一次性处理他们。

聪明地划分人与机器
由于同一国家的人更有可能是朋友，因此我们并不随机地把人分配到不同的机器上， 而是以国家，城市，州等进行划分。这样也可以减少机器间的跳转次数。
问题：广度优先搜索会标记已访问结点，对于这个问题，你怎么处理？
在这个问题中，有可能同时有许多人在搜索两人间的联系， 因此直接在原数据上做修改并不好。那怎么办呢？我们可以对每一个搜索， 使用一个哈希表来记录一个结点是否已经访问过。这样一来， 不同人的搜索之间就不会互相干扰。

其它问题

在真实的世界中，服务器有可能会出故障。你会怎么做？
你怎么利用缓存？
你是否一直搜索直到把图上的结点都遍历一次。如何决定什么时间停止搜索。
在真实世界中，你的朋友里，有一些人的朋友会更多。 那么通过他是否更有可能让你找到与特定某个人的联系。 你怎么利用这个数据来选择遍历的顺序。
下面是算法的示例代码：（java的，就不写成C++了）

public class Server { 
    ArrayList<Machine> machines = new ArrayList<Machine>();
}
public class Machine {
    public ArrayList<Person> persons = new ArrayList<Person>();
    public int machineID;
}

public class Person {    
    private ArrayList<Integer> friends;
    private int ID;
    private int machineID;
    private String info;
    private Server server = new Server();
    
    public String getInfo() { return info; }
    
    public void setInfo(String info) {
        this.info = info;
    }
    
    public int[] getFriends() {
        int[] temp = new int[friends.size()];
        for (int i = 0; i < temp.length; i++) {
            temp[i] = friends.get(i);
        }
        return temp;
    }
    
    
    
    public int getID() { return ID; }
    public int getMachineID() { return machineID; }
    public void addFriend(int id) { friends.add(id); }
    
    
    // Look up a person given their ID and Machine ID
    
    public Person lookUpFriend(int machineID, int ID) {
        for (Machine m : server.machines) {
            if (m.machineID == machineID) {
                for (Person p : m.persons) {
                    if (p.ID == ID){
                        return p;                      
                    }
                }
            }
        }
        return null;
    }
    
    
    // Look up a machine given the machine ID
    
    public Machine lookUpMachine(int machineID) {
        for (Machine m:server.machines) {
            if (m.machineID == machineID)
                return m;
        }
        return null;
    }
    
    
    public Person(int iD, int machineID) {
        ID = iD;
        this.machineID = machineID;
    }
}

12.3
Given an input file with four billion integers, provide an algorithm to generate an integer
which is not contained in the file. Assume you have 1 GB of memory.


What if you have only 10 MB of memory?


给你一个文件，里面包含40亿个整数，写一个算法找出该文件中不包含的一个整数， 假设你有1GB内存可用。

如果你只有10MB的内存呢？

解答

我们先来做个算术题，40亿个整数大概有多大？

40 * 10^8 * 4B = 16GB (大约值，因为不是按照2的幂来做单位换算)
这个明显无法一次性装入内存中。但是，如果我们用计算机中的一位来表示某个数出现与否， 就可以减少内存使用量。比如在一块连续的内存区域，15出现，则将第15位置1。 这个就是Bit Map算法。关于这个算法，网上有篇文章写得挺通俗易懂的，推荐：

http://blog.csdn.net/hguisu/article/details/7880288

如果用Bit Map算法，一个整数用一位表示出现与否，需要的内存大小是：

40 * 10^8 b = 5 * 10^8 B = 0.5GB
而我们有1GB的内存，因此该算法可行。由于Bit Map只能处理非负数， (没有说在第-1位上置1的)，因此对于有符号整数，可以将所有的数加上0x7FFFFFFF， 将数据移动到正半轴，找到一个满足条件的数再减去0x7FFFFFFF即可。 因此本文只考虑无符号整数，对于有负数的情况，按照上面的方法处理即可。

我们遍历一遍文件，将出现的数对应的那一位置1，然后遍历这些位， 找到第一个有0的位即可，这一位对应的数没有出现。代码如下：

/*
应该这么计算
int32的正数需要2^32/8 = 2^29 = 0.5G的内存
*/

#include <iostream>
#include <cstdio>
using namespace std;


int main(){
    // freopen("12.3.in", "w", stdout);
    // int miss = 12345;
    // for(int i=0; i<20000; ++i){
    //     if(i == miss) continue;
    //     cout<<i<<endl;
    // }
    // fclose(stdout);
    
    freopen("12.3.in", "r", stdin);
    
    int int_len = sizeof(int) * 8;
    int bit_len = 0xFFFFFFFF / int_len;
    int* bit = new int[bit_len];
    int v;
    while(scanf("%d", &v) != EOF){
        bit[v/int_len] |= 1<<(v%int_len);
    }
    bool found = false;
    for(int i=0; i<bit_len; ++i){
        for(int j=0; j<int_len; ++j){
            if((bit[i] & (1<<j)) == 0){
                cout<<i*int_len + j<<endl;
                found = true;
                break;
            }
                
        }
        if(found) break;
    }
    
    delete[] bit;
    fclose(stdin);
    return 0;
}
第二问，如果我们只有10MB的内存，明显使用Bit Map也无济于事了。 内存这么小，注定只能分块处理。我们可以将这么多的数据分成许多块， 比如每一个块的大小是1000，那么第一块保存的就是0到999的数，第2块保存的就是1000 到1999的数……实际上我们并不保存这些数，而是给每一个块设置一个计数器。 这样每读入一个数，我们就在它所在的块对应的计数器加1。处理结束之后， 我们找到一个块，它的计数器值小于块大小(1000)， 说明了这一段里面一定有数字是文件中所不包含的。然后我们单独处理这个块即可。

接下来我们就可以用Bit Map算法了。我们再遍历一遍数据， 把落在这个块的数对应的位置1(我们要先把这个数归约到0到blocksize之间)。 最后我们找到这个块中第一个为0的位，其对应的数就是一个没有出现在该文件中的数。 代码如下：

/*
有ｂｕｇ
因为有可能数据重复出现造成每一块的计数器都大于1000
*/
#include <iostream>
#include <cstdio>
using namespace std;

int main(){
    freopen("12.3.in", "r", stdin);// 20000 number
    int int_len = sizeof(int) * 8;
    int totalnum = 20000;
    int blocksize = 2000;
    int blocknum = totalnum / blocksize;
    int* block = new int[blocknum];
    int* bit = new int[blocksize/int_len+1];
    int v;
    while(scanf("%d", &v) != EOF){
        ++block[v/blocksize];
    }
    fclose(stdin);
    int start;
    for(int i=0; i<blocknum; ++i){
        if(block[i] < blocksize){
            start = i * blocksize;
            break;
        }
    }
    freopen("12.3.in", "r", stdin);
    while(scanf("%d", &v) != EOF){
        if(v>=start && v<start+blocksize){
            v -= start; // make v in [0, blocksize)
            bit[v/int_len] |= 1<<(v%int_len);
        }
    }

    bool found = false;
    for(int i=0; i<blocksize/int_len+1; ++i){
        for(int j=0; j<int_len; ++j){
            if((bit[i] & (1<<j)) == 0){
                cout<<i*int_len+j+start<<endl;
                found = true;
                break;
            }
        }
        if(found) break;
    }

    delete[] block;
    delete[] bit;
    fclose(stdin);
    return 0;
}

12.5

小数据可以不用bloomfilter
If you were designing a web crawler, how would you avoid getting into infinite loops?


如果让你设计一个网络爬虫，你怎么避免陷入无限循环？

看完这题，建议用python写个爬虫，对此就能理解的多一些，而且还可以做出好玩的东西。
话说爬虫为什么会陷入循环呢？答案很简单，当我们重新去解析一个已经解析过的网页时， 就会陷入无限循环。这意味着
我们会重新访问那个网页的所有链接， 然后不久后又会访问到这个网页。最简单的例子就是，网页A包含了网页B的链接， 
而网页B又包含了网页A的链接，那它们之间就会形成一个闭环。
那么我们怎样防止访问已经访问过的页面呢，答案也很简单，设置一个标志即可。 整个互联网就是一个图结构，我们
通常使用DFS(深度优先搜索)和BFS(广度优先搜索) 进行遍历。所以，像遍历一个简单的图一样，将访问过的结点标记一下即可。


12.6

在允许有少量误差的情况下完全可以使用bloomfilter

You have a billion urls, where each is a huge page. How do you detect the duplicate documents?

译文：

你有10亿个url，每个url对应一个非常大的网页。你怎么检测重复的网页？

下面的方法是没有误差的。

网页大，数量多，要把它们载入内存是不现实的。 因此我们需要一个更简短的方式来表示这些网页。而hash表正是干这事的。 我们将网页内容做哈希，而不是url，这里不同url可能对应相同的网页内容。

将每个网页转换为一个哈希值后，我们就得到了10亿个哈希值， 很明显，两两对比也是非常耗时的O(n2 )。因此我们需要使用其它高效的方法。

根据以上分析，我们可以推出满足条件的以下算法：

遍历网页，并计算每个网页的哈希值。
检查哈希值是否已经在哈希表中，如果是，说明其网页内容是重复的，输出其url。 否则保存url，并将哈希值插入哈希表。
通过这种方法我们可以得到一组url，其对应的网页内容都是唯一的。但有一个问题， 一台计算机可以完成以上任务吗？

一个网页我们要花费多少存储空间？

每个网页转换成一个4字节的哈希值
假设一个url平均是30个字符，那我们至少需要30个字节
因此对应一个url，我们一共要用掉34个字节
34字节 * 10亿 = 31.6GB。很明显，单机的内存是无法搞定的。

我们要如何解决这个问题？

我们可以将这些数据分成多个文件放在磁盘中，分次载入内存处理。 这样一来我们要解决的就是文件的载入/载出问题。

我们可以通过哈希的方式将数据保存在不同文件，这样一来，大小就不是问题了， 但存取时间就成了问题。硬盘上的哈希表随机读写要耗费较多的时间， 主要花费在寻道及旋转延迟上。关于这个问题， 可以使用电梯调度算法来消除磁头在磁道间的随机移动，以此减少消耗的时间。

我们可以使用多台机器来处理这些数据。这样一来，我们要面对的就是网络延迟。 假如我们有n台机器。

首先，我们对网页做哈希，得到一个哈希值v
v%n 决定这个网页的哈希值会存放在哪台机器
v/n 决定这个哈希值存放在该机器上哈希表的位置

12.7

You have to design a database that can store terabytes of data. It should support
efficient range queries. How would you do it?

让你来设计一个能存储TB级数据的数据库，而且要能支持高效的区间查询(范围查询)， 你会怎么做？


首先要明确，并不是所有的字段都可以做区间查询。比如对于一个员工， 性别就没有所谓的区间查询；而工资是
可以做区间查询的， 例如查询工资大于a元而小于b元的员工。

我们将需要做区间查询的字段对应的字段值提取出来作为关键字构建一棵B+树， 同时保存其对应记录的索引。
B+树会对关键字排序，这样我们就可以进行高效的插入， 搜索和删除等操作。我们给定一个查询区间，在B+ 
树中找到对应区间开始的结点只需要O(h)的时间，其中h是树高，一般来说都很小。 叶子结点保存着记录的索引，
而且是按关键字(字段值)排好序的。 当我们找到了对应区间开始的叶子结点，再依次从其下一个块中
找出对应数量的记录， 直到到到查询区间右端(即最大值)为止。这一步的时间复杂度由其区间中元素数量决定。

避免使用将数据保存在内部结点的树(B+树将数据都保存在叶子结点)， 这样会导致遍历树的开销过大(因为树并非常驻内存)。

讲得这么抽象，还是来张图辅助理解一下吧。



假设这棵B+树上对应的数字表示工资，单位千元。员工对应的工资数据， 其实就都保存在叶子结点上，
内部结点和根结点保存的只是其子结点数据的最大值。 这里假设每个叶子结点上的工资值所在
的那条记录索引并没有画出来。OK， 现在我们要查询工资大于25k小于60k的员工记录。

区间的开始值是25，我们访问根结点，发现25小于59，于是向左子结点走。 然后继续。发现25大于15且
小于44，于是向其中间子结点走。 最后在叶子结点查找到第一个大于25的值是37。接下来再依次地将结点
内部的其他值 (44)，和它下一个叶子结点的值(51,59)对应的记录返回(不再往下查找， 因为下面的数已经
大于60)。这样一来，即实现了高效的区间查询。

