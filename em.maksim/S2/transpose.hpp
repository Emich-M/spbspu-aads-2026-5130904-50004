#ifndef TRANSPOSE_HPP
#define TRANSPOSE_HPP

#include "BList.hpp"
#include <string>
#include <utility>

namespace em {

  using IntList = List<int>;
  using NamedSequence = std::pair<std::string, IntList>;
  using SequenceList = List<NamedSequence>;

  struct TransposeResult {
    List<IntList> transposed;
    IntList xor_results;
  };

  TransposeResult transpose_and_xor(const SequenceList& sequences);

}

#endif
