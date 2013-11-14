// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-13 19:10:49
// File  : code.cc
// Brief :

/*
 * 此种构图需要用TSP来做
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1001;
int matrix[MAXN][MAXN];
int visited[MAXN][MAXN];
int in[MAXN];
int out[MAXN];
int result[MAXN];
std::stack<int> stack;
int N = 0;
std::string dict[MAXN];

void DFS(int k) {
  for(int i = 0; i < N; i++) {
    if (matrix[k][i] > 0 && visited[k][i] == 0) {
      stack.push(i);
      visited[k][i] = 1;
      DFS(i);
    }
  }
}

void Felury() {
  memset(in, 0, sizeof(in));
  memset(out, 0, sizeof(out));
  memset(visited, 0, sizeof(visited));
  memset(result, -1, sizeof(result));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0) {
        out[i]++;
        in[j]++;
      }
    }
  }
  int idx_in = -1;
  int idx_out = -1;
  for (int i = 0; i < N; i++) {
    if (abs(in[i] - out[i]) > 1) {
      idx_in = -1;
      break;
    } else {
      if (in[i] == out[i] + 1) {
        if (idx_out != -1) {
          idx_in = -1;
          break;
        } else {
          idx_out = i;
        }
      } else if (out[i] == in[i] + 1) {
        if (idx_in != -1) {
          idx_in = -1;
          break;
        } else {
          idx_in = i;
        }
      }
    }
  }
  if (idx_in == -1) {
    printf("***\n");
    return;
  };
  while (!stack.empty()) stack.pop();

  stack.push(idx_in);
  while (!stack.empty()) {
    int t = stack.top();
    int bridge = 1;
    for (int i = 0; i < N; i++) {
      if (matrix[t][i] > 0 && visited[t][i] == 0) {
        bridge = 0;
        break;
      }
    }
    if (bridge == 0) {
      DFS(t);
    } else {
      printf("%s.", dict[stack.top()].c_str());
      stack.pop();
    }
  }
  printf("\n");
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    int n = 0;
    scanf("%d", &n);
    getchar();
    N = n;
    char temp[25];
    for (int j = 0; j < n; j++) {
      scanf("%s", temp);
      dict[j] = std::string(temp);
    }
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i != j && dict[i][dict[i].size() - 1] == dict[j][0]) {
          matrix[i][j] = 1;
          // LOG(INFO) << "matrix[" << i << "][" << j << "]:" << 1;
        }
      }
    }
    Felury();
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
