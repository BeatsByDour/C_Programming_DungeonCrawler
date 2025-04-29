
// script that will keep track of the dungeon generation and manages the dungeon progess
#include <stdio.h>
#include "Dungeon.h"
#include "print_Storage.c"

void createDungeon()
{
int DungeonSize;
    DungeonSize = PrintGameStart();
  // Create A dungeon based on the size given in the argument;
switch (DungeonSize)
{
    case  1:
        // give the small dungeon size      ( 10) kamers
        CreateRooms(10);
        PrintRoomSize(10);
    break;
    case  2: 
        // give the medium dungeon size     ( 25) kamers
        CreateRooms(25);
        PrintRoomSize(25);
    break;
    case  3: 
        // give the large dungeon size      ( 50) kamers
        CreateRooms(50);
        PrintRoomSize(50);
    break;
    default:
        // default To small dungoen room
        CreateRooms(10);
        PrintRoomSize(10);
    break;
}       
}

void CreateRooms( int amountOfRooms)
{

// create the amount of rooms for the dungeon

}

void LinkRooms()
{
 // link the rooms to gether with linked lists, create doors to the others rooms; 
}






