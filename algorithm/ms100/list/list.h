// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 16:55:47
// File  : list.h
// Brief :

#ifndef  __LIST_H_
#define  __LIST_H_

#include <vector>
#include <stack>
#include "base/public/logging.h"
#include "base/public/shared_ptr.h"

namespace algorithm {

template<typename type>
struct Node {
  Node() {
    next = NULL;
  }
  type value;
  Node <type> * next;
};

template <typename type>
Node<type> * BuildList(std::vector<type> & v) {
  Node<type> * head = NULL;
  Node<type> * before = NULL;
  for (int i = 0; i < v.size(); i++) {
    Node<type> * foo = new Node<type>();
    foo->value = v[i];
    if (i == 0) {
      head = foo;
    } else {
      before->next = foo;
    }
    before = foo;
  }
  return head;
};

template <typename type>
void Delete(Node<type> * head) {
  while (head != NULL) {
    Node<type> * foo = head;
    head = head->next;
    delete foo;
  }
}


template <typename type>
Node<type> * ReverseList(Node<type> * head) {
  Node<type> * before = NULL;
  while (head != NULL) {
    Node<type> * foo = head;
    head = head->next;
    foo->next = before;
    before = foo;
  }
};

template <typename type>
Node<type> * DeleteSome(Node<type> * head, Node<type> * ptr) {
  if (head == NULL || ptr == NULL) return head;
  if (head == ptr) {
    Node<type> * foo = head->next;
    delete head;
    return foo;
  }

  if (ptr->next != NULL) {
    ptr->value = ptr->next->value;
    Node<type> * foo = ptr->next;
    ptr->next = ptr->next->next;
    delete foo;
  } else {
    while (head != NULL && head->next != ptr) {
      head = head->next;
    }
    if (head != NULL) {
      head->value = ptr->value;
      head->next = NULL;
      delete ptr;
    }
  }

  return head;
}


template <typename type>
void ReverseOut(Node<type> * head) {
  if (head == NULL) return;
  ReverseOut(head->next);
  LOG(INFO) << head->value;
}

template <typename type>
void ReverseOut2(Node<type> * head) {
  std::stack<type> foo;
  while (head != NULL) {
    foo.push(head->value);
    head = head->next;
  }
  while(!foo.empty()) {
    LOG(INFO) << foo.top();
    foo.pop();
  }
}

template <typename type>
bool IsJoin(Node<type> * h1, Node<type> * h2) {
  while (h1->next) {
    h1 = h1->next;
  }
  while (h2->next) {
    h2 = h2->next;
  }
  return h1 == h2;
}

template <typename type>
Node<type> * JoinPoint(Node<type> * h1, Node<type> * h2) {
  int len1 = 0;
  int len2 = 0;
  Node<type> * tmp_h1 = h1;
  Node<type> * tmp_h2 = h2;
  while (h1->next) {
    len1++;
    h1 = h1->next;
  }
  while(h2->next) {
    len2++;
    h2 = h2->next;
  }
  if (len1 > len2) {
    int foo = len1 - len2;
    while(foo) {
      tmp_h1 = tmp_h1->next;
      foo--;
    }
  }
  if (len2 > len1) {
    int foo = len2 - len1;
    while(foo) {
      tmp_h2 = tmp_h2->next;
      foo--;
    }
  }
  while (tmp_h1 != NULL &&
         tmp_h2 != NULL &&
         tmp_h1 != tmp_h2) {
    tmp_h1 = tmp_h1->next;
    tmp_h2 = tmp_h2->next;
  }
  if (tmp_h1 == NULL || tmp_h2 == NULL) return NULL;
  return tmp_h1;
}

template <typename type>
bool HasCircle(Node<type> * h) {
  Node<type> * fast = h;
  Node<type> * slow = h;
  do {
    if (fast == NULL || slow == NULL || fast->next == NULL) return false;
    slow = slow->next;
    fast = fast->next->next;
  } while (slow != fast);  // Note: begin slow == fast
  return true;
}


template <typename type>
int CircleLen(Node<type> * h) {
  Node<type> * fast = h;
  Node<type> * slow = h;
  while (fast != slow) {
    slow = slow->next;
    fast = fast->next->next;
  }  // first meet, fast == slow must in circle
  int count = 0;
  do {
    count++;
    slow = slow->next;
    fast = fast->next->next;
  } while (slow != fast);  // second meet
  return count;
}

/* ----/ - \ */
// |   \ - /
// m    环长度 len, slow 到达circle point 点时， 设fast还差z步（再次）走到circle point点
// 第一次相遇slow不可能走完一圈，因为slow走z步，fast 走2z步，此时正好相遇
//
// s = m + z  
// 2s = m + x*len + (len - z) +2z 
//    = m + (x + 1) * len +z
// 2m + 2z = m + (x + 1)*len + z
// m + z = (x+1)*len
// m = x * len + (len - z) （x>=0）
// 结论1：
// 说明 若head 向前走 len - z 步骤，则 剩下的部分是环的整数倍
// 而第一次相遇的点距离circle point点正好 len - z 步
// 此时head和第一次相遇点都已步长1向前移动，相遇点就是circle point点
// x=0 也成立
//
// 结论2：
// 第一次相遇的地点向前走len-z步正好到达circle point点（可惜z无法求得）

template <typename type>
Node<type>* CirclePoint(Node<type> * h) {
  Node<type> * fast = h;
  Node<type> * slow = h;
  do{
    slow = slow->next;
    fast = fast->next->next;  // two steps
  } while(slow != fast);
  // first meet, fast == slow must in circle
  slow = h;
  while (slow != fast) {
    slow = slow->next;
    fast = fast->next;  // one step
  }
  return slow;
}
template <typename type>
Node<type> * Tail(Node<type> * h) {
  while (h->next) {
    h = h->next;
  }
  return h;
}

template <typename type>
int Size(Node<type> * h) {
  int rs = 0;
  while(h) {
    rs++;
    h = h->next;
  }
  return rs;
}

template <typename type>
Node<type> * Move(Node<type> * h, int i) {
  while(i > 0) {
    h = h->next;
    i--;
    if (h == NULL) return NULL;
  }
  return h;
}

}  // namespace algorithm

#endif  //__LIST_H_
