// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-18 14:50:19
// File  : code.cc
// Brief :
// http://hi.baidu.com/aekdycoin/item/081cfd247d4e0fd3a517b6db

#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;

const int maxn = 205, maxm = maxn * maxn * 2, inf = 0x3f3f3f3f;
int head[maxn],l,next[maxm],vpt[maxm],flow[maxm];
int vh[maxn],d[maxn],pre[maxn],preh[maxn],now[maxn];
void init(){
l = 1;
memset(head, 0, sizeof head);
memset(d,0 , sizeof d);
} 

void add(int a,int b,int c){
vpt[++l]=b;next[l]=head[a];head[a]=l;flow[l]=c;
vpt[++l]=a;next[l]=head[b];head[b]=l;flow[l]=0;
}
int sap(int n,int s,int t){
int i,k,f,x,y,ans = 0;
for(i = 0; i < n; ++ i) now[i] = head[i];
vh[0] = n; x = s;
while(d[s] < n){
if(x == t){
for(f = inf, i = t; i ^ s; i = pre[i]) f = min(f, flow[preh[i]] ) ;ans += f;
for(i = t; i ^ s; i = pre[i]) flow[ preh[i]] -= f, flow[preh[i] ^ 1] += f; x = s;
}
for( i =now[x]; i; i = next[i]) if(flow[i] > 0 && d[y = vpt[i]] + 1 == d[x]) {now[x] = i; break;}
if(i) pre[y] = x, preh[y] = i, x = y; else{
if(!--vh[d[x]]) break; k = n;
for(i = now[x] = head[x]; i; i =next[i]) if(flow[i] > 0 && k > d[vpt[i]]) now[x] =i, k = d[vpt[i]];
++ vh[d[x] = k + 1]; if(x ^ s) x = pre[x];
}
}
return ans;
}

int n, m;
int s,t ;
int D[maxn];
int ID[maxn][maxn];
int lis[maxm][2],lim[maxm];
int Ans[maxm]; 

bool get(){ 
if( EOF == scanf("%d%d",&n,&m)) return 0;
memset(D, 0, sizeof D);
int i;
init () ;
s = 0;
t = n + 1;
for(i = 0; i < m; ++ i){
int a, b, l, r;
scanf("%d%d%d%d",&a,&b,&l,&r);
add(a, b, r - l) ;
lis[i][0] = a; lis[i][1] = b;
ID[a][b] = i ;
lim[i] = l;
D[b] += l;
D[a] -= l;
}
for(i = 1; i <= n; ++ i) if(D[i]){
if(D[i] > 0) add(s, i, D[i]) ; else add(i, t, - D[i]) ;
}
return 1;
}

void work(){
sap(t + 1, s, t) ;
bool flg = true ;
int i,j;
for(i = head[s]; i; i = next[i]) if(i % 2 == 0 && flow[i]) flg = false;
if(! flg) {
puts("NO");
return ;
}
puts("YES");
for(i = 1; i <= n; ++ i)
for(j = head[i]; j; j = next[j])
if(j % 2 ==0 && vpt[j] >= 1 && vpt[j] <= n)
Ans[ ID[i][vpt[j]] ] = flow[j ^ 1] + lim[ ID[i][vpt[j]] ];
for(i = 0; i < m; ++ i) printf("%d\n",Ans[i]);
}
int main(){
while(get()) work();
return 0;
}
