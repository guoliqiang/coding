// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 18:45:12
// File  : common.h
// Brief :

#ifndef  __COMMON_H_
#define  __COMMON_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <locale.h>


namespace nltk {

#define LIBSVM_VERSION 317
#define INF HUGE_VAL
#define TAU 1e-12
#define Malloc(type, n) (type *) malloc((n) * sizeof(type))

typedef float Qfloat;
typedef signed char schar;

class SvmNode;
class SvmModel;
class SvmProblem;
class SvmParameter;

// svm_type
enum svm_type {
  C_SVC = 0,
  NU_SVC = 1,
  ONE_CLASS = 2,
  EPSILON_SVR = 3,
  NU_SVR = 4 
};

//  kernel_type
enum kernal_type {
  LINEAR = 0,
  POLY = 1,
  RBF = 2,
  SIGMOID = 3,
  PRECOMPUTED = 4 
};

template <class T>
inline T min(T x, T y) {
  return (x < y) ? x:y;
}

template <class T>
inline T max(T x,T y) {
  return (x > y) ? x : y;
}

template <class T>
inline void swap(T& x, T& y) {
  T t=x;
  x=y;
  y=t;
}

template <class S, class T>
inline void clone(T*& dst, S* src, int n) {
  dst = new T[n];
  memcpy((void *)dst, (void *)src, sizeof(T)*n);
}

inline double powi(double base, int times) {
  double tmp = base, ret = 1.0;
  for(int t = times; t > 0; t /= 2) {
    if(t%2==1) ret*=tmp;
    tmp = tmp * tmp;
  }
  return ret;
}

void print_string_stdout(const char *s) {
  fputs(s,stdout);
  fflush(stdout);
}

static void (*svm_print_string) (const char *) = &print_string_stdout;

void info(const char *fmt,...) {
  char buf[BUFSIZ];
  va_list ap;
  va_start(ap,fmt);
  vsprintf(buf,fmt,ap);
  va_end(ap);
  (*svm_print_string)(buf);
}



SvmModel *svm_train(const SvmProblem *prob,
                     const SvmParameter *param);

void svm_cross_validation(const SvmProblem *prob,
                          const SvmParameter *param,
                          int nr_fold,
                          double *target);

int svm_save_model(const char *model_file_name,
                   const SvmModel *model);
SvmModel *svm_load_model(const char *model_file_name);

int svm_get_svm_type(const SvmModel *model);
int svm_get_nr_class(const SvmModel *model);
void svm_get_labels(const SvmModel *model,
                    int *label);
void svm_get_sv_indices(const SvmModel *model,
                        int *sv_indices);
int svm_get_nr_sv(const SvmModel *model);
double svm_get_svr_probability(const SvmModel *model);

double svm_predict_values(const SvmModel *model,
                          const SvmNode *x,
                          double* dec_values);
double svm_predict(const SvmModel *model,
                   const SvmNode *x);
double svm_predict_probability(const SvmModel *model,
                               const SvmNode *x,
                               double* prob_estimates);

void svm_free_model_content(SvmModel *model_ptr);
void svm_free_and_destroy_model(SvmModel **model_ptr_ptr);
void svm_destroy_param(SvmParameter *param);

const char *svm_check_parameter(const SvmProblem *prob,
                                const SvmParameter *param);
int svm_check_probability_model(const SvmModel *model);

void svm_set_print_string_function(void (*print_func)(const char *));

}  // namespace nltk

#endif  // __COMMON_H_
