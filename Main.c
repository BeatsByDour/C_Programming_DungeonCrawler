// gcc main.c -o main.exe
#include "DungeonCrawler.c"
//#include "Encounter.c"
#include <stdio.h>

int main()
{
   int wincon = 0;
   

    printf("hello DungeonCrawler \n");

    DungeonRooms* kamers = CreateDungeon();
    Player Adventurer = InitilizePlayer(); 
    Adventurer.CurrentRoom = &kamers[0];
   // DungeonRooms CurrentRoom = kamers[0];


    // create player 
while (wincon == 0)
{

    switch (CountConnections(Adventurer.CurrentRoom))
    {
    case 2:
         printf("Hello Adventurer, you are currently in room %i .\nWhere do you want to go?\nThis room has doors: \n(1) room %i \n(2) room %i!\n", Adventurer.CurrentRoom->RoomNumber,Adventurer.CurrentRoom->doors[0]->RoomNumber,Adventurer.CurrentRoom->doors[1]->RoomNumber);
        break;
         case 3:
         printf("Hello Adventurer, you are currently in room %i .\nWhere do you want to go?\nThis room has doors: \n(1) room %i \n(2) room %i  \n(3) room %i!\n", Adventurer.CurrentRoom->RoomNumber,Adventurer.CurrentRoom->doors[0]->RoomNumber,Adventurer.CurrentRoom->doors[1]->RoomNumber,Adventurer.CurrentRoom->doors[2]->RoomNumber);
        break;
         case 4:
         printf("Hello Adventurer, you are currently in room %i .\nWhere do you want to go?\nThis room has doors: \n(1) room %i \n(2) room %i \n(3) room %i \n(4) room %i!\n", Adventurer.CurrentRoom->RoomNumber,Adventurer.CurrentRoom->doors[0]->RoomNumber,Adventurer.CurrentRoom->doors[1]->RoomNumber,Adventurer.CurrentRoom->doors[2]->RoomNumber,Adventurer.CurrentRoom->doors[3]->RoomNumber);
        break;
    }
    int nextRoom;
    scanf(" %i", &nextRoom -1);
    EnterNewRoom(nextRoom,Adventurer.CurrentRoom,&Adventurer);
    
   // create while loop for player to go to the next dungeon

    // in the loop the player can choose to save/ load a save file or go to another room 

    // while the player is in an another room it will fight or loot the room

    // after the encounter is over the player will loop back to the choice to load/save or go to another room 

    // if the player dies, free the memory of the dungeon and player and give a choice to restart or quit the game

    // at the very start the player can choose to load a save file or start a new game. 
    getchar();
}

   
    return 0;
}