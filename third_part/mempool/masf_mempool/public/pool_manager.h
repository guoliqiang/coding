// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#ifndef UTIL_YMEMPOOL_MASF_MEMPOOL_POOL_MANAGER_H_
#define UTIL_YMEMPOOL_MASF_MEMPOOL_POOL_MANAGER_H_

#include <vector>
#include "third_part/mempool/masf_mempool/public/masf_mempool.h"
#include "third_part/gtl/public/stl_util-inl.h"

namespace util {
namespace mempool {

class ObjPoolManager {
 public:
  static ObjPoolManager &GetInst() {
    static ObjPoolManager manager_;
    return manager_;
  }
  static void AddObjPool(SolidMempool *pool) {
    GetInst().obj_pools_.push_back(pool);
  }
  static void Recycle() {
    std::vector<SolidMempool*> &obj_pools = GetInst().obj_pools_;
    for (int i = 0; i < obj_pools.size(); ++i) {
      obj_pools[i]->Recycle();
    }
  }
  static void Destroy() {
    std::vector<SolidMempool*> &obj_pools = GetInst().obj_pools_;
    gtl::STLDeleteElements(&obj_pools);
  }
 private:
  ~ObjPoolManager() {
    gtl::STLDeleteElements(&obj_pools_);
  }
  std::vector<SolidMempool*> obj_pools_;
};

template <class Type>
class MasfObjPoolHolder {
 public:
  static MasfObjPool<Type> &GetInst() {
    return *pool_;
  }

  static void CreatePool(int item_per_block = kDftItemPerBlock,
                         float balance_factor = kDftBalanceFactor,
                         int balance_period = kDftBalancePeriod) {
    if (pool_ != NULL) return;
    pool_ = new MasfObjPool<Type>(item_per_block,
                                  balance_factor, balance_period);
    ObjPoolManager::AddObjPool(pool_);
  }

  static void Recycle() {
    GetInst().Recycle();
  }
  
 private:
  static MasfObjPool<Type> *pool_;
};

}  // namespace mempool
}  // namespace util

#endif  // UTIL_YMEMPOOL_MASF_MEMPOOL_POOL_MANAGER_H_
