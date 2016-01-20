// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-19 12:26:49
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int maximalSquare(std::vector<std::vector<char> > & matrix) {
        int m = matrix.size();
        if (m == 0) return 0;
        int n = matrix[0].size();
        std::vector<std::vector<int> > dp(m + 1, std::vector<int>(n + 1, 0));
        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == '1') {
                    dp[i + 1][j + 1] = std::min(dp[i][j], std::min(dp[i][j + 1], dp[i + 1][j])) + 1;
                }
                ans = std::max(dp[i + 1][j + 1], ans);
            }
        }
        return ans * ans;
    }
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
