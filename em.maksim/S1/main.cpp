#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include "List.h"

static bool parseNumber(
  const std::string& token,
  unsigned long long& result
) {
  try {
    std::size_t pos;
    result = std::stoull(token, &pos);
    if (pos != token.length()) {
      return false;
    }
    return true;
  } catch (const std::out_of_range&) {
    return false;
  } catch (const std::exception&) {
    return false;
  }
}

static void printNames(const em::List<std::string>& names) {
  bool first = true;
  for (auto it = names.cbegin(); it != names.cend(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}

static void printSums(const em::List<unsigned long long>& sums) {
  bool first = true;
  for (auto it = sums.cbegin(); it != sums.cend(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}

int main() {
  em::List<std::string> names;
  em::List<em::List<unsigned long long>> nums;
  std::string token;
  char c;
  bool is_first_token_in_line = true;

  while (std::cin.get(c)) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!token.empty()) {
        if (is_first_token_in_line) {
          names.push_back(token);
          nums.push_back(em::List<unsigned long long>());
          is_first_token_in_line = false;
        } else {
          unsigned long long val;
          if (!parseNumber(token, val)) {
            std::cout << '\n';
            std::cerr << "Overflow\n";
            return 1;
          }
          nums.back().push_back(val);
        }
        token.clear();
      }
      if (c == '\n') {
        is_first_token_in_line = true;
      }
    } else {
      token += c;
    }
  }

  if (!token.empty()) {
    if (is_first_token_in_line) {
      names.push_back(token);
      nums.push_back(em::List<unsigned long long>());
    } else {
      unsigned long long val;
      if (!parseNumber(token, val)) {
        std::cout << '\n';
        std::cerr << "Overflow\n";
        return 1;
      }
      nums.back().push_back(val);
    }
  }

  if (names.empty()) {
    std::cout << "0\n";
    return 0;
  }

  printNames(names);

  std::size_t max_size = 0;
  for (auto it = nums.cbegin(); it != nums.cend(); ++it) {
    if (it->size() > max_size) {
      max_size = it->size();
    }
  }

  if (max_size == 0) {
    std::cout << "0\n";
    return 0;
  }

  em::List<unsigned long long> sums;
  em::List<em::LCIter<unsigned long long>> iters;

  for (auto it = nums.cbegin(); it != nums.cend(); ++it) {
    iters.push_back(it->cbegin());
  }

  try {
    for (std::size_t col = 0; col < max_size; ++col) {
      unsigned long long current_sum = 0;
      bool first_in_row = true;
      bool has_data = false;

      auto iters_it = iters.begin();
      auto nums_it = nums.cbegin();

      for (std::size_t i = 0; i < iters.size(); ++i) {
        if (*iters_it != nums_it->cend()) {
          if (!first_in_row) {
            std::cout << " ";
          }
          std::cout << **iters_it;
          em::sum(current_sum, **iters_it);
          first_in_row = false;
          has_data = true;
          ++(*iters_it);
        }
        if (i < iters.size() - 1) {
          ++iters_it;
          ++nums_it;
        }
      }

      if (has_data) {
        std::cout << '\n';
        sums.push_back(current_sum);
      } else {
        break;
      }
    }
  } catch (const std::overflow_error&) {
    std::cout << '\n';
    std::cerr << "Overflow\n";
    return 1;
  } catch (const std::bad_alloc&) {
    std::cout << '\n';
    std::cerr << "Bad allocation failure\n";
    return 1;
  } catch (const std::exception&) {
    std::cout << '\n';
    std::cerr << "Error during sum processing\n";
    return 1;
  } catch (...) {
    std::cout << '\n';
    std::cerr << "Something went wrong\n";
    return 1;
  }

  printSums(sums);

  return 0;
}
