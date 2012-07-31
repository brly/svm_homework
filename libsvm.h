#ifndef LIBSVM_H_
#define LIBSVM_H_

#include "svm.h"

// LIBSVM の インターフェースクラス

struct Libsvm {
  explicit Libsvm(const int kernel_type, const bool problem_init_auto = true);
  ~Libsvm();

  svm_parameter param;
  svm_problem problem;

  void init_param(const int kernel_type);
  void init_problem();

  void run();
};

#endif // LIBSVM_H_

