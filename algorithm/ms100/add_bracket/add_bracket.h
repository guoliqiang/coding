// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 19:04:51
// File  : add_bracket.h
// Brief :
// Note this is
// Catalan数
//

#ifndef  __ADD_BRACKET_H_
#define  __ADD_BRACKET_H_

#include "base/public/logging.h"

namespace algorithm {

struct DpNode {
  int  num;
  std::vector<std::string> values;
};

void DP(std::string & str, DpNode ** dp) {
  for (int i = 0; i < str.size(); i++) {
    dp[i][i].num = 1;
    dp[i][i].values.push_back(std::string("(") + str[i] + ")");
  }
  for (int i = 1; i < str.size(); i++) {
    for (int j = 0; j < str.size() - i; j++) {
      std::vector<std::string> foo;
      int bar = 0;
      for (int k = j; k < j + i; k++) {
        // LOG(INFO) << i << " " << j << " " << k;
        // LOG(INFO) << k + 1 << " " << j + i;
        bar += (dp[j][k].num * dp[k + 1][j + i].num);
        for (int m1 = 0; m1 < dp[j][k].values.size(); m1++) {
          for (int m2 = 0; m2 < dp[k + 1][j + i].values.size(); m2++) {
            foo.push_back(std::string("(") + dp[j][k].values[m1]
                          + dp[k + 1][j + i].values[m2] + ")");
          }
        }
      }
      dp[j][j + i].num = bar;
      dp[j][j + i].values = foo;
    }
  }
}

void OutBracket(std::string & str) {
  DpNode ** dp = new DpNode * [str.size()];
  for (int i = 0; i < str.size(); i++) {
    dp[i] = new DpNode[str.size()];
  }
  DP(str, dp);
  LOG(INFO) << dp[0][str.size() - 1].num;
  for (int i = 0; i < dp[0][str.size() - 1].values.size(); i++) {
    LOG(INFO) << dp[0][str.size() - 1].values[i];
  }
  for (int i = 0; i < str.size(); i++) {
    delete [] dp[i];
  }
  delete [] dp;
}

}  // namespace algorithem

#endif  //__ADD_BRACKET_H_
