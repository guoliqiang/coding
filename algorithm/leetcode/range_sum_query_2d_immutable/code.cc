// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-04 23:07:22
// File  : code.cc
// Brief :

class NumMatrix {
public:
    NumMatrix(vector<vector<int>> &matrix) {
        int m = matrix.size();
        if (m == 0) return;
        int n = matrix[0].size();

        std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 && j == 0) dp[i][j] = matrix[i][j];
                else if (i == 0) dp[i][j] = dp[i][j - 1] + matrix[i][j];
                else if (j == 0) dp[i][j] = dp[i - 1][j] + matrix[i][j];
                else {
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1] - dp[i - 1][j - 1] + matrix[i][j];
                }
            }
        }
        sum.swap(dp);
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        int ans = sum[row2][col2];
        if (row1 - 1 >= 0) ans -= sum[row1 - 1][col2];
        if (col1 - 1 >= 0) ans -= sum[row2][col1 - 1];
        if (row1 - 1 >= 0 && col1 - 1 >= 0) ans += sum[row1 - 1][col1 - 1];
        return ans;
    }

    std::vector<std::vector<int> > sum;
};
