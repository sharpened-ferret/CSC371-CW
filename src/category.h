// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// A category contains one or more Items, each with
// their own identifier ('ident').
// -----------------------------------------------------

#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <map>
#include "item.h"

class Category {
private:
    std::string identifier;
    std::map<std::string, Item> items;
public:
    Category(std::string identifier);
    unsigned int size();
    bool empty();
    std::string getIdent();
    void setIdent(std::string ident);
    Item& newItem(std::string identifier);
    bool addItem(Item item);
    Item& getItem(std::string identifier);
    bool deleteItem(std::string identifier);
};

#endif // CATEGORY_H
