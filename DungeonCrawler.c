
// script that will keep track of the dungeon generation and manages the dungeon progess
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dungeon.h"
#include "print_Storage.c"

DungeonRooms* CreateDungeon()
{
int DungeonSize;
    DungeonSize = PrintGameStart();
  // Create A dungeon based on the size given in the argument;
switch (DungeonSize)
{
    case  1:
        // give the small dungeon size      ( 10) kamers
        return CreateRooms(10);
        PrintRoomSize(10);
    break;
    case  2: 
        // give the medium dungeon size     ( 25) kamers
        return CreateRooms(25);
        PrintRoomSize(25);
    break;
    case  3: 
        // give the large dungeon size      ( 50) kamers
        return CreateRooms(50);
        PrintRoomSize(50);
    break;
    default:
        // default To small dungoen room
        return CreateRooms(10);
        PrintRoomSize(10);
    break;
}       
}

Player InitilizePlayer()
{
 Player InitPlayer;

 InitPlayer.maxHp = 30;
 InitPlayer.currentHp = 30;
 InitPlayer.damageValue = 5;
 InitPlayer.CurrentRoom = calloc(1,sizeof(DungeonRooms));
 
 return InitPlayer;
    
}

DungeonRooms*  CreateRooms( int amountOfRooms)
{
    srand(time(NULL));
DungeonRooms* kamers = (DungeonRooms *)calloc(amountOfRooms , amountOfRooms * sizeof(DungeonRooms));
int tempTreasure = 0;
for (int i = 0; i < amountOfRooms; i++)
{
  
   int  Randnummer = 0;
   if(tempTreasure == 0&& i == amountOfRooms)
   {
    Randnummer = 6;
   }else if(tempTreasure == 0 )
   {
    Randnummer = rand() % 7;
   }
   else {
    Randnummer = rand() % 6;
   }
    kamers[i].RoomNumber = i+1;
    kamers[i].Content = Randnummer; // number between 1 and 6
    kamers[i].doorOne = calloc(1,sizeof(DungeonRooms));
    kamers[i].doorTwo = calloc(1,sizeof(DungeonRooms));
    kamers[i].doorThree = calloc(1,sizeof(DungeonRooms));
    kamers[i].doorFour = calloc(1,sizeof(DungeonRooms));
    if(Randnummer == 6){
        tempTreasure++;
    }
    printf(" this is room number %i with this content %i and these room connections 1) %i 2) %i 3) %i 4) %i \n", kamers[i].RoomNumber,kamers[i].Content,kamers[i].doorOne->RoomNumber,kamers[i].doorTwo->RoomNumber,kamers[i].doorThree->RoomNumber,kamers[i].doorFour->RoomNumber);

}

return kamers;
}

  
    

// create the amount of rooms for the dungeon






void LinkRooms()
{

    // go through the list to check if rooms are already connected to the room and connect them to this room aswell

 // link the rooms together with linked lists, create doors to the others rooms; 
}






