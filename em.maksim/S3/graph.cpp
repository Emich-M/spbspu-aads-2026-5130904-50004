#include "graph.h"

#include <string>

namespace em {

  bool Graph::hasVertex(const std::string& vertex) const {
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      if (*it == vertex) {
        return true;
      }
    }
    return false;
  }

  void Graph::addVertex(const std::string& vertex) {
    if (!hasVertex(vertex)) {
      vertices.push_back(vertex);
    }
  }

}
