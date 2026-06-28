#ifndef EM_GRAPH_H
#define EM_GRAPH_H

#include <string>
#include <utility>

#include "blake2b.h"
#include "hash_table.h"
#include "list.h"

namespace em {

  struct Graph {
    List<std::string> vertices;
    HashTable<std::pair<std::string, std::string>, List<unsigned int>,
              Blake2bPairHash, PairEqual> edges;

    bool hasVertex(const std::string& vertex) const;
    void addVertex(const std::string& vertex);
  };

}

#endif
