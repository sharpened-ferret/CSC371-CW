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
#include <exception>
#include <unordered_map>

#include "371pass.h"
#include "lib_cxxopts.hpp"
#include "wallet.h"

// TODO Complete this function. You have been provided some skeleton code which
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

  const Action a = parseActionArgument(args);

    bool categorySelected = false;
    bool itemSelected = false;
    bool entrySelected = false;

    std::string activeCategory;
    std::string activeItem;
    std::string activeEntry;
    //TODO handle exceptions and close program with error msg.
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
            std::cerr << "Error: missing item arguments()." << std::endl;
            throw std::invalid_argument("no item provided");
        }
    } else if (args.count("item") or args.count("entry")) {
        std::cerr << "Error: missing category arguments()." << std::endl;
        throw std::invalid_argument("no category provided");
    }

  switch (a) {
  case Action::CREATE:
//    throw std::runtime_error("create not implemented");
    // TODO rework order to improve efficiency
    if (categorySelected) {
        if (itemSelected) {
            if (entrySelected) {
                Category * currCategory = &wObj.newCategory(activeCategory);
                Item * currItem  = &currCategory->newItem(activeItem);
                std::stringstream entryParams(activeEntry);
                std::string seg;
                std::vector<std::string> segList;

                while(std::getline(entryParams, seg, ',')) {
                    segList.push_back(seg);
                }
                //TODO this can error with some inputs, add error handling
                currItem->addEntry(segList[0], segList[1]);
            } else {
                Category * currCategory = &wObj.newCategory(activeCategory);
                currCategory->newItem(activeItem);
            }
        } else {
            wObj.newCategory(activeCategory);
        }
    } else {
        std::cerr << "Error: missing category, item or entry argument(s)." << std::endl;
    }
    wObj.save(db);
    break;

  case Action::READ:
    // TODO needs selectors for just reading category, item, entry, etc.
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
    throw std::runtime_error("update not implemented");
    break;

  case Action::DELETE:
    throw std::runtime_error("delete not implemented");
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

// TODO Write a function, getJSON, that returns a std::string containing the
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
  //  return "{}";
  // Only uncomment this once you have implemented the functions used!
   return wObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
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
//  return "{}";
  // Only uncomment this once you have implemented the functions used!
   auto cObj = wObj.getCategory(c);
   return cObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
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
//  return "{}";
  // Only uncomment this once you have implemented the functions used!
   auto cObj = wObj.getCategory(c);
   Item iObj = cObj.getItem(i);
   return iObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
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
//  return "{}";
  // Only uncomment this once you have implemented the functions used!
   auto cObj = wObj.getCategory(c);
   auto iObj = cObj.getItem(i);
   return iObj.getEntry(e);
}