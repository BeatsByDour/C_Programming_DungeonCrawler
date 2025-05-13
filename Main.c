// gcc main.c -o main.exe
#include "DungeonCrawler.c"
#include "Encounter.c"
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

    printf("Hello Adventurer What is the next room you want to enter? ")

   // create while loop for player to go to the next dungeon

    // in the loop the player can choose to save/ load a save file or go to another room 

    // while the player is in an another room it will fight or loot the room

    // after the encounter is over the player will loop back to the choice to load/save or go to another room 

    // if the player dies, free the memory of the dungeon and player and give a choice to restart or quit the game

    // at the very start the player can choose to load a save file or start a new game. 
}

   
    return 0;
}