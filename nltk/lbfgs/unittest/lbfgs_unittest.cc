#include "nltk/lbfgs/public/lbfgs.h"
#include <stdio.h>

class objective_function {
 protected:
  double *m_x;

 public:
  objective_function() : m_x(NULL) { }
  virtual ~objective_function() {
    if (m_x != NULL) {
      nltk::lbfgs::lbfgs_free(m_x);
      m_x = NULL;
    }
  }

  int run(int N) {
    double fx;
    double * m_x = nltk::lbfgs::lbfgs_malloc(N);

    if (m_x == NULL) {
      printf("ERROR: Failed to allocate a memory block for variables.\n");
      return 1;
    }

    // Initialize the variables.
    for (int i = 0;i < N;i += 2) {
      m_x[i] = -1.2;
      m_x[i+1] = 1.0;
    }

    // Start the L-BFGS optimization; this will invoke the callback functions
    // evaluate() and progress() when necessary.
    int ret = nltk::lbfgs::lbfgs(N, m_x, &fx, _evaluate, _progress, this, NULL);

    // Report the result.
    printf("L-BFGS optimization terminated with status code = %d\n", ret);
    printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, m_x[0], m_x[1]);

    return ret;
  }

 protected:
  static double _evaluate(void * instance, const double * x, double * g,
                          const int n, const double step) {
    return reinterpret_cast<objective_function*>(instance)->evaluate(
        x, g, n, step);
  }

  double evaluate(const double * x, double * g, const int n,
                  const double step) {
    double fx = 0.0;

    for (int i = 0;i < n;i += 2) {
      double t1 = 1.0 - x[i];
      double t2 = 10.0 * (x[i+1] - x[i] * x[i]);
      g[i+1] = 20.0 * t2;
      g[i] = -2.0 * (x[i] * g[i+1] + t1);
      fx += t1 * t1 + t2 * t2;
    }
    return fx;
  }

  static int _progress(void * instance, const double * x, const double * g,
                       const double fx, const double xnorm, const double gnorm,
                       const double step, int n, int k, int ls) {
    return reinterpret_cast<objective_function*>(instance)->progress(
        x, g, fx, xnorm, gnorm, step, n, k, ls);
  }

  int progress(const double * x, const double * g, const double fx,
               const double xnorm, const double gnorm, const double step,
               int n, int k, int ls) {
    printf("Iteration %d:\n", k);
    printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, x[0], x[1]);
    printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
    printf("\n");
    return 0;
  }
};

#define N 100

int main(int argc, char *argv[]) {
  objective_function obj;
  return obj.run(N);
}
