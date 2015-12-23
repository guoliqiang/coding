// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-16 20:38:49
// File  : noncopyable.h
// Brief :

#ifndef  __NONCOPYABLE_H_
#define  __NONCOPYABLE_H_
namespace base {

class Noncopyable {
  protected:
   Noncopyable() {}
   ~Noncopyable() {}

  private:
   // not need to implement, because non will be allowed to call it.
   void operator = (const Noncopyable &);
   Noncopyable(const Noncopyable &);
};
}  // namespace base

typename base::Noncopyable Noncopyable;

#endif  //__NONCOPYABLE_H_
