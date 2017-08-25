# MQ2List
A plugin for MacroQuest 2 that wraps the C++ List type

## Usage
### Command Line
See tests.mac for usage detailed usage examples

/clist create PlayersToBuff
-Creates a list named PlayersToBuff

/clist delete PlayersToBuff
-Deletes a lsit named PlayersToBuff (if it exists, does nothing if it does not exist)

/clist clear PlayersToBuff
-Clears all items from the list PlayersToBuff (Empties the list)

/clist pushf|pushb|popf|popb PlayersToBuff JeffTheTank
-pushf will push the item to the front of the list
-pushb will push the item to the back of the list
-popf will pop the item off the front of the list (no return value is given, see TLO for how to get the item popped)
-popb will pop the item off the back of the list (no return value is given, see TLO for how to get the item popped)

/clist remove PlayersToBuff JeffTheTank
-Removes JeffTheTank from the PlayersToBuff list

### Usage - Top Level Object (TLO)
Properties:
* Active - Is the plugin active or not? (Currently returns TRUE or NULL)
* MapSize - Returns the size of the collection holding all the lists, essentially the number of Lists created
* ListSize[PlayersToBuff] - Returns the Number of Elements in the List PlayersToBuff
* ListPushFront[PlayersToBuff,JeffTheTank] - Returns true if successfully pushes the item to the list (front)
* ListPushBack[PlayersToBuff,JeffTheTank] - Returns true if successfully pushes the item to the list (back)
* ListPopFront[PlayersToBuff,JeffTheTank] - Returns the string item popped from the front of the list (NULL otherwise)
* ListPopBack[PlayersToBuff,JeffTheTank] - Returns the string item popped from the back of the list (NULL otherwise)
