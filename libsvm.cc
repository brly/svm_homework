#include "libsvm.h"
#include "svm.h"

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cassert>

namespace {

int kernels[] = { LINEAR, POLY, RBF, SIGMOID };

void print_null(const char* s) {}

};

Libsvm::Libsvm(const int kernel_type, const bool problem_init_auto) {
  init_param(kernel_type);
  if (problem_init_auto)
    init_problem();
  svm_set_print_string_function(&::print_null);
}

Libsvm::~Libsvm() {
  if (problem.y)
    delete[] problem.y;
  if (problem.x) {
    for (int i = 0; i < problem.l; ++i)
      if (problem.x[i])
        delete[] problem.x[i];
    delete[] problem.x;
  }
}

void Libsvm::init_param(const int kernel_type) {
  param.svm_type = C_SVC;
  param.kernel_type = kernels[kernel_type];
  param.degree = 3;
  param.gamma = 1;
  param.coef0 = 0;
  param.nu = 0.5;
  param.cache_size = 100;
  param.C = 1;
  param.eps = 1e-3;
  param.p = 0.1;
  param.shrinking = 1;
  param.probability = 1;
  param.nr_weight = 0;
  param.weight_label = NULL;
  param.weight = NULL;
}

void Libsvm::init_problem() {
  // set num of samples
  const int kL = 50;
  problem.l = 50;
  problem.y = new double[kL];
  problem.x = new svm_node*[kL];
  // make sample
  for (int i = 0; i < kL; ++i) {
    double x = static_cast<double>(rand()) / RAND_MAX * 2.0;
    double y = sin(x * 180.0 / M_PI);
    double d = static_cast<double>(rand()) / RAND_MAX / 5;
    if (static_cast<double>(rand())/ RAND_MAX < 0.5) d *= -1;

    problem.x[i] = new svm_node[3];
    problem.x[i][0].index = 1; problem.x[i][0].value = x;
    problem.x[i][1].index = 2; problem.x[i][1].value = y + d;
    problem.x[i][2].index = -1;
    problem.y[i] = (y + d > y) ? 1 : -1;

    printf("%f %f\n", x, y+d);
  }

  puts("\n");
}

void Libsvm::run() {
  // check
  if (svm_check_parameter(&problem, &param) != NULL)
    assert(false);

  // get model
  svm_model *model = svm_train(&problem, &param);

  // print
  for (int i = 0; i < model->l; ++i) {
    printf("%f %f\n", model->SV[i][0].value, model->SV[i][1].value);
  }

  // release
  svm_free_and_destroy_model(&model);
}
