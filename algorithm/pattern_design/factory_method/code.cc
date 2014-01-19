// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 10:49:45
// File  : code.cc
// Brief :

/*
工厂方法模式：
一个抽象产品类，可以派生出多个具体产品类。   
一个抽象工厂类，可以派生出多个具体工厂类。   
每个具体工厂类只能创建一个具体产品类的实例。

抽象工厂模式：
多个抽象产品类，每个抽象产品类可以派生出多个具体产品类。   
一个抽象工厂类，可以派生出多个具体工厂类。   
每个具体工厂类可以创建多个具体产品类的实例。   
    
区别：
工厂方法模式只有一个抽象产品类，而抽象工厂模式有多个。   
工厂方法模式的具体工厂类只能创建一个具体产品类的实例，而抽象工厂模式可以创建多个。
http://zhidao.baidu.com/link?url=CNKfE2ePsmsePJvIdB0QTuyuKjOQZgOdkaoGMttk2EvyZgPZwDBgXKVfrok5UQDW7DB3prTcGWwBEwL06YBVTK
*/

// 子类中返回具体生成的对象
#include "base/public/common_ojhead.h"

namespace algorithm {
// 只有一个产品类
class Product;

class ProductCreator {
 public:
  virtual ~ProductCreator() {}
  virtual Product * CreatFactory() = 0;
};

// 也可以用继承实现多个子类
template <typename T>
class GeneralProductCreator : public ProductCreator {
 public:
  Product * CreatFactory() {
    return new T();
  }
};

class Product {
 public:
  virtual ~Product() {}
  virtual void Do() {}
};

class Product1 : public Product {
};

class Product2 : public Product {
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  ProductCreator * creator = new GeneralProductCreator<Product1>();
  Product * p = creator->CreatFactory();
  p->Do();
  return 0;
}
