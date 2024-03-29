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
    Item& newItem(const std::string& ident);
    bool addItem(Item item);
    Item& getItem(const std::string& ident);
    bool deleteItem(const std::string& ident);
    std::string str() const;

    friend bool operator==(const Category& lhs, const Category& rhs);
    friend void merge(Category& initial, Category& additional);
    friend void to_json(nlohmann::json& j, const Category& category);
};

#endif // CATEGORY_H
