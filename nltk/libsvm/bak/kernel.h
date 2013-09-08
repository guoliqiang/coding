// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 19:56:28
// File  : kernel.h
// Brief :

#ifndef  __KERNEL_H_
#define  __KERNEL_H_

#include "common.h"
#include "svm_node.h"
#include "svm_parameter.h"
#include "cache.h"
#include "svm_problem.h"

namespace nltk {

// Kernel evaluation
//
// the static method k_function is for doing single kernel evaluation
// the constructor of Kernel prepares to calculate the l*l kernel matrix
// the member function get_Q is for getting one column from the Q Matrix
//

class QMatrix {
 public:
  virtual Qfloat *GetQ(int column, int len) const = 0; 
  virtual double *GetQD() const = 0; 
  virtual void SwapIndex(int i, int j) const = 0; 
  virtual ~QMatrix() {}
};

class Kernel: public QMatrix {
 public:
  Kernel(int l, SvmNode ** x, 
         const SvmParameter & param);
  virtual ~Kernel();

  static double Kfunction(const SvmNode *x, const SvmNode *y,
                          const SvmParameter& param);
  
  virtual Qfloat * GetQ(int column, int len) const = 0; 
  virtual double * GetQD() const = 0; 
  virtual void SwapIndex(int i, int j) const {
    swap(x_[i], x_[j]);
    if(x_square_) swap(x_square_[i], x_square_[j]);
  }

 protected:
  double (Kernel::*kernel_function_)(int i, int j) const;

 private:
  SvmNode ** x_; 
  double * x_square_;
  
  // SvmParameter
  const int kernel_type_;
  const int degree_;
  const double gamma_;
  const double coef0_;

 private:
  static double Dot(const SvmNode *px,
                    const SvmNode *py);
  
  double Linear(int i,int j) const;
  double Poly(int i, int j) const;
  double Rbf(int i, int j) const;
  double Sigmoid(int i, int j) const;
  double Precomputed(int i, int j) const;
};


//
// Q matrices for various formulations
//
class SVC_Q: public Kernel { 
 public:
  SVC_Q(const SvmProblem & prob,
        const SvmParameter & param,
        const schar *y);
  
  Qfloat *GetQ(int i, int len) const;
  double *GetQD() const;
  void SwapIndex(int i, int j) const;
  ~SVC_Q();

 private:
  schar * y_;
  Cache * cache_;
  double * QD_;
};

};
#endif  //__KERNEL_H_
