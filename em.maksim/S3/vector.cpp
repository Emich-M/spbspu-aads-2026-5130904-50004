#include "vector.h"

#include <string>
#include <utility>

namespace em {

  void sort_strings(Vector<std::string>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
      for (size_t j = 0; j + 1 < vec.size() - i; ++j) {
        if (vec[j] > vec[j + 1]) {
          std::string tmp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = tmp;
        }
      }
    }
  }

  void sort_uints(Vector<unsigned int>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
      for (size_t j = 0; j + 1 < vec.size() - i; ++j) {
        if (vec[j] > vec[j + 1]) {
          unsigned int tmp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = tmp;
        }
      }
    }
  }

}
