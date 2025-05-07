#include <stdio.h>

// script where all the print statments are gathered
void PrintRoomSize(int roomsSize);
int  PrintGameStart();
void PrintRoomContents();


void PrintRoomSize(int roomsSize)
{
    printf("You entered a dungeon With %i rooms.", roomsSize);
}
int  PrintGameStart()
{
     int temp;
    printf("Hello there adventurer, which Dungeon do you want to visit? \n1) The small cave \n2) The medium ravine \n3) The momentous dungeon \nWhich one will you choose ?\n ");
    scanf(" %i", &temp);
    return temp;
}
void PrintRoomContents()
{
  //  printf(" this is room number %i with this content %i and these room connections 1) %i 2) %i 3) %i 4) %i", kamers[i].RoomNumber,kamers[i].Content,kamers[i].doorOne->RoomNumber,kamers[i].doorTwo->RoomNumber,kamers[i].doorThree->RoomNumber,kamers[i].doorFour->RoomNumber);
}