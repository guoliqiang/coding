// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 22:34:59
// File  : code.cc
// Brief :
/*
Write a function that accepts two or more strings and returns the longest common substring in all of them.
Longest common sub-string of strings S1, S2,.... Sn can be found as follows: 
1. Build a generalized suffix tree for S1, S2,... Sn 
2. Mark each internal node v by 1 (resp. 2, ... n) if the subtree below v contains a leaf for a suffix of S1 (resp. S2,... Sn) 
3. Traverse the tree to find nodes marked by all 
http://www.careercup.com/question?id=16381662
*/
