// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "item.h"
#include <string>
#include <exception>
#include <stdexcept>

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
Item::Item(std::string identifier): identifier(identifier) {}
//
// Example:
//  Item iObj{"identIdent"};

// Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of entries in the Item contains.
//
unsigned int Item::size() {
    return this->entries.size();
}
// Example:
//  Item iObj{"identIdent"};
//  auto size = iObj.size();

// Write a function, empty, that takes no parameters and returns true
//  if the number of entries in the Item is zero, false otherwise.
bool Item::empty() {
    return this->entries.empty();
}
// Example:
//  Item iObj{"identIdent"};
//  auto empty = iObj.empty();

// Write a function, setIdent, that takes one parameter, a string for a new
//  Item identifier, and updates the member variable. It returns nothing.
void Item::setIdent(std::string ident) {
    this->identifier = std::move(ident);
}
// Example:
//  Item iObj{"identIdent"};
//  iObj.setIdent("identIdent2");

// Write a function, getIdent, that returns the identifier for the Item.
std::string Item::getIdent() {
    return this->identifier;
}
// Example:
//  Item iObj{"identIdent"};
//  auto ident = iObj.getIdent();

// Write a function, addEntry, that takes two parameters, an entry
//  key and value and returns true if the entry was inserted into the
//  container or false if the entry already existed and was replaced.
bool Item::addEntry(std::string key, std::string value) {
    const auto status = this->entries.insert({key, value});
    return status.second;
}
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");

// Write a function, getEntry, that takes one parameter, an entry
//  key and returns it's value. If no entry exists, throw an appropriate
//  exception.
std::string Item::getEntry(std::string key) {
    if (this->entries.count(key) == 0) {
        throw std::out_of_range("no value exists for key");
    } else {
        return this->entries.at(key);
    }
}
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  auto value = iObj.getEntry("key");

// Write a function, deleteEntry, that takes one parameter, an entry
//  key, deletes it from the container, and returns true if the Item was
//  deleted. If no entry exists, throw an appropriate exception.
bool Item::deleteEntry(std::string key) {
    if (this->entries.count(key) == 0) {
        throw std::out_of_range("no value exists for key");
    } else {
        return this->entries.erase(key);
    }
}
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  iObj.deleteEntry("key");

// TODO Write an == operator overload for the Item class, such that two
//  Item objects are equal only if they have the same identifier and same
//  entries.
//
// Example:
//  Item iObj1{"identIdent"};
//  iObj1.addEntry("key", "value");
//  Item iObj2{"identIdent2"};
//  if(iObj1 == iObj2) {
//    ...
//  }

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();