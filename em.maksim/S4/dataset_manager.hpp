#ifndef EM_DATASET_MANAGER_HPP
#define EM_DATASET_MANAGER_HPP

#include "bstree.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace em
{

using Dataset = BSTree<int, std::string>;
using DatasetMap = BSTree<std::string, Dataset>;

void load_datasets_from_file(DatasetMap &datasets, const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    std::exit(1);
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    size_t pos = 0;
    std::string name;
    while (pos < line.size() && line[pos] != ' ') {
      name += line[pos];
      ++pos;
    }
    if (name.empty()) {
      continue;
    }
    if (pos < line.size()) {
      ++pos;
    }

    Dataset ds;
    while (pos < line.size()) {
      int key = 0;
      bool negative = false;
      if (line[pos] == '-') {
        negative = true;
        ++pos;
      }
      while (pos < line.size() && line[pos] >= '0' && line[pos] <= '9') {
        key = key * 10 + (line[pos] - '0');
        ++pos;
      }
      if (negative) {
        key = -key;
      }
      if (pos < line.size()) {
        ++pos;
      }

      std::string value;
      while (pos < line.size() && line[pos] != ' ') {
        value += line[pos];
        ++pos;
      }
      ds.push(key, value);
      if (pos < line.size()) {
        ++pos;
      }
    }
    datasets.push(name, std::move(ds));
  }
}

void print_dataset(const Dataset &ds, const std::string &name)
{
  if (ds.empty()) {
    std::cout << "<EMPTY>" << std::endl;
    return;
  }
  std::cout << name;
  for (const auto &kv : ds) {
    std::cout << " " << kv.first << " " << kv.second;
  }
  std::cout << std::endl;
}

Dataset set_union(const Dataset &a, const Dataset &b)
{
  Dataset result = a;
  for (const auto &kv : b) {
    if (!result.findNode(kv.first)) {
      result.push(kv.first, kv.second);
    }
  }
  return result;
}

Dataset set_intersect(const Dataset &a, const Dataset &b)
{
  Dataset result;
  for (const auto &kv : a) {
    if (b.findNode(kv.first)) {
      result.push(kv.first, kv.second);
    }
  }
  return result;
}

Dataset set_complement(const Dataset &a, const Dataset &b)
{
  Dataset result;
  for (const auto &kv : a) {
    if (!b.findNode(kv.first)) {
      result.push(kv.first, kv.second);
    }
  }
  return result;
}

bool execute_command(DatasetMap &datasets, const std::string &line)
{
  size_t pos = 0;
  std::string cmd;
  while (pos < line.size() && line[pos] != ' ') {
    cmd += line[pos];
    ++pos;
  }
  if (pos < line.size()) {
    ++pos;
  }

  if (cmd == "print") {
    std::string name;
    while (pos < line.size() && line[pos] != ' ') {
      name += line[pos];
      ++pos;
    }
    if (name.empty()) {
      return false;
    }
    auto *ds_node = datasets.findNode(name);
    if (!ds_node) {
      std::cout << "<INVALID COMMAND>" << std::endl;
      return true;
    }
    print_dataset(ds_node->data_.second, name);
    return true;
  }

  std::string new_name, name1, name2;
  while (pos < line.size() && line[pos] != ' ') {
    new_name += line[pos];
    ++pos;
  }
  if (pos < line.size()) {
    ++pos;
  }
  while (pos < line.size() && line[pos] != ' ') {
    name1 += line[pos];
    ++pos;
  }
  if (pos < line.size()) {
    ++pos;
  }
  while (pos < line.size() && line[pos] != ' ') {
    name2 += line[pos];
    ++pos;
  }

  if (new_name.empty() || name1.empty() || name2.empty()) {
    return false;
  }

  auto *ds1 = datasets.findNode(name1);
  auto *ds2 = datasets.findNode(name2);
  if (!ds1 || !ds2) {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return true;
  }

  if (datasets.findNode(new_name)) {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return true;
  }

  Dataset result;
  if (cmd == "complement") {
    result = set_complement(ds1->data_.second, ds2->data_.second);
  } else if (cmd == "intersect") {
    result = set_intersect(ds1->data_.second, ds2->data_.second);
  } else if (cmd == "union") {
    result = set_union(ds1->data_.second, ds2->data_.second);
  } else {
    return false;
  }

  datasets.push(new_name, std::move(result));
  return true;
}

}

#endif
