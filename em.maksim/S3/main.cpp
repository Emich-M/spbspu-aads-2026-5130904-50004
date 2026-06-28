#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "blake2b.h"
#include "graph.h"
#include "hash_table.h"
#include "vector.h"

namespace {

  void sort_pairs(em::Vector<std::pair<std::string, em::Vector<unsigned int>>>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
      for (size_t j = 0; j + 1 < vec.size() - i; ++j) {
        if (vec[j].first > vec[j + 1].first) {
          auto tmp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = tmp;
        }
      }
    }
  }

}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::ifstream fin(argv[1]);
  if (!fin.is_open()) {
    std::cerr << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }

  em::HashTable<std::string, em::Graph, em::Blake2bHash, em::StringEqual> graphs_table(64);

  std::string token;
  while (fin >> token) {
    std::string gname = token;
    fin >> token;
    size_t edge_count = std::stoul(token);
    em::Graph g;
    for (size_t i = 0; i < edge_count; ++i) {
      std::string v1, v2, w_str;
      fin >> v1 >> v2 >> w_str;
      unsigned int w = std::stoul(w_str);
      g.addVertex(v1);
      g.addVertex(v2);
      auto key = std::make_pair(v1, v2);
      if (g.edges.has(key)) {
        g.edges.at(key).push_back(w);
      } else {
        em::List<unsigned int> weights;
        weights.push_back(w);
        g.edges.add(key, weights);
      }
    }
    if (graphs_table.has(gname)) {
      graphs_table.at(gname) = g;
    } else {
      graphs_table.add(gname, g);
    }
  }
  fin.close();

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmd == "graphs") {
      em::Vector<std::string> names;
      for (auto it = graphs_table.begin(); it != graphs_table.end(); ++it) {
        names.push_back((*it).first);
      }
      em::sort_strings(names);
      for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << "\n";
      }
    } else if (cmd == "vertexes") {
      std::string gname;
      if (!(std::cin >> gname)) {
        break;
      }
      if (!graphs_table.has(gname)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Graph& g = graphs_table.at(gname);
      em::Vector<std::string> verts;
      for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
        verts.push_back(*it);
      }
      em::sort_strings(verts);
      for (size_t i = 0; i < verts.size(); ++i) {
        std::cout << verts[i] << "\n";
      }
    } else if (cmd == "outbound") {
      std::string gname, v;
      if (!(std::cin >> gname >> v)) {
        break;
      }
      if (!graphs_table.has(gname)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Graph& g = graphs_table.at(gname);
      if (!g.hasVertex(v)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Vector<std::pair<std::string, em::Vector<unsigned int>>> out_verts;
      for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
        auto& key = (*it).first;
        auto& weights = (*it).second;
        if (key.first == v) {
          bool found = false;
          for (size_t i = 0; i < out_verts.size(); ++i) {
            if (out_verts[i].first == key.second) {
              for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                out_verts[i].second.push_back(*wit);
              }
              found = true;
              break;
            }
          }
          if (!found) {
            em::Vector<unsigned int> w_vec;
            for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
              w_vec.push_back(*wit);
            }
            out_verts.push_back(std::make_pair(key.second, w_vec));
          }
        }
      }
      sort_pairs(out_verts);
      for (size_t i = 0; i < out_verts.size(); ++i) {
        em::sort_uints(out_verts[i].second);
        std::cout << out_verts[i].first;
        for (size_t j = 0; j < out_verts[i].second.size(); ++j) {
          std::cout << " " << out_verts[i].second[j];
        }
        std::cout << "\n";
      }
    } else if (cmd == "inbound") {
      std::string gname, v;
      if (!(std::cin >> gname >> v)) {
        break;
      }
      if (!graphs_table.has(gname)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Graph& g = graphs_table.at(gname);
      if (!g.hasVertex(v)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Vector<std::pair<std::string, em::Vector<unsigned int>>> in_verts;
      for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
        auto& key = (*it).first;
        auto& weights = (*it).second;
        if (key.second == v) {
          bool found = false;
          for (size_t i = 0; i < in_verts.size(); ++i) {
            if (in_verts[i].first == key.first) {
              for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                in_verts[i].second.push_back(*wit);
              }
              found = true;
              break;
            }
          }
          if (!found) {
            em::Vector<unsigned int> w_vec;
            for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
              w_vec.push_back(*wit);
            }
            in_verts.push_back(std::make_pair(key.first, w_vec));
          }
        }
      }
      sort_pairs(in_verts);
      for (size_t i = 0; i < in_verts.size(); ++i) {
        em::sort_uints(in_verts[i].second);
        std::cout << in_verts[i].first;
        for (size_t j = 0; j < in_verts[i].second.size(); ++j) {
          std::cout << " " << in_verts[i].second[j];
        }
        std::cout << "\n";
      }
    } else if (cmd == "bind") {
      std::string gname, v1, v2, w_str;
      if (!(std::cin >> gname >> v1 >> v2 >> w_str)) {
        break;
      }
      if (!graphs_table.has(gname)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      unsigned int w = std::stoul(w_str);
      em::Graph& g = graphs_table.at(gname);
      g.addVertex(v1);
      g.addVertex(v2);
      auto key = std::make_pair(v1, v2);
      if (g.edges.has(key)) {
        g.edges.at(key).push_back(w);
      } else {
        em::List<unsigned int> weights;
        weights.push_back(w);
        g.edges.add(key, weights);
      }
    } else if (cmd == "cut") {
      std::string gname, v1, v2, w_str;
      if (!(std::cin >> gname >> v1 >> v2 >> w_str)) {
        break;
      }
      if (!graphs_table.has(gname)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Graph& g = graphs_table.at(gname);
      if (!g.hasVertex(v1) || !g.hasVertex(v2)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      auto key = std::make_pair(v1, v2);
      if (!g.edges.has(key)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      unsigned int w = std::stoul(w_str);
      em::List<unsigned int>& weights = g.edges.at(key);
      bool found = false;
      em::List<unsigned int> new_weights;
      for (auto it = weights.begin(); it != weights.end(); ++it) {
        if (*it == w && !found) {
          found = true;
        } else {
          new_weights.push_back(*it);
        }
      }
      if (!found) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      weights = new_weights;
      if (weights.size() == 0) {
        g.edges.drop(key);
      }
    } else if (cmd == "create") {
      std::string gname;
      if (!(std::cin >> gname)) {
        break;
      }
      if (graphs_table.has(gname)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string count_str;
      if (!(std::cin >> count_str)) {
        break;
      }
      size_t count = std::stoul(count_str);
      em::Graph g;
      for (size_t i = 0; i < count; ++i) {
        std::string v;
        if (!(std::cin >> v)) {
          break;
        }
        g.addVertex(v);
      }
      graphs_table.add(gname, g);
    } else if (cmd == "merge") {
      std::string new_g, g1, g2;
      if (!(std::cin >> new_g >> g1 >> g2)) {
        break;
      }
      if (graphs_table.has(new_g) || !graphs_table.has(g1) || !graphs_table.has(g2)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      em::Graph& src1 = graphs_table.at(g1);
      em::Graph& src2 = graphs_table.at(g2);
      em::Graph new_graph;
      for (auto it = src1.vertices.begin(); it != src1.vertices.end(); ++it) {
        new_graph.addVertex(*it);
      }
      for (auto it = src2.vertices.begin(); it != src2.vertices.end(); ++it) {
        new_graph.addVertex(*it);
      }
      for (auto it = src1.edges.begin(); it != src1.edges.end(); ++it) {
        auto& key = (*it).first;
        auto& weights = (*it).second;
        if (new_graph.edges.has(key)) {
          for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
            new_graph.edges.at(key).push_back(*wit);
          }
        } else {
          new_graph.edges.add(key, weights);
        }
      }
      for (auto it = src2.edges.begin(); it != src2.edges.end(); ++it) {
        auto& key = (*it).first;
        auto& weights = (*it).second;
        if (new_graph.edges.has(key)) {
          for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
            new_graph.edges.at(key).push_back(*wit);
          }
        } else {
          new_graph.edges.add(key, weights);
        }
      }
      graphs_table.add(new_g, new_graph);
    } else if (cmd == "extract") {
      std::string new_g, old_g;
      if (!(std::cin >> new_g >> old_g)) {
        break;
      }
      if (graphs_table.has(new_g) || !graphs_table.has(old_g)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string count_str;
      if (!(std::cin >> count_str)) {
        break;
      }
      size_t count = std::stoul(count_str);
      em::Graph& src = graphs_table.at(old_g);
      em::Vector<std::string> verts;
      for (size_t i = 0; i < count; ++i) {
        std::string v;
        if (!(std::cin >> v)) {
          break;
        }
        if (!src.hasVertex(v)) {
          std::cout << "<INVALID COMMAND>\n";
          return 0;
        }
        verts.push_back(v);
      }
      em::Graph new_graph;
      for (size_t i = 0; i < verts.size(); ++i) {
        new_graph.addVertex(verts[i]);
      }
      for (auto it = src.edges.begin(); it != src.edges.end(); ++it) {
        auto& key = (*it).first;
        bool v1_found = false;
        bool v2_found = false;
        for (size_t i = 0; i < verts.size(); ++i) {
          if (verts[i] == key.first) {
            v1_found = true;
          }
          if (verts[i] == key.second) {
            v2_found = true;
          }
        }
        if (v1_found && v2_found) {
          new_graph.edges.add(key, (*it).second);
        }
      }
      graphs_table.add(new_g, new_graph);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
