
m和n最小公倍数
  任何数都可以表示成质因数积的形式
  Define q to be 1.
  for each prime number p less than m and n:
        find the largest a and b such that m % p^a == 0 and n % p^b == 0
        let q = q * p^max(a, b)
   return q


int gcd(int m, int n) {
  if (!m) return n;
  if (!n) return m;
  return gcd(n, m % n);
}
// 最小公倍数:
m * n / gcd(m, n)

10.1
You have a basketball hoop and someone says that you can play 1 of 2 games.
Game #1: You get one shot to make the hoop.
Game #2: You get three shots and you have to make 2 of 3 shots.
If p is the probability of making a particular shot, for which values of p should you pick one game or the other?

译文：
你有一个篮球架，可以在以下游戏中选择一个来玩。
游戏1：投一次球，进了算赢。
游戏2：投三次球，至少要进2个才算赢。
如果命中率是p，那么p是什么值时你会选游戏1来玩？或p是什么值时你会选择游戏2？


命中率是p，那么对于游戏1，赢的概率是p。对于游戏2，至少进2个球才算赢， 那么赢的概率为：
C(2,3) * p^2 * (1-p) + p^3 = 3p^2 - 2p^3  // bug fix， 开始时忘记+ p^3
哪个游戏赢的概率大我们就选哪个游戏，所以当
p > 3p^2 - 2p^3  ==>  p < 0.5
我们选游戏1。当p > 0.5时，我们选游戏2。当p = 0.5时，选哪个都可以，赢的概率相等。

10.2
There are three ants on different vertices of a triangle. What is the probability of collision
(between any two or all of them) if they start walking on the sides of the triangle?
Similarly find the probability of collision with ‘n’ ants on an ‘n’ vertex polygon.

首先，题目中没有提到蚂蚁运动的速度，所以认为它们的速度一样， 不会发生一只蚂蚁追上另一只蚂蚁的情况(面试时可以向面试官确认一下)。 对于任何1只蚂蚁，它有2条边可以选择来走。不会发生冲突的情况是， 所有的蚂蚁都顺时针走或是逆时针走，剩下的情况都是会冲突的。所以，冲突的概率为：

p = 1 - 2 * (1/2)^3 = 3/4
对于n只蚂蚁在n边形上，道理是一样的：

p = 1 - 2 * (1/2)^n = 1 - 1/2^(n-1)

10.3
Given two lines on a Cartesian plane, determine whether the two lines would intersect.
给定笛卡尔坐标系中的两条直线，判断两条线是否相交。

通过斜率判断 y = kx + b 斜率等于k
这道题目给出的条件非常有限。首先，我们要考虑一些边界条件，比如直线重合， 重合的话是否算相
交(假设我们把重合算作是相交)。还有当直线垂直于x轴时， 此时直线的斜率不存在，我们如果用斜率
和直线在y轴上的截距来表示直线的话， 这种情况怎么表示？接着是其它一般的情况，斜率不相等时，两
直线相交， 但我们不能假设斜率是整数，如果是浮点数，表示两个数不相等是让它们作差， 然后它们的
差大于一个足够小的数epsilon(一般为0.000001)。

判断叉积是不是0，如果是0再判断是否重合，不重合则不相交
// TODO guoliqiang 实现相关算法

10.5

Given two squares on a two dimensional plane, find a line that would cut these two squares in half.

给出二维平面上的两个正方形，找到一条直线能同时将两个正方形都分为面积相等的两半。
NB:
一条直线只要过正方形的中心，就一定会将它分为面积相等的两半。(矩形也一样) 那么，我们只要作一条过这两个正方形
中心点的直线， 即可同时把这两个正方形都分为面积相等的两半.


