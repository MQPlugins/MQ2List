# MQ2List
A plugin for MacroQuest 2 that wraps the C++ List type

I was motivation to contribute to the MacroQuest community given I've been using it for years on and off. I wanted to be able to use a data type such as a List in a Macro but was unable to access it. This is my best attempt (with my limited understanding) to expose that data type to macros. 

If there is a much easier way to expose popular data types, please let me know!

## Usage
### Command Line
See tests.mac for usage detailed usage examples

/clist create PlayersToBuff
-Creates a list named PlayersToBuff, if the list already exists IT WILL BE CLEARED

/clist delete PlayersToBuff
-Deletes a lsit named PlayersToBuff, if the list does NOT already exist nothing happens

/clist clear PlayersToBuff
-Clears all items from the list PlayersToBuff (Empties the list)

/client reset
-Clears all Lists from the global map


/clist pushf | pushb | popf | popb PlayersToBuff JeffTheTank
-pushf will push the item to the front of the list
-pushb will push the item to the back of the list
-popf will pop the item off the front of the list (no return value is given, see TLO for how to get the item popped)
-popb will pop the item off the back of the list (no return value is given, see TLO for how to get the item popped)
-Example: /clist pushf PlayersToBuff JeffTheTank (pushes JeffTheTank to the front of PlayersToBuff list)

/clist remove PlayersToBuff JeffTheTank
-Removes JeffTheTank from the PlayersToBuff list

### Usage - Top Level Object (TLO)
Properties:
* ${List.Active} - Is the plugin active or not? (Currently returns TRUE or NULL)
* ${MapSize} - Returns the size of the collection holding all the lists, essentially the number of Lists created
* ${ListSize[PlayersToBuff]} - Returns the Number of Elements in the List PlayersToBuff
* ${ListPushFront[PlayersToBuff,JeffTheTank]} - Returns true if successfully pushes the item to the list (front)
* ${ListPushBack[PlayersToBuff,JeffTheTank]} - Returns true if successfully pushes the item to the list (back)
* ${ListPopFront[PlayersToBuff,JeffTheTank]} - Returns the string item popped from the front of the list (NULL otherwise)
* ${ListPopBack[PlayersToBuff,JeffTheTank]} - Returns the string item popped from the back of the list (NULL otherwise)

### Some Internal Notes
The Plugin has a global Map that holds all of the Lists you create/delete/edit. Given the global nature of this variable, it will PERSIST through macros. This could be a good thing, it could be a bad thing, I don't really know. Ultimately just know this and be smart about it. You can easily control how this works though, if you want a fresh list for each time you start a macro simply call the /clist reset at the top of macros accordingly.

## TODO LIST
Below are a list of improvements I personally intend to make or would like to see others contribute. Please feel free to submit pull requests with any of your recommended updates as well and I'll be happy to include them!

* Add the ability to iterate through the list in some manner. I think this would be somewhat challenging but NOT impossible. Within the Macro itself it may look a bit hacky but I think we can make it work!
* Add the ability to retrieve items from lists based off an index
* Add the ability to list out all Lists in the global map
* Do a better job with casting/converting/handling C/C++ strings etc. If you look at the source you'll see what I mean, it's just odd going from strcpy and string and so forth. I'm new to C/C++ so be gentle!
* Review the logging/debugging output that I've put all throughout the macro. I think most of it can be removed/cleaned up or just use some basic debug variable to control output to EQ MQ2 window
* Lots more!
