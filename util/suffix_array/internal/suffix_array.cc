// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 00:33:10
// File  : suffix_array.cc
// Brief :

#include "../public/suffix_array.h"


namespace util {

int SuffixArray::DiffSubStringNum() {
  int rs = 0;
  rs += str_.size() - sa_[1];
  for (int i = 2; i < str_.size(); i++) {
    rs += str_.size() - sa_[i] - height_[i];
  }
  return rs;
}

int SuffixArray::NotOverlapLen() {
  int min = 1;
  int max = str_.size() - 1;
  while (min <= max) {
    int mid = min + (max - min) / 2;
    if (NotOverlapCheck(mid)) {
      min = mid + 1;
    } else {
      max = mid - 1;
    }
  }
  return max;
}

bool SuffixArray::NotOverlapCheck(int n) {
  int max = sa_[1];
  int min = sa_[1];
  for (int i = 2; i < str_.size(); i++) {
    if (height_[i] < n) {
      max = sa_[i];
      min = sa_[i];
    } else {
      if (sa_[i] > max) {
        max = sa_[i];
      }
      if (sa_[i] < min) {
        min = sa_[i];
      }
      if (max - min >= n) return true;
    }
  }
  return false;
}

void SuffixArray::BucketSort(int j) {
  int size = str_.size();
  int * tsa = new int[size];
  memset(tsa, 0, size * sizeof(tsa[0]));
  memset(sum_, 0, LEN * sizeof(sum_[0]));

  // when i + j > size the rank_[i + j]] value is 0
  for (int i = 1; i < size; i++) {
    CHECK(rank_[i + j] >= 0 && rank_[i + j] < LEN);
    // the check may find LEN is not enough
    sum_[rank_[i + j]]++;
  }

  // i must begin from 1
  // sum_[0] is usefull, because of rank_[i + j] may be zero.
  for (int i = 1; i < LEN; i++) {
    sum_[i] += sum_[i - 1];
  }

  // sort using the second key
  for (int i = size - 1; i >= 1; i--) {
    tsa[sum_[rank_[i + j]]--] = i;
  }
  // tsa is the result after sorting using second key

  // for (int i = 1; i < size; i++) {
  //   std::cout << "tsa[" << i << "] : " << tsa[i] << " ";
  // }
  // std::cout << std::endl;

  memset(sum_, 0, LEN * sizeof(sum_[0]));
  for (int i = 1; i < size; i++) {
    sum_[rank_[i]]++;
  }
  for (int i = 1; i < LEN; i++) {
    sum_[i] += sum_[i - 1];
  }
  // sort using the first key
  for (int i = size - 1; i > 0; i--) {
    sa_[sum_[rank_[tsa[i]]]--] = tsa[i];
  }
  delete [] tsa;
}

void SuffixArray::Build() {
  int * trank = new int[str_.size()];
  // the character num may be lagger than 0xff
  int size = str_.size();

  for (int i = 1; i< size; i++) {
    trank[i]= str_[i];
  }
  for (int i = 1; i < size; i++) {
    sum_[static_cast<int>(trank[i])]++;
  }
  for (int i = 2; i < LEN; i++) {
    sum_[i] += sum_[i - 1];
  }

  for (int i = size - 1; i >= 1; i--) {
    // LOG(INFO) << trank[i] << static_cast<int>(trank[i]);
    // LOG(INFO) << sum_[static_cast<int>(trank[i])];
    sa_[sum_[static_cast<int>(trank[i])]--] = i;
  }
  // LOG(INFO) << "first sa:";
  // OutSa();

  // build first rank and sa
  // at begin rank may have same values.
  rank_[sa_[1]] = 1;
  int p = 1;
  for (int i = 2; i < size; i++) {
    if (trank[sa_[i]] != trank[sa_[i - 1]]) p++;
    rank_[sa_[i]] = p;
  }
  // LOG(INFO) << "first rank:";
  // OutRank();

  for (int j = 1; j < size; j *= 2) {
    BucketSort(j);
    trank[sa_[1]] = 1;
    p = 1;
    for (int i = 2; i < size; i++) {
      if ((rank_[sa_[i]] != rank_[sa_[i-1]]) ||
          (rank_[sa_[i] + j ] != rank_[sa_[i - 1] + j])) {
        p++;
      }
      trank[sa_[i]] = p;
    }
    for (int i = 1; i < size; i++) {
      rank_[i] = trank[i];
      CHECK(rank_[i] >= 0);
    }
  }

  Height();
  RMQ(1, 1, str_.size() - 1);
  int foo;   // not used in this function
  RMQMax(1, 1, str_.size() - 1, &foo);
}

// height[i] >= height[i - 1] -1 (i is the subscript of string:, 1,2...)
// "abcdf"
//  height[cdf] >= height[bcdf] - 1
//  Proof: if height[bcdf] == 0 abosolutely.
//
//  Note: In final the subscript of height is the rank value.
//
void SuffixArray::Height() {
  int j = 0;
  for (int i = 1; i < str_.size(); i++) {
    if (rank_[i] == 1) continue;
    while (str_[i + j] ==
           str_[sa_[rank_[i] - 1] + j] &&
           sa_[rank_[i] - 1] + j < str_.size()) {
      // if (rank_[i] == 17) {
      //   LOG(INFO) << "i:" << i << " j:" << j;
      //   LOG(INFO) << i + j << " " << str_[i + j] << " "
      //             << sa_[rank_[i] - 1] + j
      //             << " "<<str_[sa_[rank_[i] - 1] + j];
      // }
      j++;
    }
    height_[rank_[i]] = j;
    if (j > 0) j--;
  }
}

// the subscript of height is the rank value
void SuffixArray::RMQ(int index, int b, int e) {
  if (b == e) {
    rmq_[index] = height_[b];
  } else {
    int mid = b + (e - b) / 2;
    RMQ(2 * index, b, mid);  // [b, mid]
    RMQ(2 * index + 1, mid + 1, e);  // [mid + 1, e]
    if (rmq_[2 * index] < rmq_[2 * index + 1]) {
      rmq_[index] = rmq_[2 * index];
    } else {
      rmq_[index] = rmq_[2 * index + 1];
    }
  }
  // LOG(INFO) << b << " " << e << " " << index << " " << rmq_[index];
}

int SuffixArray::GetRMQ(int b, int e) {
  return GetRMQInternal(b, e, 1, str_.size() - 1, 1);
}

int SuffixArray::GetRMQInternal(int b, int e, int i, int j, int index) {
  if (b == i && e == j) return rmq_[index];
  int mid = i + (j - i) / 2;
  if (e <= mid) {
    return GetRMQInternal(b, e, i, mid, 2 * index);
  }
  if (b > mid) {
    return GetRMQInternal(b, e, mid + 1, j, 2 * index + 1);
  }
  int foo = GetRMQInternal(b, mid, i, mid, 2 * index);
  int bar = GetRMQInternal(b, mid + 1, mid + 1, j, 2 * index + 1);
  return foo < bar ? foo : bar;
}

// the subscript of height is the rank value
void SuffixArray::RMQMax(int index, int b,
                         int e, int *rs /*=NULL*/) {
  if (b == e) {
    rmq_max_[index] = height_[b];
    rmq_max_index_[index] = b;
    if (!rs) *rs = b;
  } else {
    int mid = b + (e - b) / 2;
    int tmp_index1 = 0;
    int tmp_index2 = 0;
    RMQMax(2 * index, b, mid, &tmp_index1);  // [b, mid]
    RMQMax(2 * index + 1, mid + 1, e, &tmp_index2);  // [mid + 1, e]
    if (rmq_max_[2 * index] > rmq_max_[2 * index + 1]) {
      rmq_max_[index] = rmq_max_[2 * index];
      rmq_max_index_[index] = tmp_index1;
    } else {
      rmq_max_[index] = rmq_max_[2 * index + 1];
      rmq_max_index_[index] = tmp_index2;
    }
  }
}

int SuffixArray::GetRMQMax(int b, int e, int * rs /*= NULL*/) {
  return GetRMQInternalMax(b, e, 1, str_.size() - 1, 1, rs);
}

int SuffixArray::GetRMQInternalMax(int b, int e, int i,
                                   int j, int index, int * rs) {
  if (b == i && e == j) {
    if (!rs) *rs = rmq_max_index_[index];
    return rmq_max_[index];
  }
  int mid = i + (j - i) / 2;
  if (e <= mid) {
    return GetRMQInternalMax(b, e, i, mid, 2 * index, rs);
  }
  if (b > mid) {
    return GetRMQInternalMax(b, e, mid + 1, j, 2 * index + 1, rs);
  }
  int tmp1 = 0;
  int tmp2 = 0;
  int foo = GetRMQInternalMax(b, mid, i, mid, 2 * index, &tmp1);
  int bar = GetRMQInternalMax(b, mid + 1, mid + 1, j, 2 * index + 1, &tmp2);
  if (foo > bar) {
    if (!rs) *rs = tmp1;
    return foo;
  }
  if (!rs) *rs = tmp2;
  return bar;
}

void SuffixArray::OutRank() {
  for (int i = 1; i < str_.size(); i++) {
    std::cout << "rank[" << i << "] : " << rank_[i] << " : "
              << str_.substr(i) << std::endl;
  }
  std::cout << std::endl;
}

void SuffixArray::OutSa() {
  for (int i = 1; i < str_.size(); i++) {
    std::cout << "sa[" << i << "] : " << sa_[i] << " "
              << str_.substr(sa_[i]) << std::endl;
  }
  std::cout << std::endl;
}

void SuffixArray::OutHeight() {
  for (int i = 2; i < str_.size(); i++) {
    std::cout << "height[" <<i << "] : " << height_[i] << " for "
              << str_.substr(sa_[i - 1]) << "--"
              << str_.substr(sa_[i]) << std::endl;
  }
  std::cout << std::endl;
}

void SuffixArray::OutRMQ() {
  int foo = pow(2, log(str_.size()) / log(2) + 1);
  for (int i = 0; i < foo; i++) {
    std::cout << "rmq[" << i << "] : " << rmq_[i] << " ";
  }
  std::cout << std::endl;
}
}  // namespace util
