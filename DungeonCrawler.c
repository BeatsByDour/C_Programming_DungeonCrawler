
// script that will keep track of the dungeon generation and manages the dungeon progess
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dungeon.h"
#include "print_Storage.c"
#include "Encounter.c"

DungeonRooms* CreateDungeon(int DungeonSize)
{

  
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

// initialized the player char
Player InitilizePlayer()
{
 Player InitPlayer;

 InitPlayer.maxHp = 30;
 InitPlayer.currentHp = 30;
 InitPlayer.damageValue = 5;
 InitPlayer.CurrentRoom = (DungeonRooms*)calloc(1,sizeof(DungeonRooms));
 
 return InitPlayer;
    
}

int EnterNewRoom(int RoomToGo,DungeonRooms* currentRoom,Player* adventurer )
{
    printf(" selected room is %i \n", RoomToGo);
    RoomToGo--;
    // Check if doorIndex is valid (1-4)
    if (RoomToGo < 0 || RoomToGo >= 4) {
        printf("Invalid door selection! Choose between 1 and 4.\n");
        return 0;
    }

    // Check if the door exists (not NULL)
    if (currentRoom->doors[RoomToGo] == NULL) {
        printf("There is no door in that direction!\n");
        return 0;
    }

    // Update player's current room
    adventurer->CurrentRoom = currentRoom->doors[RoomToGo];
    printf("You entered Room %d.\n", adventurer->CurrentRoom->RoomNumber);

    if(adventurer->CurrentRoom->Isvisited == 0)
    {
        printf("You are entering room %i, with this content %i , What are you gonna find ???\n",adventurer->CurrentRoom->RoomNumber,adventurer->CurrentRoom->Content);
        return Encounter(adventurer->CurrentRoom->Content,adventurer);
    }else
    {
        printf("You have already been to this room Womp Womp, lets go to the next\n");
        return 0;
    }
    // check if you visited this room 
    // if 0 Go to encounter if 1 nothing happens.
}

#pragma region Dungeon Generation

DungeonRooms* CreateRooms(int amountOfRooms) {
    srand(time(NULL));
    DungeonRooms* kamers = (DungeonRooms*)calloc(amountOfRooms, sizeof(DungeonRooms));
    int tempTreasure = 0;

    // Initialize rooms
    for (int i = 0; i < amountOfRooms; i++) {
        int Randnummer = 0;
        if (tempTreasure == 0 && i == amountOfRooms - 1) {
            Randnummer = 6; // Force treasure in last room if none yet
        } else if (tempTreasure == 0) {
            Randnummer = 1 + rand() % 6; // 1-6 (6 = treasure)
        } else {
            Randnummer = 1 + rand() % 5; // 1-5 (no treasure)
        }

        if(i == 0){
            Randnummer = 5;
        }
        kamers[i].RoomNumber = i + 1;

        kamers[i].Content = Randnummer;
        
        // Initialize all doors to NULL
        for (int d = 0; d < 4; d++) {
            kamers[i].doors[d] = NULL;
        }

        if (Randnummer == 6) tempTreasure++;
    }

   // Connect doors (with variable connections per room)
    for (int i = 0; i < amountOfRooms; i++) {
        // Randomly decide how many connections this room should have (1-4)
        int targetConnections = 2 + rand() % 3; // Random between 2 and 4

        // Only connect if current connections < target
        while (CountConnections(&kamers[i]) < targetConnections) {
            int Failsafe = 0;
            int SelectedRoom = rand() % amountOfRooms;

            // Avoid invalid connections:
            // - Selected room is full (>=4 connections)
            // - Selected room is itself
            // - Already connected
            while (CountConnections(&kamers[SelectedRoom]) >= 4 || 
                   &kamers[SelectedRoom] == &kamers[i] || 
                   AreRoomsConnected(&kamers[SelectedRoom], &kamers[i])) {
                SelectedRoom = rand() % amountOfRooms;
                Failsafe++;
                if (Failsafe > 20) break; // Prevent infinite loops
            }

            if (Failsafe <= 20) { // Only connect if a valid room was found
                ConnectRooms(&kamers[i], &kamers[SelectedRoom]);
            } else {
                break; // No valid connections left
            }
        }
    

      
    }

    for (size_t j = 0; j < amountOfRooms; j++)
    {
        // Safe print (check for NULL doors)
        printf("Room %i, Content: %i, Connections: ", kamers[j].RoomNumber, kamers[j].Content);
        for (int d = 0; d < 4; d++) {
            if (kamers[j].doors[d] != NULL) {
                printf("%d) %i ", d+1, kamers[j].doors[d]->RoomNumber);
            } else {
                printf("%d) NULL ", d+1);
            }
        }
        printf("\n");
    }
    
    return kamers;
}

// count the amount of rooms that are connected to this room
int CountConnections(DungeonRooms* room)
 {
    int count = 0;
    for (int d = 0; d < 4; d++) {
        if (room->doors[d] != NULL) {count++;}
    }
    return count;
}

// Helper function to connect two rooms bidirectionally
void ConnectRooms(DungeonRooms* a, DungeonRooms* b) 
{
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

#pragma endregion

#pragma region MemoryFreeing functions

void FreePlayer(Player* player)
{
if(player != NULL){
    free(player->CurrentRoom);
    player->CurrentRoom = NULL;
}
}

void FreeRooms(DungeonRooms* rooms, int amountOfRooms)
{
if(rooms != NULL){
    free(rooms);
}

}

#pragma endregion

