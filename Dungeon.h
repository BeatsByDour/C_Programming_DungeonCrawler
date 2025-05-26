#ifndef DUNGEON_H
#define DUNGEON_H

// struct definitions

#pragma region structs

typedef struct DungeonRooms
{
 // number of this specific room   
 int RoomNumber;   // room ID  
  // the amount of doors to other rooms  
struct DungeonRooms* doors[4];
 // a check for if the room was already visited
 int Isvisited; // 0 is not visited, 1 is visited
 // What is in the room (1: extra HP, 2: DMG increase, 3: OPP1 GOB, 4: OPP2 ORC, 5: nothing , 6: TREASURE)
 int Content;

}DungeonRooms;

// struct for the player stats 
typedef struct Player{
  //player hp stats
    int maxHp,currentHp;
    // player dmg stats
    int damageValue;
    // the room the players currently resides in
    struct DungeonRooms* CurrentRoom;
    }Player;

    #pragma endregion

// function definitions

#pragma region DunguenGen

/// @brief Function that lets the player go to the next room
/// @param RoomToGo Room the player chose to go too
/// @param currentRoom the the player currently resides in
/// @param adventurer  the player struct that will be used for the encounter
int EnterNewRoom(int RoomToGo,DungeonRooms* currentRoom,Player* adventurer );
/// @brief This fucntion initializes the player and give the player values
/// @return it returns a player struct that is filled with correct values
Player InitilizePlayer();
/// @brief function that is used for the player to tell how big the dungeon will be 
/// @return it returns an array of rooms that are in the dungeon
DungeonRooms* CreateDungeon();
/// @brief  this function creates the dungeon with rooms in it and links all rooms together
/// @param amountOfRooms the amount of rooms that the player has chosen 
/// @return  it returns an array of rooms that are in the dungeon
DungeonRooms* CreateRooms(int amountOfRooms);
/// @brief This function checks if the rooms are already connected
/// @param a  The first room 
/// @param b  the second room
/// @return  the is will return a 1 if the rooms are connected and a 0 if they aren't
int AreRoomsConnected(DungeonRooms* a, DungeonRooms* b);
/// @brief This function connects two rooms together 
/// @param a the first room that will be connected
/// @param b the second room that will be connected
void ConnectRooms(DungeonRooms* a, DungeonRooms* b);
/// @brief Function to check the amount of rooms that are connected to the given room
/// @param room room that will be checked
/// @return integer ranging from 0 to 4 
int CountConnections(DungeonRooms* room);
// struct creation

#pragma endregion

#pragma region  encounter 
/// @brief  This function checks the room for the content it contains and checks if the room is already visited
/// @param contentID this is variable checks the content of the room  
/// @param Adventurer the player struct that will be used for the combat and buff encounters
/// @return This function returns 0,1 or 2 depending if the player lives or not 
int Encounter(int contentID, Player* Adventurer);
/// @brief This function is used when you encounter an opponent in the rooms
/// @param enemyID  The id of the enemy, 1 for goblin ,2 for Orc
/// @param Adventurer The player struct that will be used for the attacking and hp stats
/// @return  his function returns 0,1 or 2 depending if the player lives or not 
int FightEncounter(int enemyID, Player* Adventurer);
/// @brief This function is used to give the player a buff 
/// @param BuffId  This checks which buff the player will get, 1 for healing , 2 for more dammage
/// @param Adventurer The player struct that will be used for the attacking and hp stats
void BuffEncounter(int BuffId, Player* Adventurer);
/// @brief  This fucntion is used to fight the monster in the room 
/// @param hp The hp of the monster
/// @param dmg  The dmg of the monster
/// @param oppName  The name of the monster
/// @param Adventurer The player struct that will be used for the attacking and hp stats
/// @return  his function returns 0,1 or 2 depending if the player lives or not 
int FightOppenent(int hp,int dmg, char oppName[10], Player* Adventurer);
/// @brief This function is used for when the player finds the treasure
/// @return this function will return a 1
int VictoryCondition();

#pragma endregion

#endif 