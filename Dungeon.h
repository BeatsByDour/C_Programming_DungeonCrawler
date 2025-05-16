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

// add payer struct 
typedef struct Player{
    int maxHp,currentHp;
    int damageValue;
    struct DungeonRooms* CurrentRoom;
    }Player;

// function definitions


/// @brief 
/// @param RoomToGo 
/// @param DungeonRooms 
/// @param adventurer 
void EnterNewRoom(int RoomToGo,DungeonRooms* DungeonRooms,Player* adventurer );
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
void LinkRooms();
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


