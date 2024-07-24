#ifndef subtree_extraction_failed_H
#define subtree_extraction_failed_H

#include<stdexcept>
#include <string>

class subtree_extraction_failed: public std::runtime_error {

public: 
    subtree_extraction_failed(const std::string &message): std::runtime_error(message) {}
};

#endif