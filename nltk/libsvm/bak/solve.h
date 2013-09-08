// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-05 10:17:43
// File  : solve.h
// Brief :

#ifndef  SOLVE_H_
#define  SOLVE_H_

#include "common.h"
#include "kernel.h"

namespace nltk {

// An SMO algorithm in Fan et al., JMLR 6(2005), p. 1889--1918
// Solves:
//
//  min 0.5(\alpha^T Q \alpha) + p^T \alpha
//
//    y^T \alpha = \delta
//    y_i = +1 or -1
//    0 <= alpha_i <= Cp for y_i = 1
//    0 <= alpha_i <= Cn for y_i = -1
//
// Given:
//
//  Q, p, y, Cp, Cn, and an initial feasible point \alpha
//  l is the size of vectors and matrices
//  eps is the stopping tolerance
//
// solution will be put in \alpha, objective value will be put in obj
//

class Solver {
 public:
  Solver() {};
  virtual ~Solver() {};
  struct SolutionInfo {
    double obj_;
    double rho_;
    double upper_bound_p_;
    double upper_bound_n_;
    double r_; // for Solver_NU
  };
  void Solve(int l, const QMatrix& Q,
             const double *p, const schar *y,
             double *alpha, double Cp, double Cn,
             double eps, SolutionInfo* si,
             int shrinking);
 
 protected:
  enum {
    LOWER_BOUND = 0,
    UPPER_BOUND = 1,
    FREE = 2
  };

 protected:
  int active_size_;
  schar * y_;
  double * G_;  // gradient of objective function
  char * alpha_status_; // LOWER_BOUND, UPPER_BOUND, FREE
  double * alpha_;
  const QMatrix * Q_;
  const double * QD_;
  double eps_;
  double Cp_;
  double Cn_;
  double * p_;
  int * active_set_;
  double * G_bar_;    // gradient, if we treat free variables as 0
  int l_;
  bool unshrink_;    // XXX
 
 protected:
  double GetC(int i);
  void UpdateAlphaStatus(int i);
  bool IsUpperBound(int i);
  bool IsLowerBound(int i);
  bool IsFree(int i);
  void SwapIndex(int i, int j);
  void ReconstructGradient();
  virtual int SelectWorkingSet(int &i, int &j);
  virtual double CalculateRho();
  virtual void DoShrinking();

private:
  bool BeShrunk(int i, double Gmax1, double Gmax2);  
};

}  // namespace nltk

#endif  // SOLVE_H_
