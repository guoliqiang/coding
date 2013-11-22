// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-21 06:59:26
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// http://blog.csdn.net/sunmenggmail/article/details/8605538
// vector 的swap仅仅是交换了首尾指针
// 只有vec的clear不释放内存,其他容器clear都会释放内存
namespace algorithm {

void VecClear() {
  std::vector<int> vec(10000, 0);
  LOG(INFO) << "begin size:" << vec.capacity();
  for (int i = 0; i < 1000; i++) vec.pop_back();
  // 赋值和拷贝构造函数都不拷贝多余的空间
  std::vector<int> v2 = vec;
  LOG(INFO) << "v2 size:" << v2.size();
  
  std::vector<int> v3(vec);
  LOG(INFO) << "v3 size:" << v3.size();
  
  std::vector<int> v4;
  v4 = vec;
  LOG(INFO) << "v4 size:" << v4.size();
  
  LOG(INFO) << "afeter 1000 pop_back size :" << vec.capacity();
  std::vector<int>(vec).swap(vec);  // 利用了无名变量的作用域只是当前行
  LOG(INFO) << "afeter swap size:" << vec.capacity();
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  VecClear();
  return 0;
}
