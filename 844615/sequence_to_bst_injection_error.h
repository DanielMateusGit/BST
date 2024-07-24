#ifndef sequence_to_bst_injection_error_H
#define sequence_to_bst_injection_error_H

#include<stdexcept>
#include <string>

class sequence_to_bst_injection_error: public std::runtime_error{
public : 
  sequence_to_bst_injection_error(const std::string &message): std::runtime_error(message) {}
};

#endif