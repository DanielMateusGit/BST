#ifndef unique_contraint_violation_H
#define unique_contraint_violation_H

#include<stdexcept>
#include <string>

class unique_contraint_violation: public std::invalid_argument {
public:
  unique_contraint_violation(const std::string &message): std::invalid_argument(message) {}
};

#endif