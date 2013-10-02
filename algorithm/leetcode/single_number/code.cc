// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 23:06:37
// File  : code.cc
// Brief :

/*
14 / 14 test cases passed.
Status: Accepted
Runtime: 56 ms
*/

class Solution {
public:
    int singleNumber(int A[], int n) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        for (int i = 1; i < n; i++) {
            A[0] ^= A[i];
        }
        return A[0];
    }
};
