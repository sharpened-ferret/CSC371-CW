# 371pass

You should use this README to document things you did that you think are clever and could get you marks, or things you couldn't get working and perhaps why. It is fully expected that a lot of students will not complete this coursework—it is designed to be challenging. The mark scheme is also designed to be fair—marks are awarded for everything you complete, with some given for the quality of the code you write.

## Testing Tweaks and Related Bugfixes
### Unit Test 1:
Unit test 1 did not check case sensitivity for arguments.   
Added local testing to ensure casting to lowercase worked as expected. 

### 371 Pass
#### Invalid entry arguments for 'create'
When using 'create' to add a new entry to an item, an untested (in unit tests) error could occur if the value provided is not a comma-seperated pair.   
I added handling for this so that the program will write an "Error: invalid entry argument(s)." to standard error and close the program with an error code.

### No-Values and Delete or Update
The specification didn't seem to specify behaviour for if update or delete actions are called without any categories, items, or entries given.  
In this case, an "Error: No update argument(s) provided." or "Error: No delete argument(s) provided." message will be written to standard error, and the program will terminate in an error state. 


## Additional Functions
### Fixed Error when no Arguments are given
There was an issue where if 371pass was called with no arguments given, it would throw a CXXopts exception from the parseActionArgument function.  
Changed so that, if no arguments are provided, it will instead print the help message.  

### merge()
A 'merge' function was added to Item and Category, to facilitate merging the lists for the 'addItem' and 'addCategory' functions.  
This function takes pointers to two objects, and adds all the list entries in the second object into the list property of the first item.  
It is defined as a friend, to allow direct access to class properties.

### to_json()
A 'to_json' function was added to Category and Item. This adds compatability to those objects for the recursive object serialisation in nlohmann::json.  
This takes a reference to a nlohmann::json object and a reference to a Category or Item object, and populates the json with the relevant data from the Category or Item. 