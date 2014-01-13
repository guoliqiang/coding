// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 11:56:12
// File  : code.cc
// Brief :

/*
观察着模型，适用于发布-订阅需求(publis - subscribe)
比如fb和renren登录的时候都能看到好友的最近的更新信息
这种情况每个user即是观察着又是目标

fb目前采用的是memcache
http://www.careercup.com/question?id=14712676
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct User;

struct Msg {
  int time;
  User * user;
  std::string val;
};

struct User{
  int uid;
  std::list<Msg> self_update;
  std::list<Msg * > friends_update;
  std::list<User *> friends;
};

/*
每次用户自己更新时，除了更新self_update，以外还需要将此Msg的指针加入到
firends的friends_update中，这样friend登录的时候就可以看到自己的更新了
http://www.careercup.com/question?id=15501978
这是推模式，更新的信息主动推送给friends，适用于好友不多的情况，但如果好友很多的话
就不合适了，比如姚晨的粉丝很多，但僵尸粉也不少，这样每次姚晨更新状态都需要推送给很多粉丝
此时可以采用拉的模式，粉丝登录的时候主动到friends中拉来更新信息。
*/
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
