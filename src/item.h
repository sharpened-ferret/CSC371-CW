// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// An Item class contains multiple 'entries' as
// key/value pairs (e.g., a key might be 'username'
// and a value would be the username, another might be
// 'url' and the value is the website address the
// username is for.
// -----------------------------------------------------

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <map>

class Item {
private:
    std::string identifier;
    std::map<std::string, std::string> entries;
public:
    Item(std::string identifier);
    unsigned int size();
    bool empty();
    void setIdent(std::string ident);
    std::string getIdent();
    bool addEntry(std::string key, std::string value);
    std::string getEntry(std::string key);
    bool deleteEntry(std::string key);

    friend bool operator==(const Item& rhs, const Item& lhs);
};

#endif // ITEM_H
