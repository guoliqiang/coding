// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-16 05:28:34
// File  : code.cc
// Brief :

/*
 * Accepted 30112K  1657MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 500010;
int vec[MAXN];

int offset[MAXN];
std::string name[MAXN];
int N = 0;

struct Node {
  int l;
  int r;
  int count;
};

Node tr[MAXN * 3];  // note multi 3

void Build(int l, int r, int k) {
  tr[k].l = l;
  tr[k].r = r;
  tr[k].count = r - l + 1;
  if (l == r) return;
  else {
    int mid = l + (r - l) / 2;
    Build(l, mid, 2 * k);
    Build(mid + 1, r, 2 * k + 1);
  }
}

int Delete(int n, int k) {
  if (n > tr[k].count) {
    return -1;
  }
  tr[k].count--;
  if (tr[k].r == tr[k].l) return tr[k].r;
  else {
    if (tr[2 * k].count >= n) {
      return Delete(n, 2 * k);
    } else {
      return Delete(n - tr[2 * k].count, 2 * k + 1);
    }
  }
}


int MaxP() {
  memset(vec, 0, sizeof(vec));
  int index = 1;
  int min = vec[0];
  for (int i = 2; i <= N; i++) {
    for (int j = i; j <= N; j += i) {
      vec[j]++;
      if (vec[j] > min) {
        min = vec[j];
        index = j;
      }
    }
  }
  return index;
}

void Read() {
  int n, k;
  while (scanf("%d%d", &n, &k) != EOF) {
    getchar();
    N = n;
    memset(offset, 0, sizeof(offset));
    for (int i = 0; i < n; i++) {
      char temp[20];
      int off;
      scanf("%s%d", temp, &off);
      getchar();
      name[i + 1] = std::string(temp);
      offset[i + 1] = off;
    }
    int p = MaxP();
    Build(1, n, 1);
    int pos = 0;
    offset[pos] = 0;
    for(int i = 0; i < p; i++) {  
      if (offset[pos] > 0) {  
        k = ((k + offset[pos] - 2) % tr[1].count + tr[1].count) % tr[1].count + 1;
      } else {  
        k = ((k + offset[pos] - 1) % tr[1].count + tr[1].count) % tr[1].count + 1;  
      }  
      pos = Delete(k, 1);  
    }
    printf("%s %d\n", name[pos].c_str(), vec[p] + 1);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
