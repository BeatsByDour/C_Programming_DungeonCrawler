#include <stdio.h>
// script where all the print statments are gathered
void PrintRoomSize(int roomsSize);
int  PrintGameStart();


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