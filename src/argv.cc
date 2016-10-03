#include "argv.h"

#include <iostream>
#include <utility>


Argv::Argv() : longest_(0) {
  Add('h', "help", "Show this help message.");
}

void Argv::Add(char little, std::string big, std::string description) {
  if (little != ' ') {
    littles_.insert(little);
    little_to_big_.insert(std::make_pair(little, big));
  }
  bigs_.insert(big);
  toggled_.insert(std::make_pair(big, false));
  descriptions_.insert(std::make_pair(big, description));
  if (big.size() > longest_) {
    longest_ = big.size();
  }
}

char Argv::BigsFiber(std::string big) {
  for (const auto& x : little_to_big_) {
    if (x.second == big) {
      return x.first;
    }
  }
  return ' ';
}

void Argv::Help() {
  std::cerr << "Usage: " << program_name_
            << " [options] program.quiv <initial accumulator value>"
            << std::endl;
  std::cerr << "Options:" << std::endl;
  for (const auto& desc : descriptions_) {
    char c = BigsFiber(desc.first);
    std::cerr << "  ";
    if (c == ' ') {
      std::cerr << "    ";
    } else {
      std::cerr << "-" << c << ", ";
    }
    std::cerr << "--" << desc.first;
    for (size_t i = 0; i < longest_ - desc.first.size(); i++) {
      std::cerr << " ";
    }
    std::cerr << "    " << desc.second << std::endl;
  }
}

void Argv::Parse(int argc, char* argv[]) {
  program_name_ = argv[0];
  for (size_t i = 1; i < (size_t) argc; i++) {
    std::string arg(argv[i]);
    if (arg[0] == '-') {
      if (arg.size() == 1) {
        std::cerr << "Warning: Stray -." << std::endl;
        continue;
      }
      if (arg[1] == '-') {
        arg.erase(0, 2);
        if (bigs_.find(arg) != bigs_.end()) {
          toggled_.at(arg) = true;
        } else {
          std::cerr << "Warning: Unrecognized option "
                    << arg << "." << std::endl;
        }
      } else {
        arg.erase(0, 1);
        if (littles_.find(arg[0]) != littles_.end()) {
          toggled_.at(little_to_big_.at(arg[0])) = true;
        } else {
          std::cerr << "Warning: Unrecognized option "
                    << arg << "." << std::endl;
        }
      }
    } else {
      rest_.push_back(arg);
    }
  }
}

bool Argv::Check(std::string s) {
  if (toggled_.find(s) == toggled_.end()) {
    return false;
  } else {
    return toggled_.at(s);
  }
}

const std::vector<std::string>& Argv::Rest() {
  return rest_;
}
