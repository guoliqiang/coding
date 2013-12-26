// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-20 22:27:41
// File  : code.cc
// Brief :

/*
Sort a linked list in O(n log n) time using constant space complexity.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x = 0) : val(x), next(NULL) {}
};

// TLE
void Sort(ListNode * head, ListNode * tail) {
  if (head == tail || head == NULL || tail == NULL) return;
  ListNode * pre = NULL;
  ListNode * foo = head;
  while (foo != tail) {
    if (foo->val < tail->val) {
      if (pre == NULL) pre = head;
      else pre = pre->next;
      if (pre != foo) std::swap(pre->val, foo->val);
    }
    foo = foo->next;
  }
  ListNode * mid = NULL;
  if (pre != NULL) {
    mid = pre;
    pre = pre->next;
  } else {
    pre = head;
  }
  if (pre != tail) std::swap(pre->val, tail->val);
  if (mid != NULL) Sort(head, mid);
  if (pre != tail) Sort(pre->next, tail);
}

ListNode * Sort(ListNode * head) {
  if (head == NULL) return head;
  ListNode * tail = head;
  while (tail->next != NULL) tail = tail->next;
  Sort(head, tail);
  return head;
}

/*
 * 15 / 15 test cases passed.
 * Status: Accepted
 * Runtime: 264 ms
 * Submitted: 0 minutes ago
 *
 * */

ListNode * Merge(ListNode * head1, ListNode * head2) {
  ListNode * rs = NULL;
  ListNode ** pre = &rs;
  while (head1 != NULL && head2 != NULL) {
    if (head1->val < head2->val) {
      *pre = head1;
      head1 = head1->next;
    } else {
      *pre = head2;
      head2 = head2->next;
    }
    pre = &((*pre)->next);
  }
  if (head1 != NULL) *pre = head1;
  if (head2 != NULL) *pre = head2;
  return rs;
}

/*
 * 使用到了递归，不符合O(1)空间复杂度的要求
 * */
ListNode * MergeSort(ListNode * head1, int n) {
  if (n <= 1) return head1;
  
  int t = n / 2 - 1;
  ListNode * foo = head1;
  while (t--) foo = foo->next;
  ListNode * head2 = foo->next;
  foo->next = NULL;

  head1 = MergeSort(head1, n / 2);
  head2 = MergeSort(head2, n - n / 2);
  return Merge(head1, head2);
}


ListNode * MergeSort(ListNode * head) {
  ListNode * foo = head;
  int n = 0;
  while (foo != NULL) {
    n++;
    foo = foo->next;
  }
  return MergeSort(head, n);
}

void Out(ListNode * ptr) {
  while (ptr != NULL) {
    LOG(INFO) << ptr->val;
    ptr = ptr->next;
  }
}

ListNode * Build(std::vector<int> & v) {
  ListNode * rs = NULL;
  ListNode ** pre = &rs;
  for (int i = 0; i < v.size(); i++)  {
    *pre = new ListNode(v[i]);
    pre = &((*pre)->next);
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace NB {

// Accepted
// return the tail of the sorted list of [head1, head1 + n]
ListNode * Merge(ListNode ** pre, ListNode * head1, int n) {
  int t = n / 2;
  ListNode * head2 = head1;
  ListNode * temp = head2;
  while (t-- && head2 != NULL) {
    temp = head2;
    head2 = head2->next;
  }
  if (head2 == NULL) return temp;

  int k1 = 0;
  int k2 = 0;
  while (head2 != NULL && k1 < n / 2 && k2 < n / 2) {
    if (head1->val < head2->val) {
      *pre = head1;
      head1 = head1->next;
      k1++;
    } else {
      *pre = head2;
      head2 = head2->next;
      k2++;
    }
    pre = &((*pre)->next);
  }
  if (k2 == n / 2 || head2 == NULL) {
    *pre = head1;
    while (k1 < n / 2 - 1) {
      head1 = head1->next;
      k1++;
    }
    head1->next = head2;
    return head1;
  } else {
    *pre = head2;
    while (k2 < n / 2 - 1 && head2->next != NULL) {
      head2 = head2->next;
      k2++;
    }
    return head2;
  }
}

ListNode * MergeSort(ListNode * head) {
  int len = 0;
  ListNode * foo = head;
  while (foo != NULL) {
    len++;
    foo = foo->next;
  }
  int k = 2;
  while (k / 2 < len) {
    ListNode * * pre = &head;
    ListNode * cur = head;
    do {
      ListNode * t = Merge(pre, cur, k);
      pre = &(t->next);
      cur = t->next;
    } while (cur != NULL);
    k *= 2;
  }
  return head;
}
}  // namespace NB

namespace NNB {

ListNode * Merge(ListNode* l1, ListNode* l2, ListNode ** pre){
  ListNode * rs = *pre;
  while(l1 || l2) {
    if(!l1) {
      *pre = l2;
      l2 = l2->next;
    } else if(!l2){
      *pre = l1;
       l1 = l1->next;
    } else if (l1->val < l2->val){
      *pre = l1;
      l1=l1->next;
    } else {
      *pre = l2;
      l2=l2->next;
    }
    rs = *pre;
    pre = &((*pre)->next);
  }
  return rs;
}

ListNode* Split(ListNode* h, int len){
  for (int i = 1; i < len && h != NULL; i++){
    h = h->next;
  }
  if(!h) return NULL;
  ListNode* t = h->next;
  h->next=NULL;
  return t;
}

ListNode *SortList(ListNode *head) {
  int listlen=0;
  ListNode * foo = head;
  while(foo) {
    listlen++;
    foo = foo->next;
  }
  for(int len = 1; len < listlen; len *= 2){
    ListNode * t= head;
    ListNode * * pre = &head;
    ListNode * l1 = NULL;
    ListNode * l2 = NULL;
    while(t != NULL){
      l1 = t;
      l2 = Split(t, len);
      t = Split(l2, len);
      ListNode* tail = Merge(l1, l2, pre);
      pre = &(tail->next);
   }
 }
 return head;
}
}  // namespace NNB

namespace third {
ListNode * Merge(ListNode * l1, ListNode * l2, ListNode * * pre) {
  ListNode * last = NULL;
  while (l1 != NULL || l2 != NULL) {
    if (l1 == NULL) {
      *pre = l2;
      l2 = l2->next;
    } else if (l2 == NULL) {
      *pre = l1;
      l1 = l1->next;
    } else if (l1->val < l2->val) {
      *pre = l1;
      l1 = l1->next;
    } else {
      *pre = l2;
      l2 = l2->next;
    }
    last = *pre;
    pre = &((*pre)->next);
  }
  return last;
}

ListNode * Split(ListNode * root, int len) {
  ListNode * tmp = root;
  for (int i = 1; i < len && tmp != NULL; i++) {
    tmp = tmp->next;
  }
  if (tmp == NULL) return NULL;
  ListNode * rs = tmp->next;
  tmp->next = NULL;
  return rs;
}

ListNode * MergeSort(ListNode * head) {
  ListNode * tmp = head;
  int count = 0;
  while (tmp != NULL) {
    count++;
    tmp = tmp->next;
  }
  for (int len = 1; len < count; len *= 2) {
    ListNode * t = head;
    ListNode ** pre = &head;
    while (t != NULL) {
      ListNode * l1 = t;
      ListNode * l2 = Split(l1, len);
      t = Split(l2, len);
      ListNode * tail = Merge(l1, l2, pre);
      pre = &(tail->next);
    }
  }
  return head;
}
}  // namespace third

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(2);
  v.push_back(1);
  ListNode * head = Build(v);
  Out(head);
  head = third::MergeSort(head);
  HERE(INFO);
  Out(head);
  return 0;
}
