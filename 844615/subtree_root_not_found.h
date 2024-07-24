#ifndef subtree_root_not_found_H
#define subtree_root_not_found_H

#include<stdexcept>
#include <string>

class subtree_root_not_found: public std::invalid_argument {
public:
  subtree_root_not_found(const std::string &message): std::invalid_argument(message) {}
};

#endif