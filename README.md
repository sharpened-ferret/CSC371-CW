# 371pass

You should use this README to document things you did that you think are clever and could get you marks, or things you couldn't get working and perhaps why. It is fully expected that a lot of students will not complete this coursework—it is designed to be challenging. The mark scheme is also designed to be fair—marks are awarded for everything you complete, with some given for the quality of the code you write.

## Testing Tweaks
### Test 1:
Test 1 did not check case sensitivity for arguments.   
Added local testing to ensure casting to lowercase worked as expected. 

## Additional Functions
### merge()
A merge function was added to Item and Category, to facilitate merging the lists for the 'addItem' and 'addCategory' functions.  
This function takes pointers to two objects, and adds all the list entries in the second object into the list property of the first item.