// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-25 00:00:07
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string IntToString(int v) {
  std::string ans;
  while (v > 0) {
    ans.push_back('0' + v % 10);
    v /= 10;
  }
  for (int i = ans.size(); i < 2; i++) {
    ans.push_back('0');
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}

std::string GetPwd() {
  int num = 90;
  int count = 0;
  std::string ans = "9";
  std::set<int> visit;

  while (visit.size() < 100) {
    ans.append(1, '0' + num % 10);
    if (visit.count(num)) LOG(INFO) << "dup " << IntToString(num);
    visit.insert(num);
    LOG(INFO) << IntToString(num);
    num *= 10;
    num %= 100;
    int add = 0;
    while (add < 9) {
      if (!visit.count(num + add)) break;
      add++;
    }
    num += (add % 10);
    count++;
  }
  LOG(INFO) << count;
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

namespace DFS {
int matrix[100][100];

bool DFS(std::set<int> & visited, std::vector<int> & path, int v) {
  path.push_back(v);
  visited.insert(v);

  if (path.size() == 100) return true;
  for (int i = 0; i < 100; i++) {
    if (matrix[v][i] == 1 && !visited.count(i)) {
      if (DFS(visited, path, i)) return true;
    }
  }
  visited.erase(v);
  path.pop_back();
  return false;
}

void Get() {
  memset(matrix, 0, sizeof(int) * 100 * 100);
  for (int i = 0; i < 100; i++) {
    int t = (i % 10) * 10;
    for (int j = 0; j < 10; j++) {
      matrix[i][t + j] = 1;
    }
  }
  std::vector<int> path;
  std::set<int> visited;
  LOG(INFO) << DFS(visited, path, 99);
  LOG(INFO) << path.size() << " " << JoinVector(path);
}

}  // namespace DFS

int main(int argc, char** argv) {
  DFS::Get();
  return 0;
  std::string ans = GetPwd();
  LOG(INFO) << ans << " size=" << ans.size();
  return 0;
}
