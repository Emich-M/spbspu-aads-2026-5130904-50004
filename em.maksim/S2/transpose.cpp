#include "transpose.hpp"
#include <stdexcept>

namespace em {

  TransposeResult transpose_and_xor(const SequenceList& sequences) {
    TransposeResult result;

    if (sequences.empty()) {
      return result;
    }

    size_t max_len = 0;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      if (it->second.size() > max_len) {
        max_len = it->second.size();
      }
    }

    if (max_len == 0) {
      throw std::logic_error(
        "cannot compute XOR: all sequences are empty"
      );
    }

    for (size_t i = 0; i < max_len; ++i) {
      IntList transposed;
      int xor_val = 0;
      bool has_value = false;

      for (auto it = sequences.cbegin();
           it != sequences.cend(); ++it) {
        if (i < it->second.size()) {
          auto num_it = it->second.cbegin();
          for (size_t j = 0; j < i; ++j) {
            ++num_it;
          }
          int val = *num_it;
          transposed.push_back(val);
          xor_val ^= val;
          has_value = true;
        }
      }

      if (has_value) {
        result.transposed.push_back(transposed);
        result.xor_results.push_back(xor_val);
      }
    }

    return result;
  }

}
