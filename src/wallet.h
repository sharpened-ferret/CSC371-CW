// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// The root object that holds all data for 371pass. This
// class contains Categories, which in turn hold Items,
// which hold a mapping of entries.
// -----------------------------------------------------

#ifndef WALLET_H
#define WALLET_H

#include <map>
#include <string>
#include "category.h"

class Wallet {
private:
    std::map<std::string, Category> categories;
public:
    Wallet();
    unsigned int size();
    bool empty();
    Category& newCategory(const std::string& identifier);
    bool addCategory(Category category);
    Category& getCategory(const std::string& ident);
    bool deleteCategory(const std::string& ident);
    void load(const std::string& filename);
    void save(const std::string& filename);
    std::string str();

    friend bool operator==(Wallet& lhs, Wallet& rhs);
};

#endif // WALLET_H
