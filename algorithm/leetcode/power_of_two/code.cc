// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 19:23:08
// File  : code.cc
// Brief :

class Solution {
public:
    bool isPowerOfTwo(int n) {
        return n <= 0 ? false : ((n & (n - 1)) == 0);
    }
};
