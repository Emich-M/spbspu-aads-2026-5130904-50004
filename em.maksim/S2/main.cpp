#include "BList.hpp"
#include "transpose.hpp"
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

int main() {
  em::SequenceList sequences;

  std::string name;
  while (std::cin >> name) {
    em::IntList numbers;
    int num;
    while (std::cin >> num) {
      numbers.push_back(num);
      std::cin >> std::ws;
      int next_char = std::cin.peek();
      if (next_char == EOF || std::isalpha(next_char)) {
        break;
      }
    }
    sequences.push_back(std::make_pair(name, numbers));
  }

  if (sequences.empty()) {
    std::cout << 0 << std::endl;
    return 0;
  }

  bool first = true;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << std::endl;

  try {
    em::TransposeResult result =
      em::transpose_and_xor(sequences);

    for (auto it = result.transposed.cbegin();
         it != result.transposed.cend(); ++it) {
      first = true;
      for (auto num_it = it->cbegin();
           num_it != it->cend(); ++num_it) {
        if (!first) {
          std::cout << " ";
        }
        std::cout << *num_it;
        first = false;
      }
      std::cout << std::endl;
    }

    first = true;
    for (auto it = result.xor_results.cbegin();
         it != result.xor_results.cend(); ++it) {
      if (!first) {
        std::cout << " ";
      }
      std::cout << *it;
      first = false;
    }
    std::cout << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
