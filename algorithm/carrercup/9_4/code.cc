// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 22:02:09
// File  : code.cc
// Brief :
/*
 * 我们将文件分为K份，其中X*K=2GB。每次取其中一份载入到内存中， 用O(nlog n)的算法排序，然后再保存到外部文件。
 * 载入下一份并排序
 * 当我们将K份小文件都排好序，合并它们。
 * 上面的算法就是外排序，步骤3又称为N路归并。
 * 赢着树 败着树, 堆
 * leetcode/merge_k_sorted_lists/
 * */
