// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-02 22:14:52
// File  : t.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::string LongestBString(std::string str) {
  std::string rs(100, -1);
  for (int i = 0; i < str.size(); i++) {
    int p1 = -1;
    int p2 = -1;
    int r = 1;
    int j = 0;
    for (j = 0; rs[j] != -1; j++) {
      if (p1 == -1 && rs[j] == str[i]) {
        p1 = j;
        for (int k = j + 1; rs[k] != -1; k++) {
          if (rs[k] > str[i]) {
            p2 = k;
            break;
          }
        }
        if (p2 != -1) {
          for (int k = p1 + 1; k < p2; k++) {
            r = 0;
            for (int l = i + 1; l < str.size(); l++) {
              if (str[l] == rs[k]) {
                r = 1;
                break;
              }
            }
            if (r == 0) break;
          }
        }
      }
      if (p2 != -1 && r == 1) rs[j] = rs[j + 1];
    }
    if (p2 != -1 && r == 1) rs[j - 1] = str[i];
    else if (p1 == -1) rs[j] = str[i];
  }
  int i = 0;
  while (rs[i] != -1) i++;
  return rs.substr(0, i);
}
}  // namespace algorithm

namespace NB {
void CarrCup() {
  char s[1000000]; 
  int p[257],i,j,r,p1,p2,k,t; 
  scanf("%s",s); 
  for(i=0;i<257;i++) p[i]=-1; 
  for(i=0;s[i]!='\0';i++) { 
    r=1; 
    p1=-1; 
    p2=-1;
    LOG(INFO) << "s[" << i << "]:" << (char)s[i];
    for(j=0;p[j]!=-1;j++)  { 
      if(p1==-1 && p[j]==s[i]) { 
        p1=j;
        for(k=j+1;p[k]!=-1;k++) { 
          if(p[k]>s[i]) { 
            p2=k; 
            break; 
          } 
        } 
        if(p2>-1) { 
          for(k=p1+1;k<p2;k++) { 
            r=0; 
            for(t=i+1;s[t]!='\0';t++) { 
              if(s[t]==p[k]) { 
                r=1; 
                break; 
              } 
            } 
            if(r==0) break; 
          } 
        } 
      }
      if(p2>-1 && r==1) {
        p[j]=p[j+1];
      }
    } 
    if(p2>-1 && r==1) p[j-1]=s[i]; 
    else if(p1==-1) p[j]=s[i]; 
  } 
  for(i=0;p[i]!=-1;i++) printf("%c",p[i]); 
  printf("\n");
 
}
}  // namespace NB
using namespace algorithm;

int main() { 
 LOG(INFO) << LongestBString("adbczbc");
 LOG(INFO) << LongestBString("babab");
 LOG(INFO) << LongestBString("nlhthgrfdnnlprjtecpdrthigjoqdejsfkasoctjijaoebqlrgaiakfsbljmpibkidjsrtkgrdnqsknbarpabgokbsrfhmeklrle");
 return 0; 
}
