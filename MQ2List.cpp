#pragma warning(disable:4786) 
#include "../MQ2Plugin.h" 
#include <iostream>
#include <string>
PreSetup("MQ2List");

#define PLUGIN_MSG "\ag[MQ2List]\ax "
#define ISINDEX() (Index[0])
#define ISNUMBER() (IsNumber(Index))
#define GETNUMBER() (atoi(Index))
#define GETFIRST() Index

/*
	This is a global list, it should be noted that the contents in this list will persist through macros
	I'm open to suggestions but I've wrapped a Reset method around the global incase you want to restart
	it each time you launch a macro
*/
std::map<std::string, std::list<string>> fLists;
class MQ2ListType *pListType = 0;

CHAR Buffer[MAX_STRING] = { 0 };
BOOL dataList(PCHAR szName, MQ2TYPEVAR &Dest);

void ListCommand(PSPAWNINFO pChar, PCHAR szLine);
void ListMaint(PCHAR pListToCreate, bool pCreate);
bool ListItemRemove(PCHAR pList, PCHAR pItem);
bool ListItemRemove(PCHAR pInput);
std::list<string> *GetList(PCHAR pListName);
bool ListPush(PCHAR pInput, bool pFront);
string ListPop(PCHAR pInput, bool pFront);
bool ListHas(PCHAR pInput);

VOID ListCommand(PSPAWNINFO pChar, PCHAR szLine)
{
	std::string vCommand = std::string(szLine);
	if (vCommand.empty()) {
		WriteChatf(PLUGIN_MSG "No augs supplied");
	} else {
		GetArg(Buffer, szLine, 1);

		if (!_strnicmp(Buffer, "reset", 5)) {
			CHAR ListName[MAX_STRING];
			GetArg(ListName, szLine, 2);
			WriteChatf(PLUGIN_MSG "Resetting Map");
			fLists.clear();
		}

		//Create List && Clear List will do the same thing (create if it doesn't exist, clear it otherwise)
		if (!_strnicmp(Buffer, "create", 6) || !_strnicmp(Buffer, "clear", 5)) {
			CHAR ListName[MAX_STRING];
			GetArg(ListName, szLine, 2);
			WriteChatf(PLUGIN_MSG "(Re)Creating List: \am%s\ax", ListName);
			ListMaint(ListName, true);
		}

		//Delete List
		if (!_strnicmp(Buffer, "delete", 6)) {
			CHAR ListName[MAX_STRING];
			GetArg(ListName, szLine, 2);
			WriteChatf(PLUGIN_MSG "Deleting List:\am%s\ax", ListName);
			ListMaint(ListName, false);
		}

		//Push Item To List
		if (!_strnicmp(Buffer, "pushf", 5) || !_strnicmp(Buffer, "pushb", 5) ||
			!_strnicmp(Buffer, "popf", 5) || !_strnicmp(Buffer, "popb", 5)) {
			CHAR ListName[MAX_STRING];
			GetArg(ListName, szLine, 2);
			CHAR ItemName[MAX_STRING];
			GetArg(ItemName, szLine, 3);
			CHAR Input[MAX_STRING];
			strcpy(Input, ListName);
			strcat(Input, ",");
			strcat(Input, ItemName);
			if (!_strnicmp(Buffer, "pushf", 5))
				ListPush(Input, true);
			else if (!_strnicmp(Buffer, "pushb", 5))
				ListPush(Input, false);
			else if (!_strnicmp(Buffer, "popf", 4))
				ListPop(Input, true);
			else if (!_strnicmp(Buffer, "popb", 4))
				ListPop(Input, false);
		}

		//Remove Item From List
		if (!_strnicmp(Buffer, "remove", 6)) {
			CHAR ListName[MAX_STRING];
			GetArg(ListName, szLine, 2);

			CHAR ItemName[MAX_STRING];
			GetArg(ItemName, szLine, 3);

			WriteChatf(PLUGIN_MSG "Remove Item \am%s\ax to List:\am%s\ax", ItemName, ListName);
			ListItemRemove(ListName, ItemName);
		}
	}
	return;
}

/*
	List create/delete a list in our global map
	-Sanitize the input, make sure we always use lowercase 
	-If the List already exists, delete it and create a brand new one
	-TODO: consider throwing error instead of deleting existing list?
*/
void ListMaint(PCHAR pListToCreate, bool pCreate) {
	std::string vListName = std::string(pListToCreate);
	std::transform(vListName.begin(), vListName.end(), vListName.begin(), ::tolower);

	std::map<std::string, std::list<string>>::iterator it;
	it = fLists.find(vListName);
	if (it != fLists.end())
		fLists.erase(it);

	if (pCreate)
		fLists.insert(std::pair<std::string, std::list<std::string>>(vListName, std::list<std::string>()));
}

bool ListItemRemove(PCHAR pInput) {
	char vListName[MAX_STRING];
	char vListValue[MAX_STRING];
	GetArg(vListName, pInput, 1, FALSE, FALSE, FALSE, ',');
	GetArg(vListValue, pInput, 2, FALSE, FALSE, FALSE, ',');
	return ListItemRemove(vListName, vListValue);
}

bool ListItemRemove(PCHAR pList, PCHAR pItem) {
	std::string vListName = std::string(pList);
	std::transform(vListName.begin(), vListName.end(), vListName.begin(), ::tolower);

	std::map<std::string, std::list<string>>::iterator it;
	it = fLists.find(vListName);
	if (it != fLists.end()) {
		it->second.remove(pItem);
		return true;
	}
	return false;
}

bool ListPush(PCHAR pInput, bool pFront) {
	char vListName[MAX_STRING];
	char vListValue[MAX_STRING];
	GetArg(vListName, pInput, 1, FALSE, FALSE, FALSE, ',');
	GetArg(vListValue, pInput, 2, FALSE, FALSE, FALSE, ',');
	WriteChatf(PLUGIN_MSG "Add Item \am%s\ax to List:\am%s\ax", vListValue, vListName);
	std::list<string> *vList = GetList(vListName);
	if (vList) {
		if (pFront)
			vList->push_front(vListValue);
		else
			vList->push_back(vListValue);
		return true;
	}
	return false;
}

bool ListHas(PCHAR pInput) {
	char vListName[MAX_STRING];
	char vListValue[MAX_STRING];
	GetArg(vListName, pInput, 1, FALSE, FALSE, FALSE, ',');
	GetArg(vListValue, pInput, 2, FALSE, FALSE, FALSE, ',');

	std::list<string> *vList = GetList(vListName);
	if (vList) {
		return (std::find(vList->begin(), vList->end(), vListValue) != vList->end());
	}
	return false;
}

string ListPop(PCHAR pInput, bool pFront) {
	char vListName[MAX_STRING];
	string vReturn = "NULL";
	GetArg(vListName, pInput, 1, FALSE, FALSE, FALSE, ',');

	std::list<string> *vList = GetList(vListName);
	if (vList) {
		if (pFront) {
			vReturn = vList->front();
			vList->pop_front();
		}
		else {
			vReturn = vList->back();
			vList->pop_back();
		}
	}
	return vReturn;
}

std::list<string> *GetList(PCHAR pListName) {
	std::string vListName = std::string(pListName);
	std::transform(vListName.begin(), vListName.end(), vListName.begin(), ::tolower);

	std::map<std::string, std::list<string>>::iterator it;
	it = fLists.find(vListName);
	if (it != fLists.end()) {
		return &it->second;
	}
	return nullptr;
}


class MQ2ListType : public MQ2Type {
private:
	char Temps[MAX_STRING];
public:
	enum ListMembers {
		Active = 1,
		MapSize = 2,
		ListSize = 3,
		ListPushFront = 4,
		ListPushBack = 5,
		ListPopFront = 6,
		ListPopBack = 7,
		ListContains = 8,
		ListRemove = 9
	};
	MQ2ListType() :MQ2Type("List") {
		TypeMember(Active);
		TypeMember(MapSize);
		TypeMember(ListSize);
		TypeMember(ListPushFront);
		TypeMember(ListPushBack);
		TypeMember(ListPopFront);
		TypeMember(ListPopBack);
		TypeMember(ListContains);
		TypeMember(ListRemove);
	}
	bool MQ2ListType::GETMEMBER() {
		PMQ2TYPEMEMBER pMember = MQ2ListType::FindMember(Member);
		//Default everything to NULL unless otherwise specified!
		strcpy_s(DataTypeTemp, "NULL");
		Dest.Type = pStringType;
		Dest.Ptr = &DataTypeTemp[0];
		if (pMember) {
			switch ((ListMembers)pMember->ID) {
				case Active:
					Dest.DWord = (gbInZone && GetCharInfo() && GetCharInfo()->pSpawn);
					Dest.Type = pBoolType;
					return true;
				case MapSize:
					Dest.DWord = fLists.size();
					Dest.Type = pIntType;
					return true;
				case ListSize:
				{
					std::list<string> *vList = GetList(Index);
					if (vList) {
						Dest.DWord = vList->size();
						Dest.Type = pIntType;
					}
					return true;
				}
				case ListPushFront:
				{
					Dest.DWord = ListPush(Index, true);
					Dest.Type = pBoolType;
					return true;
				}
				case ListPushBack:
				{
					Dest.DWord = ListPush(Index, false);
					Dest.Type = pBoolType;
					return true;
				}
				case ListPopFront:
				{
					std::string vValue = ListPop(Index, true);
					strcpy_s(Temps, vValue.c_str());
					Dest.Ptr = Temps;
					Dest.Type = pStringType;
					return true;
				}
				case ListPopBack:
				{
					std::string vValue = ListPop(Index, false);
					strcpy_s(Temps, vValue.c_str());
					Dest.Ptr = Temps;
					Dest.Type = pStringType;
					return true;
				}
				case ListContains:
				{
					Dest.DWord = ListHas(Index);
					Dest.Type = pBoolType;
					return true;
				}
				case ListRemove:
				{
					Dest.DWord = ListItemRemove(Index);
					Dest.Type = pBoolType;
					return true;
				}
			}
		}
		return true;
	}
	bool ToString(MQ2VARPTR VarPtr, PCHAR Destination) {
		strcpy_s(Destination, MAX_STRING, "TRUE");
		return true;
	}
	bool FromData(MQ2VARPTR &VarPtr, MQ2TYPEVAR &Source) {
		return false;
	}
	bool FromString(MQ2VARPTR &VarPtr, PCHAR Source) {
		return false;
	}
	~MQ2ListType() { }
};

BOOL dataList(PCHAR szName, MQ2TYPEVAR &Dest) {
	Dest.DWord = 1;
	Dest.Type = pListType;
	return true;
}

PLUGIN_API VOID InitializePlugin(VOID)
{
	DebugSpewAlways("Initializing MQ2List");
	pListType = new MQ2ListType;
	AddMQ2Data("List", dataList);
	AddCommand("/clist", ListCommand);
}

PLUGIN_API VOID ShutdownPlugin(VOID)
{
	DebugSpewAlways("Shutting down MQ2List");
	delete pListType;
	RemoveMQ2Data("List");
	RemoveCommand("/clist");
}