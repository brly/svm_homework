#include "libsvm.h"
#include "svm.h"

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cassert>
#include <ctime>

#include <vector>
#include <utility>

namespace {

int kernels[] = { LINEAR, POLY, RBF, SIGMOID };

void print_null(const char* s) {}

};

Libsvm::Libsvm(const int kernel_type, const bool problem_init_auto) {
  srand(static_cast<unsigned>(time(NULL)));
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
  const int kL = 500;
  problem.l = kL;
  problem.y = new double[kL];
  problem.x = new svm_node*[kL];
  // make sample
  for (int i = 0; i < kL; ++i) {
    double x = static_cast<double>(rand()) / RAND_MAX * 4.0 * M_PI;
    double y = sin(x);
    double d = static_cast<double>(rand()) / RAND_MAX / 2;
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

  // support vector
  typedef std::pair<double, double> P;
  std::vector<P> psv, nsv, neg, pos;
  for (int i = 0; i < model->l; ++i) {
    P p = P(model->SV[i][0].value, model->SV[i][1].value);
    if (model->sv_coef[0][i] > 0)
      psv.push_back(p);
    else
      nsv.push_back(p);
  }

  // predict
  for (int i = 0; i < 1000; ++i) {
    svm_node query[3];
    double x = (static_cast<double>(rand()) / RAND_MAX) * 4.0 * M_PI;
    double y = static_cast<double>(rand()) / RAND_MAX;
    if ((static_cast<double>(rand()) / RAND_MAX) < 0.5 ) y *= -1;
    
    query[0].index = 1; query[0].value = x;
    query[1].index = 2; query[1].value = y;
    query[2].index = -1;

    if (svm_predict(model, query) > 0)
      pos.push_back(P(x, y));
    else
      neg.push_back(P(x, y));
  }

  // print
  // positive support vector
  for (unsigned i = 0; i < psv.size(); ++i)
    printf("%f %f\n", psv[i].first, psv[i].second);
  puts("\n");
  // negative supprot vector
  for (unsigned i = 0; i < nsv.size(); ++i)
    printf("%f %f\n", nsv[i].first, nsv[i].second);
  puts("\n");
  // positive
  for (unsigned i = 0; i < pos.size(); ++i)
    printf("%f %f\n", pos[i].first, pos[i].second);
  puts("\n");
  // negative
  for (unsigned i = 0; i < neg.size(); ++i)
    printf("%f %f\n", neg[i].first, neg[i].second);
  puts("\n");

  // release
  svm_free_and_destroy_model(&model);
}
