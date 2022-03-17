// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851182
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>

#include "371pass.h"
#include "lib_cxxopts.hpp"
#include "wallet.h"

//  Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a Wallet object
//  2. Parse the 'action' argument to decide what action should be taken (
//     create, read, update, or delete). Read is the easiest to implement, and
//     update is the hardest. The details of how these arguments work is in the
//     coursework specification.
//  4. Save the updated Wallet object to the JSON file if there have been
//     changes (calling save() on the Wallet object).
//
// Some commented out code has been provided. Using some of this will be
// demonstrated in the coursework video on Canvas. Remember, this coursework is
// meant to be challenging and testing your understanding of programming in C++.
// Part of the challenge is figuring things out on your own. That is a major
// part of software development.
//
// Example:
//  int main(int argc, char *argv[]) { return App::run(argc, argv); }
int App::run(int argc, char *argv[]) {
    auto options = App::cxxoptsSetup();
    auto args = options.parse(argc, argv);

    // Print the help usage if requested
    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }

    // Open the database and construct the Wallet
    const std::string db = args["db"].as<std::string>();
    Wallet wObj{};
    wObj.load(db);


    Action a;
    try {
        a = parseActionArgument(args);
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error: invalid action argument(s)." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    bool categorySelected = false;
    bool itemSelected = false;
    bool entrySelected = false;

    std::string activeCategory;
    std::string activeItem;
    std::string activeEntry;
    if (args.count("category")) {
        categorySelected = true;
        activeCategory = args["category"].as<std::string>();
        if (args.count("item")) {
            itemSelected = true;
            activeItem = args["item"].as<std::string>();
            if (args.count("entry")) {
                entrySelected = true;
                activeEntry = args["entry"].as<std::string>();
            }
        } else if (args.count("entry")) {
            std::cerr << "Error: missing item argument(s)." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (args.count("item") or args.count("entry")) {
        std::cerr << "Error: missing category argument(s)." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    switch (a) {
        case Action::CREATE:
            if (categorySelected) {
                if (itemSelected) {
                    if (entrySelected) {
                        Category *currCategory = &wObj.newCategory(activeCategory);
                        Item *currItem = &currCategory->newItem(activeItem);
                        std::stringstream entryParams(activeEntry);
                        std::string seg;
                        std::vector<std::string> segList;

                        while (std::getline(entryParams, seg, ',')) {
                            segList.push_back(seg);
                        }
                        try {
                            currItem->addEntry(segList[0], segList[1]);
                        } catch (const std::exception &ex) {
                            std::cerr << "Error: invalid entry argument(s)." << std::endl;
                            std::exit(EXIT_FAILURE);
                        }
                    } else {
                        Category *currCategory = &wObj.newCategory(activeCategory);
                        currCategory->newItem(activeItem);
                    }
                } else {
                    wObj.newCategory(activeCategory);
                }
            } else {
                std::cerr << "Error: missing category, item or entry argument(s)." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            wObj.save(db);
            break;

        case Action::READ:
            if (categorySelected) {
                if (itemSelected) {
                    if (entrySelected) {
                        std::cout << App::getJSON(wObj, activeCategory, activeItem, activeEntry) << std::endl;
                    } else {
                        std::cout << App::getJSON(wObj, activeCategory, activeItem) << std::endl;
                    }
                } else {
                    std::cout << App::getJSON(wObj, activeCategory) << std::endl;
                }
            } else {
                std::cout << App::getJSON(wObj) << std::endl;
            }
            break;

        case Action::UPDATE:
            if (categorySelected) {
                try {
                    if (itemSelected) {
                        Category *currCategory = &wObj.getCategory(activeCategory);
                        try {
                            if (entrySelected) {
                                Item *currItem = &currCategory->getItem(activeItem);
                                std::stringstream entryParams(activeEntry);
                                std::string seg;
                                std::vector<std::string> segList;
                                while (std::getline(entryParams, seg, ',')) {
                                    segList.push_back(seg);
                                }
                                try {
                                    currItem->deleteEntry(segList[0]);
                                    currItem->addEntry(segList[0], segList[1]);
                                } catch (const std::exception &ex) {
                                    std::cerr << "Error: invalid entry argument(s)." << std::endl;
                                    std::exit(EXIT_FAILURE);
                                }
                            } else {
                                // Splits old and new item idents from input args
                                std::stringstream itemParams(activeItem);
                                std::string seg;
                                std::vector<std::string> segList;
                                while (std::getline(itemParams, seg, ':')) {
                                    segList.push_back(seg);
                                }
                                // Creates a pointer to the item, deletes it from the category, changes the item ident,
                                // then adds it back to the category
                                Item *currItem = &currCategory->getItem(segList[0]);
                                currCategory->deleteItem(segList[0]);
                                currItem->setIdent(segList[1]);
                                currCategory->addItem(*currItem);
                            }
                        } catch (const std::out_of_range &ex) {
                            std::cerr << "Error: invalid item argument(s)." << std::endl;
                            std::exit(EXIT_FAILURE);
                        }
                    } else {
                        // Splits old and new category idents from input args
                        std::stringstream categoryParams(activeItem);
                        std::string seg;
                        std::vector<std::string> segList;
                        while (std::getline(categoryParams, seg, ':')) {
                            segList.push_back(seg);
                        }
                        // Creates a pointer to the category, deletes it from the wallet, changes the category ident,
                        // then adds it back to the wallet
                        Category *currCategory = &wObj.getCategory(segList[0]);
                        wObj.deleteCategory(segList[0]);
                        currCategory->setIdent(segList[1]);
                        wObj.addCategory(*currCategory);
                    }
                } catch (const std::out_of_range &ex) {
                    std::cerr << "Error: invalid category argument(s)." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error: No update argument(s) provided." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            wObj.save(db);
            break;

        case Action::DELETE:
            if (categorySelected) {
                try {
                    if (itemSelected) {
                        Category *currCategory = &wObj.getCategory(activeCategory);
                        if (entrySelected) {
                            Item *currItem = &currCategory->getItem(activeItem);
                            currItem->deleteEntry(activeEntry);
                        } else {
                            currCategory->deleteItem(activeItem);
                        }
                    } else {
                        wObj.deleteCategory(activeCategory);
                    }
                } catch (const std::out_of_range &ex) {
                    std::cerr << "Error: invalid " << ex.what() << " argument(s)." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error: No delete argument(s) provided." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            wObj.save(db);
            break;

        default:
            throw std::runtime_error("Unknown action not implemented");
    }

    return 0;
}

// Create a cxxopts instance. You do not need to modify this function.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
cxxopts::Options App::cxxoptsSetup() {
    cxxopts::Options cxxopts("371pass", "Student ID: " + STUDENT_NUMBER + "\n");

    cxxopts.add_options()(
            "db", "Filename of the 371pass database",
            cxxopts::value<std::string>()->default_value("database.json"))(

            "action", "Action to take, can be: 'create', 'read', 'update', 'delete'.",
            cxxopts::value<std::string>())(

            "category",
            "Apply action to a category (e.g., if you want to add a category, set the"
            " action argument to 'add' and the category argument to your chosen"
            " category identifier).",
            cxxopts::value<std::string>())(

            "item",
            "Apply action to an item (e.g., if you want to add an item, set the "
            "action argument to 'add', the category argument to your chosen category "
            "identifier and the item argument to the item identifier).",
            cxxopts::value<std::string>())(

            "entry",
            "Apply action to an entry (e.g., if you want to add an entry, set the "
            "action argument to 'add', the category argument to your chosen category "
            "identifier, the item argument to your chosen item identifier, and the "
            "entry argument to the string 'key,value'). If there is no comma, an "
            "empty entry is inserted. If you are simply retrieving an entry, set the "
            "entry argument to the 'key'. If you are updating an entry key, use a : "
            "e.g., oldkey:newkey,newvalue.",
            cxxopts::value<std::string>())(

            "h,help", "Print usage.");

    return cxxopts;
}

// Rewrite this function so that it works. This function should
//  case-insensitively check the action argument retrieved from cxxopts and
//  convert this to a value from the ACTION enum. If an invalid value is given
//  in a string, throw an std::invalid_argument exception.
//
// TODO we will get a cxxopts no value exception if the action arg is not included.
//   may want to look into correcting this in cxx handling somewhere
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
//  App::Action action = parseActionArgument(args);
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
    std::string input = args["action"].as<std::string>();
    // Casts string to lowercase and compares the value
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input == "create") {
        return Action::CREATE;
    } else if (input == "read") {
        return Action::READ;
    } else if (input == "update") {
        return Action::UPDATE;
    } else if (input == "delete") {
        return Action::DELETE;
    }
    throw std::invalid_argument("action");
}

//  Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::cout << getJSON(wObj);
std::string App::getJSON(Wallet &wObj) {
    return wObj.str();
}

//  Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Category in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::cout << getJSON(wObj, c);
std::string App::getJSON(Wallet &wObj, const std::string &c) {
    try {
        auto cObj = wObj.getCategory(c);
        return cObj.str();
    } catch (const std::out_of_range &ex) {
        std::cerr << "Error: invalid " << ex.what() << " argument(s)." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//  Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Item in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::string i = "item argument value";
//  std::cout << getJSON(wObj, c, i);
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i) {
    try {
        auto cObj = wObj.getCategory(c);
        Item iObj = cObj.getItem(i);
        return iObj.str();
    } catch (const std::out_of_range &ex) {
        std::cerr << "Error: invalid " << ex.what() << " argument(s)." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//  Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Entry in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::string i = "item argument value";
//  std::string e = "entry argument value";
//  std::cout << getJSON(wObj, c, i, e);
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i, const std::string &e) {
    try {
        auto cObj = wObj.getCategory(c);
        auto iObj = cObj.getItem(i);
        return iObj.getEntry(e);
    } catch (const std::out_of_range &ex) {
        std::cerr << "Error: invalid " << ex.what() << " argument(s)." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}