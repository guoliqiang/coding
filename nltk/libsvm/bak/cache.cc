// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 19:03:26
// File  : cache.cc
// Brief :

#include "../public/cache.h"
#include "base/public/logging.h"

namespace nltk {

Cache::Cache(int l, int64_t size):l_(l), size_(size) {
  // initialized to 0
  head_ = reinterpret_cast<head_t *>(calloc(l_, sizeof(head_t)));
  size_ /= sizeof(Qfloat);
  size_ -= (l_ * sizeof(head_t)) / sizeof(Qfloat);

  // cache must be large enough for two columns
  size_ = max(size_, static_cast<int64_t>(2 * l_));
  lru_head_.next_ = lru_head_.prev_ = &lru_head_;
}

Cache::~Cache() {
  for (head_t *h = lru_head_.next_;
       h != &lru_head_; h = h->next_) {
    free(h->data_);
  }
  free(head_);
}

void Cache::LruDelete(head_t *h) {
  // delete from current location
  h->prev_->next_ = h->next_;
  h->next_->prev_ = h->prev_;
}

void Cache::LruInsert(head_t *h) {
  // insert to last position
  h->next_ = &lru_head_;
  h->prev_ = lru_head_.prev_;
  h->prev_->next_ = h;
  h->next_->prev_ = h;
}

int Cache::GetData(const int index,
                    Qfloat **data,
                    int len) {
  head_t *h = &head_[index];
  if (h->len_) LruDelete(h);
  int more = len - h->len_;

  if (more > 0) {
    // free old space
    while (size_ < more) {
      head_t *old = lru_head_.next_;
      CHECK_NE(old, &lru_head_);

      LruDelete(old);
      free(old->data_);
      size_ += old->len_;
      old->data_ = NULL;
      old->len_ = 0;
    }

    // allocate new space
    h->data_ = reinterpret_cast<Qfloat *>
               (realloc(h->data_, sizeof(Qfloat) * len));
    size_ -= more;
    swap(h->len_, len);
  }

  LruInsert(h);
  *data = h->data_;
  return len;
}


void Cache::SwapIndex(int i, int j) {
  if (i == j) return;

  if (head_[i].len_) LruDelete(&head_[i]);
  if (head_[j].len_) LruDelete(&head_[j]);
  swap(head_[i].data_, head_[j].data_);
  swap(head_[i].len_, head_[j].len_);
  if (head_[i].len_) LruInsert(&head_[i]);
  if (head_[j].len_) LruInsert(&head_[j]);

  if (i > j) swap(i, j);
  for (head_t *h = lru_head_.next_;
       h != &lru_head_; h = h->next_) {
    if (h->len_ > i) {
      if (h->len_ > j) {
        swap(h->data_[i], h->data_[j]);
      } else {
        // give up
        LruDelete(h);
        free(h->data_);
        size_ += h->len_;
        h->data_ = NULL;
        h->len_ = 0;
      }
    }  // if
  }  // for
}

}  // namespace nltk
