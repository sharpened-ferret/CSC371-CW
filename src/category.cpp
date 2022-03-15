// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "category.h"
#include <string>
#include <utility>
#include <stdexcept>

// Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
Category::Category(std::string identifier): identifier(std::move(identifier)) {}
// Example:
//  Category c{"categoryIdent"};

// Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of Items in the Category contains.
unsigned int Category::size() {
    return this->items.size();
}
// Example:
//  Category c{"categoryIdent"};
//  auto size = c.size();

// Write a function, empty, that takes no parameters and returns true
//  if the number of Items in the Category is zero, false otherwise.
bool Category::empty() {
    return this->items.empty();
}
// Example:
//  Category c{"categoryIdent"};
//  auto empty = c.empty();

// Write a function, getIdent, that returns the identifier for the
//  Category.
std::string Category::getIdent() {
    return this->identifier;
}
// Example:
//  Category cObj{"categoryIdent"};
//  auto ident = cObj.getIdent();

// Write a function, setIdent, that takes one parameter, a string for a new
//  Category identifier, and updates the member variable. It returns nothing.
void Category::setIdent(std::string ident) {
    this->identifier = std::move(ident);
}
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.setIdent("categoryIdent2");

// Write a function, newItem, that takes one parameter, an Item identifier,
//  (a string) and returns the Item object as a reference. If an object with the
//  same identifier already exists, then the existing object should be returned.
//  Throw a std::runtime_error if the Item object cannot be inserted into the
//  container for whatever reason.
Item& Category::newItem(std::string identifier) {
    if (this->items.count(identifier) == 0) {
        const auto status = this->items.insert({identifier, Item(identifier)});
        if (status.second) {
            return this->items.at(identifier);
        } else {
            throw std::runtime_error("add item failed");
        }
    } else {
        return this->items.at(identifier);
    }
}
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");

// TODO Write a function, addItem, that takes one parameter, an Item object,
//  and returns true if the object was successfully inserted. If an object with
//  the same identifier already exists, then the contents should be merged and
//  return false.
bool Category::addItem(Item item) {
    if (this->items.count(item.getIdent()) == 0) {
        const auto status = this->items.insert({item.getIdent(), item});
        return status.second;
    } else {
        // TODO handle merge between existing item and added item
        Item existingCopy = this->items.at(item.getIdent());
        throw std::invalid_argument("not yet implemented - TODO");
    }
}
// Example:
//  Category cObj{"categoryIdent"};
//  Item iObj{"itemIdent"};
//  cObj.addItem(iObj);

// Write a function, getItem, that takes one parameter, an Item
//  identifier (a string) and returns the Item as a reference. If no Item
//  exists, throw an appropriate exception.
Item& Category::getItem(std::string identifier) {
    if (this->items.count(identifier) == 0) {
        throw std::out_of_range("no item exists with identifier");
    } else {
        return items.at(identifier);
    }
}
// Hint:
//  See the test scripts for the exception expected.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  auto iObj = cObj.getItem("itemIdent");

// Write a function, deleteItem, that takes one parameter, an Item
//  identifier (a string), deletes it from the container, and returns true if
//  the Item was deleted. If no Item exists, throw an appropriate exception.
bool Category::deleteItem(std::string identifier) {
    if (this->items.count(identifier) == 0) {
        throw std::out_of_range("item to delete does not exist");
    } else {
        return this->items.erase(identifier);
    }
}
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  bool result = cObj.deleteItem("itemIdent");

// TODO Write an == operator overload for the Category class, such that two
//  Category objects are equal only if they have the same identifier and same
//  Items.
//
// Example:
//  Category cObj1{"categoryIdent1"};
//  cObj1.newItem("itemIdent");
//  Category cObj2{"categoryIdent2"};
//  if(cObj1 == cObj2) {
//    ...
//  }

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Category.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Category cObj{"categoryIdent"};
//  std::string s = cObj.str();
