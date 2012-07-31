#include "libsvm.h"

#include <string>

namespace {

const std::string kernels[] = {
  "Linear", "Polynomial", "RBF", "Sigmoid"
};

int print_error() {
  puts("usage : ./application [0-3]");
  puts("[0-3] is kernel_type.");
  for (int i = 0; i < 4; ++i)
    printf("%d : %s\n", i, kernels[i].c_str());
  return 1;
}

};

int main(int argc, char**argv) {
  // check argument count
  if (argc != 2) return print_error();

  const int kernel_type = atoi(argv[1]);
  // check validation of kernel_type
  if (kernel_type < 0 || 3 < kernel_type) return print_error();

  // running
  Libsvm libsvm(kernel_type, true);
  libsvm.run();
  
  return 0;
}
