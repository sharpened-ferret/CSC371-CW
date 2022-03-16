// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "wallet.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "lib_json.hpp"

//  Write a Wallet constructor that takes no parameters and constructs an
//  empty wallet.
Wallet::Wallet() = default;
// Example:
//  Wallet wObj{};

//  Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of categories in the Wallet contains.
unsigned int Wallet::size() {
    return this->categories.size();
}
// Example:
//  Wallet wObj{};
//  auto size = wObj.size();

//  Write a function, empty, that takes no parameters and returns true
//  if the number of categories in the Wallet is zero, false otherwise.
bool Wallet::empty() {
    return this->categories.empty();
}
// Example:
//  Wallet wwObj{};
//  auto isEmpty = wObj.empty();

//  Write a function, newCategory, that takes one parameter, a category
//  identifier, and returns the Category object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Category object cannot be
//  inserted into the container.
Category& Wallet::newCategory(std::string identifier) {
    if (this->categories.count(identifier) == 0) {
        const auto status = this->categories.insert({identifier, Category(identifier)});
        if (status.second) {
            return this->categories.at(identifier);
        } else {
            throw std::runtime_error("new category failed");
        }
    } else {
        return this->categories.at(identifier);
    }
}
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");

//  Write a function, addCategory, that takes one parameter, a Category
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged and then return false. Throw a std::runtime_error if the Category
//  object cannot be inserted into the container for whatever reason.
bool Wallet::addCategory(Category category) {
    if (this->categories.count(category.getIdent()) == 0) {
        const auto status = this->categories.insert({category.getIdent(), category});
        return status.second;
    } else {
        // Gets the conflicting item and calls to a helper function in item to merge the Item entries
        Category existingCopy = this->categories.at(category.getIdent());
        merge(existingCopy, category);
        return false;
    }
}
// Example:
//  Wallet wObj{};
//  Category cObj{"categoryIdent"};
//  wObj.addCategory(cObj);

//  Write a function, getCategory, that takes one parameter, a Category
//  identifier and returns the Category. If no Category exists, throw an
//  appropriate exception.
Category& Wallet::getCategory(std::string ident) {
    if (this->categories.count(ident) == 0) {
        throw std::out_of_range("no Category exists with identifier");
    } else {
        return this->categories.at(ident);
    }
}
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
//  auto cObj = wObj.getCategory("categoryIdent");

//  Write a function, deleteCategory, that takes one parameter, a Category
//  identifier, and deletes it from the container, and returns true if the
//  Category was deleted. If no Category exists, throw an appropriate exception.
bool Wallet::deleteCategory(std::string ident) {
    if (this->categories.count(ident) == 0) {
        throw std::out_of_range("no Category exists with identifier");
    } else {
        return this->categories.erase(ident);
    }
}
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
//  wObj.deleteCategory("categoryIdent");

//  Write a function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this Wallet. If the file does open throw an
//  appropriate exception (either std::runtime_error or a derived class).
void Wallet::load(std::string filename) {
    //TODO clean this code + add exception throwing
    std::string line;
    std::stringstream buffer;
    std::ifstream database(filename);

    while(std::getline(database, line)) {
        buffer << line;
    }
    database.close();
    std::string fileText = buffer.str();
    nlohmann::json jsonObject = nlohmann::json::parse(fileText);

    for (auto curr = jsonObject.begin(); curr != jsonObject.end(); ++curr) {
        this->addCategory(Category(curr.key()));
        Category * currCategory = &this->getCategory(curr.key());
        auto items = curr.value();
        for (auto item = items.begin(); item != items.end(); ++item) {
            currCategory->addItem(item.key());
            Item * currItem = &currCategory->getItem(item.key());
            auto entries = item.value();
            for (auto entry = entries.begin(); entry != entries.end(); ++entry) {
                std::string entryValue = entry.value();
                currItem->addEntry(entry.key(), entryValue);
            }
        }
    }
}
// A note on clashes:
//  If you encounter two categories with the same key, the categories should be
//  merged (not replaced!). If you encounter two items with the same key in the
//  same category, the items should be merged (not replaced!). If you encounter
//  two entries with the same key in the same item, the entries should be merged
//  (undefined as to which value is picked). Two items in different categories
//  can have the same key, as can two entries in different items.
//
// JSON formatting:
//  The JSON file has the following format (see the sample database.json file
//  also provided with the coursework framework):
//    {
//      "Category 1" : {
//        "Item 1":  {
//          "detail 1 key": "detail 1 value",
//          "detail 2 key": "detail 2 value",
//          ...
//        },
//        "Item 2":  {
//          "detail 1 key": "detail 1 value",
//          "detail 2 key": "detail 2 value"
//        },
//        ...
//      },
//      "Category 2": {
//        "Item 1": {
//          "detail 1 key": "detail 1 value"
//        }
//        ...
//      }
//    }
//
// More help:
//  To help you with this function, I've selected the nlohmann::json
//  library that you must use for your coursework. Read up on how to use it
//  here: https://github.com/nlohmann/json. You may not use any other external
//  library other than the one I have provided. You may choose to process the
//  JSON yourself without the help of the library but I guarantee this will be
//  more work.
//
//  Understanding how to use external libraries is part of this coursework! You
//  will need to use this file to deserialize the JSON from string
//  to the JSON object provided by this library. Don't just look at the code
//  provided below, or in the README on the GitHub and despair. Google search,
//  look around, try code out in a separate file to all figure out how to use
//  this library.
//
//  Once you have deserialized the JSON string into an object, you will need to
//  loop through this object, constructing Category and Item objects according
//  to the JSON data in the file.
//
// Example:
//  Wallet wObj{};
//  wObj.load("database.json");

// TODO Write a function ,save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the Wallet object
//  as JSON.
void Wallet::save(std::string filename) {
    nlohmann::json jSave(categories);
    std::ofstream out(filename);
    out << std::setw(4) << jSave << std::endl;
}
// Example:
//  Wallet wObj{};
//  wObj.load("database.json");
//  ...
//  wObj.save("database.json");

// TODO Write an == operator overload for the Wallet class, such that two
//  Wallet objects are equal only if they have the exact same data.
bool operator==(Wallet& lhs, Wallet& rhs) {
    if (lhs.categories == rhs.categories) {
        return true;
    } else {
        return false;
    }
}
// Example:
//  Wallet wObj1{};
//  Wallet wObj2{};
//  if(wObj1 == wObj2) {
//    ...
//  }

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Wallet.
//
// Hint:
//  See the coursework specification for how this JSON should look.
//
// Example:
//  Wallet wObj{};
//  std::string s = wObj.str();
