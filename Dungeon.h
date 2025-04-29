void createDungeon();
void CreateRooms(int amountOfRooms);
void LinkRooms();
struct DungeonRooms
{
 // number of this specific room   
 int RoomNumber;   // room ID  
  // the amount of doors to other rooms  
 struct DungeonRooms* doorOne; // can be a link to an other room or null
 struct DungeonRooms* doorTwo; 
 struct DungeonRooms* doorThree; 
 struct DungeonRooms* doorFour;
 // a check for if the room was already visited
 int Isvisited; // 0 is not visited, 1 is visited
 // What is in the room (1: extra HP, 2: DMG increase, 3: OPP1 GOB, 4: OPP2 ORC, 5: Treasure , 6: nothing)
 int Treasure;

};