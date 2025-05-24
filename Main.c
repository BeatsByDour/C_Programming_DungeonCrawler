// gcc main.c -o main.exe
#include "DungeonCrawler.c"
<<<<<<< HEAD
#include "saveFunctions.c"
=======
>>>>>>> dd9f2347bbbc8043439cd8fc4251142506f9b6ab
#include <stdio.h>

#define Clear() printf("\e[1;1H\e[2J")
int main()
{
<<<<<<< HEAD
    int endGame = 0;
    while (endGame == 0)
    {
        int SaveFileInfo = 0;
        DungeonRooms* kamers;
        Player Adventurer;
        int wincon = 0;
        int dungeonSize = 0;
=======
   int wincon = 0;
   int loseCon = 0;
   

    printf("hello DungeonCrawler \n");

    DungeonRooms* kamers = CreateDungeon();
    Player Adventurer = InitilizePlayer(); 
    Adventurer.CurrentRoom = &kamers[0];
   // DungeonRooms CurrentRoom = kamers[0];
>>>>>>> dd9f2347bbbc8043439cd8fc4251142506f9b6ab

       printf("Do you want to load a previous saveFile?\n\t (0) No\n\t (1) Yes\n");
       scanf(" %i" ,&SaveFileInfo );
       if(SaveFileInfo == 1)
       {
        load_game("saveFile.json",kamers, &Adventurer);
       }else
       {
            
        dungeonSize = PrintGameStart();
        printf("hello DungeonCrawler \n");
        kamers = CreateDungeon(dungeonSize);
        Adventurer = InitilizePlayer(); 
        Adventurer.CurrentRoom = &kamers[0];
       }

    // at the very start the player can choose to load a save file or start a new game. 
    // create player 
while (wincon == 0 && loseCon == 0)
{
    Adventurer.CurrentRoom->Isvisited = 1;
   // Clear();
    printf("You have %i / %i HP\nYou Deal %i Damage\n You are in Room %i\n",Adventurer.currentHp,Adventurer.maxHp,Adventurer.damageValue,Adventurer.CurrentRoom->RoomNumber);
    
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
    int nextRoom = 0;
    int ConCheck = 0;
    scanf(" %i", &nextRoom);
    ConCheck = EnterNewRoom(nextRoom,Adventurer.CurrentRoom,&Adventurer);

    switch (ConCheck)
    {
    case 1:
       wincon =1;
            break;
    case 2:
        loseCon = 1;
            break;
    default:
        break;
    }
    
    if(wincon == 0 && )
    Printf("Well done Completing this room")
    // in the loop the player can choose to save/ load a save file or go to another room 

    getchar();
}

<<<<<<< HEAD
FreePlayer(&Adventurer);
FreeRooms(kamers,dungeonSize);
// free the memomry of the previous session

int repeatAnswer = 0; 

while (repeatAnswer == 1  && repeatAnswer == 2)
{
    printf("You lost the game!\n Do you want to play Again (1)\nDo you want to quit the game (2)");
    scanf(" %i", &repeatAnswer);
    if(repeatAnswer == 2)
    {
        endGame = 1;
    } else if ( repeatAnswer =! 1)
    {
        printf("You entered an invalid answer in the prompt please enter 1 or 2 for answers!\n");
    }
}



    }
    

=======
if(loseCon == 1)
{
    printf(" player died whewhewhe  womp womp unlucky\n");
}
if(wincon == 1)
{
    printf(" player lived to tell the tale Yippie\n");
}
>>>>>>> dd9f2347bbbc8043439cd8fc4251142506f9b6ab
   
    return 0;
}