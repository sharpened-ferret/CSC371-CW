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

class Category {
private:
    std::string identifier;
    std::map<std::string, std::string> items;
public:
    Category(std::string identifier);
    unsigned int size();
    bool empty();
};

#endif // CATEGORY_H
