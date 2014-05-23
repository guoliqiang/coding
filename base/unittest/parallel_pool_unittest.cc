// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-05-22 20:40:31
// File  : parallel_pool_unittest.cc
// Brief :

#include "base/public/parallel_pool.h"
#include "base/public/mutex.h"

void Foo(int flag,  base::BlockingCounter * bc) {
  LOG(INFO) << flag;
  bc->DecrementCount();
}

int main(int argc, char** argv) {
  base::AtExitManager at_exit;
  base::ParallelPool * pool = base::ParallelPool::GetInstance();
  scoped_ptr<base::BlockingCounter> bc(new base::BlockingCounter(3));
  pool->Add(NewOneTimeCallback(&Foo, 1, bc.get()));
  pool->Add(NewOneTimeCallback(&Foo, 2, bc.get()));
  pool->Add(NewOneTimeCallback(&Foo, 3, bc.get()));
  bc->Wait();
  LOG(INFO) << "all done!";
  return 0;
}
