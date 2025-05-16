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

// function definitions

DungeonRooms* CreateDungeon();
DungeonRooms* CreateRooms(int amountOfRooms);
void LinkRooms();
/// @brief Function to check the amount of rooms that are connected to the given room
/// @param room room that will be checked
/// @return integer ranging from 0 to 4 
int CountConnections(DungeonRooms* room);
// struct creation

// add payer struct 
typedef struct Player{
    int maxHp,currentHp;
    int damageValue;
    struct DungeonRooms* CurrentRoom;
    }Player;
