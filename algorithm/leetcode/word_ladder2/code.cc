// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-01 14:01:59
// File  : code.cc
// Brief :

/*
Given two words (start and end), and a dictionary, find all shortest transformation sequence(s) from start to end, such that:

Only one letter can be changed at a time
Each intermediate word must exist in the dictionary
For example,

Given:
start = "hit"
end = "cog"
dict = ["hot","dot","dog","lot","log"]
Return
  [
    ["hit","hot","dot","dog","cog"],
    ["hit","hot","lot","log","cog"]
  ]
Note:
All words have the same length.
All words contain only lowercase alphabetic characters.
*/

/*
 * 自己认为用dijastra 和 trace的方法很好，但会T
 * 原因是 large test 里有个2w词的case
 * */

#include <unordered_set>
#include <unordered_map>
#include "base/public/common_head.h"


namespace NB {
using namespace std;

class Solution {
  vector<string> vdict;
  vector<vector<int> > adj;

  void build(unordered_set<string> &dict){
    vdict.clear();
    unordered_map<string, int> ids;
    for(auto it = dict.begin(); it != dict.end(); it++){
        ids[*it] = vdict.size();
        vdict.push_back(*it);
    }
    adj.clear();
    adj.resize(vdict.size());
    for (int i = 0; i < vdict.size(); i++){
      string w = vdict[i];
      for (int j = 0; j < vdict[i].size(); j++){
        for (char c = 'a'; c <= 'z'; c++)
          if (c != vdict[i][j]){
             w[j] = c;
             if (ids.count(w)) adj[i].push_back(ids[w]);
             w[j] = vdict[i][j];
          }
      }
    }
  }

  void gen(int v1, int v2, vector<vector<int> >& prev, vector<int>& path, vector<vector<string> >&ans){
    path.push_back(v2);
    if (v2 == v1 and path.size() > 1){
      ans.push_back(vector<string>());
      for (auto rit = path.rbegin(); rit != path.rend(); rit++){
          ans.back().push_back(vdict[*rit]);
      }
    } else {
      int i;
      for (i = 0; i < prev[v2].size(); i++){
        gen(v1, prev[v2][i], prev, path, ans);
      }
    }
    path.pop_back();
  }

public:
  vector<vector<string> > findLadders(string start, string end, unordered_set<string> &dict) {
    dict.insert(start);
    dict.insert(end);
    build(dict);
    queue<int> que;
    vector<vector<int> > prev(vdict.size());
    vector<int> distance(vdict.size());

    int v, v1, v2;
    for (v1 = 0; vdict[v1] != start; v1++);
    for (v2 = 0; vdict[v2] != end; v2++);
    for (int i = 0; i < adj[v1].size(); i++){
      v = adj[v1][i];
      que.push(v);
      prev[v].push_back(v1);
      distance[v] = 1;
    }
    while (!que.empty()){
      int v1 = que.front(); que.pop();
      if(v1 == v2) break;
      int d = distance[v1] + 1;
      for(int i = 0; i < adj[v1].size(); i++){
        v = adj[v1][i];
        if(prev[v].size() == 0){
          prev[v].clear();
          prev[v].push_back(v1);
          distance[v] = d;
          que.push(v);
        }else if(distance[v] == d){
          prev[v].push_back(v1);
        }
      }
    }
    vector<vector<string> > ans;
    vector<int> path;
    gen(v1, v2, prev, path, ans);
    return ans;
  }

};
}  // namespace NB

namespace NB {

using namespace std;

int LadderLength(string start, string end, unordered_set<string> &dict) {
  int ret = 0;
  if (start == end) return ret;
  unordered_set<string> visited;
  queue<string> que;
  int lev1 = 1;
  int lev2 = 0;
  
  que.push(start);
  visited.insert(start);

  while (!que.empty()) {
    string s = que.front();
    que.pop();
    lev1--;

    for (int i = 0; i < s.length(); i++) {
      for (int j = 0; j < 26; ++j) {
        string t = s;
        t[i] = 'a' + j;
        if (t == end) return ret + 2;
        if (dict.find(t) != dict.end() &&
            visited.find(t) == visited.end()) {
           que.push(t);
           visited.insert(t);
           lev2++;
         }
       }
     }
     if (lev1 == 0) {
        ret++;
        lev1 = lev2;
        lev2 = 0;
     }
   }
   return 0;
}



}  // namespace NB

namespace algorithm {

/*
 * 还是 T
 * */

class BitSet {
 public:
  BitSet(int len) : len_(len) {
    ptr_ = new char[len_ / 8 + 1];
    memset(ptr_, 0, sizeof(char) * (len_ / 8 + 1));
  }

  bool IsSet(int i) {
    char * p = ptr_ + (i / 8);
    char foo = (1 << (i % 8));
    return (foo & (*p));
  }

  void Set(int i) {
    char * p = ptr_ + (i / 8);
    char foo = (1 << (i % 8));
    (*p) |= foo;
  }

  void Clear(int i) {
    char * p = ptr_ + (i / 8);
    char foo = ~(1 << (i % 8));
    (*p) &= foo;
  }

  ~BitSet() {
    delete [] ptr_;
  }

 private:
  int len_;
  char * ptr_;
 
 private:
  BitSet(const BitSet &) {}
  void operator = (const BitSet &) {}
};


bool Ok(std::string & s1, std::string & s2) {
  int flag = false;
  for (int i = 0; i < s1.size(); i++) {
    if (s1[i] != s2[i] && flag) return false;
    if (s1[i] != s2[i] && !flag) flag = true;
  }
  return flag;
}

int Dijastra(std::vector<std::string> & v, BitSet & bs) {
  std::vector<int> d(v.size(), -1);
  
  int min = 0;
  d[0] = 0;
  std::set<int> used;

   while (used.size() < v.size() && min != -1) {
     for (int i = 0; i < v.size(); i ++) {
       if (bs.IsSet(min * v.size() + i) && (d[i] == -1 || d[i] > d[min] + 1)) {
         d[i] = d[min] + 1;
       }
     }
     if (d[v.size() - 1] != -1) break;
     // LOG(INFO) << "d:" << JoinVector(d);
     used.insert(min);
     min = -1;
     for (int i = 1; i < d.size(); i++) {
       if (used.count(i)) continue;
       if (min == -1 && d[i] != -1) min = i;
       else if (d[i] < d[min] && d[i] != -1) {
         min = i;
       }
     }
  }
  return d[v.size() - 1] == -1 ? 0 : d[v.size() - 1] + 1;
}


void Trace(std::vector<std::string> & v, int n, int k,
           BitSet & bs,
           std::vector<std::string> & path,
           std::vector<std::vector<std::string> > &rs) {
  if (n == 1) {
    if (k == 0) {
      path.push_back(v[k]);
      rs.push_back(std::vector<std::string>(path.rbegin(), path.rend()));
      path.pop_back();
    }
  } else {
    path.push_back(v[k]);
    for (int i = 0; i < v.size(); i++) {
      if (bs.IsSet(k * v.size() + i)) {
        Trace(v, n - 1, i, bs, path, rs);
      }
    }
    path.pop_back();
  }
}

std::vector<std::vector<std::string> > Dijastra(std::vector<std::string> & v, int n_i = -1) {
  BitSet bs(v.size() * v.size());
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      if (Ok(v[i], v[j])) {
        bs.Set(i * v.size() + j);
        bs.Set(j * v.size() + i);
      }
    }
  }

  int n = 0;
  if (n_i != -1) n = n_i;  //Dijastra(v, bs);
  std::vector<std::vector<std::string> > rs;
  if (n == 0) return rs;
  std::vector<std::string> path;
  Trace(v, n, v.size() - 1, bs, path, rs);
  return rs;
}



std::vector<std::vector<std::string> >
FindLadder(std::string & start, std::string & end, std::unordered_set<std::string> & dict) {
  std::vector<std::string> tdict;
  for (std::unordered_set<std::string>::iterator i = dict.begin(); i != dict.end();
       i++) {
    if (*i != start && *i != end) tdict.push_back(*i);
  }
  tdict.insert(tdict.begin(), start);
  tdict.insert(tdict.end(), end);
  return Dijastra(tdict, NB::LadderLength(start, end, dict));
}



}  // namespace algorithm

using namespace algorithm;

namespace BFS1 {
std::vector<std::vector<std::string> > BFS(std::string & start, std::string & end, std::unordered_set<std::string> & dict) {
    std::vector<std::vector<std::string> > ans;
    
    std::vector<std::string> queue;
    std::set<std::string> visited;
    std::vector<int> pre;
    queue.push_back(start);
    pre.push_back(-1);
    
    int s_idx = 0;
    while (ans.size() == 0 && s_idx < queue.size()) {
        LOG(INFO) << queue.size() << " " << s_idx;
        int e_idx = queue.size();
        for (int i = s_idx; i < e_idx; i++) {
            std::string cur = queue[i];
            
            for (int j = 0; j < cur.size(); j++) {
                for (int k = 0; k < 26; k++) {
                    if (cur[j] == 'a' + k) continue;
                    std::string t = cur;
                    t[j] = 'a' + k;
                    if (t == end) {
                        std::vector<std::string> path;
                        path.insert(path.begin(), t);
                        path.insert(path.begin(), cur);
                        int m = i;
                        while (pre[m] != -1) {
                            path.insert(path.begin(), queue[pre[m]]);
                            m = pre[m];
                        }
                        ans.push_back(path);
                    } else {
                         if (!dict.count(t)) continue;
                         if (visited.count(t)) continue;
                         queue.push_back(t);
                         visited.insert(t);
                         pre.push_back(i);
                    }
                }
            }
        }
        s_idx = e_idx;
    }
    return ans;
}
}  // namespace BFS1


int main(int argc, char** argv) {
  std::string start = "hit";
  std::string end = "cog";
  std::unordered_set<std::string> dict;
  dict.insert("hot");
  dict.insert("cog");
  dict.insert("dot");
  dict.insert("dog");
  dict.insert("hit");
  dict.insert("lot");
  // dict.insert("lot");
  dict.insert("log");
  std::vector<std::vector<std::string> > rs = BFS1::BFS(start, end, dict);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
