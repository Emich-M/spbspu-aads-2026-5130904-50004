#ifndef EM_BLAKE2B_H
#define EM_BLAKE2B_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <utility>

namespace em {

  struct Blake2bHash {
    std::size_t operator()(const std::string& key) const;
  };

  struct Blake2bPairHash {
    std::size_t operator()(
      const std::pair<std::string, std::string>& key) const;
  };

  struct StringEqual {
    bool operator()(
      const std::string& a,
      const std::string& b) const;
  };

  struct PairEqual {
    bool operator()(
      const std::pair<std::string, std::string>& a,
      const std::pair<std::string, std::string>& b) const;
  };

}

#endif
