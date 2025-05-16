
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

    for (int i = 0; i < amountOfRooms; i++) {
    for (int d = 0; d < 4; d++) {
        kamers[i].doors[d] = NULL;
    }
    
}
    if(Randnummer == 6){
        tempTreasure++;
    }
    printf(" this is room number %i with this content %i and these room connections 1) %i 2) %i 3) %i 4) %i \n", kamers[i].RoomNumber,kamers[i].Content,kamers[i].doors[0]->RoomNumber,kamers[i].doors[1]->RoomNumber,kamers[i].doors[2]->RoomNumber,kamers[i].doors[3]->RoomNumber);


    // ConnentDoors
    for (int i = 0; i < amountOfRooms; i++)
    {
       
    }
    
}

return kamers;
}

int CountConnections(DungeonRooms* room)
 {
    int count = 0;
    for (int d = 0; d < 4; d++) {
        if (room->doors[d] != NULL) count++;
    }
    return count;
}

// link to rooms together

void LinkRooms()
{

    // go through the list to check if rooms are already connected to the room and connect them to this room aswell

 // link the rooms together with linked lists, create doors to the others rooms; 
}


// Helper function to connect two rooms bidirectionally
void ConnectRooms(DungeonRooms* a, DungeonRooms* b) {
    // Connect a to b
    for (int d = 0; d < 4; d++) {
        if (a->doors[d] == NULL) {
            a->doors[d] = b;
            break;
        }
    }
    
    // Connect b to a
    for (int d = 0; d < 4; d++) {
        if (b->doors[d] == NULL) {
            b->doors[d] = a;
            break;
        }
    }
}
// Helper function to check if two rooms are already connected
int AreRoomsConnected(DungeonRooms* a, DungeonRooms* b) {
    for (int d = 0; d < 4; d++) {
        if (a->doors[d] == b) return 1;
    }
    return 0;
}



