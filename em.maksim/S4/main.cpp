#include "dataset_manager.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  em::DatasetMap datasets;
  em::load_datasets_from_file(datasets, argv[1]);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    if (!em::execute_command(datasets, line)) {
      std::cout << "<INVALID COMMAND>" << std::endl;
    }
  }

  return 0;
}
