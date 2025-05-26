#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "Dungeon.h"

int Encounter(int contentID, Player* Adventurer) 
{
    sleep(5);
    switch (contentID)
    {
 
        case 1:
        // gain HP
        printf("You found An alter with a Buff!!! Yippie\n");
        BuffEncounter(0,Adventurer);
        break;
        case 2:
        // gain DMG
        printf("You found An alter with a Buff!!! Yippie\n");
        BuffEncounter(1,Adventurer);
        break;
        case 3:
        // fight Goblin
        printf("You found A monster in this room!!! WHAAAA\n");
        return FightEncounter(0,Adventurer);
        break;
        case 4:
        // fight ORC
        printf("You found A monster in this room!!! WHAAAA\n");
        return FightEncounter(1,Adventurer);
        break;
        case 6:
        // treasure / victory
        printf("You found the treasure Yippie!!! YAAAHOOOOO\n");
        return  VictoryCondition();
        break;
    default:
        printf("There is nothing in this room!!! WOMP WOMP\n");
        break;
    }
    return 0;
}

int FightEncounter(int enemyID, Player* Adventurer)
{
     sleep(3);
    srand(time(NULL));
if(enemyID == 0)
{
    // fight Goblin
   return FightOppenent(10,2,"Goblin",Adventurer);
}else{
    // fight orc
   return FightOppenent(18,6,"ORC",Adventurer);
}
}
void BuffEncounter(int BuffId, Player* Adventurer)
{
     sleep(3);
if(BuffId == 0){
    printf("The alter Has regenerated your hp!!! Yippie\n");
    if(Adventurer->currentHp + 15 >= Adventurer->maxHp+5){
        Adventurer->maxHp = Adventurer->maxHp + 5;
        Adventurer->currentHp = Adventurer->maxHp;
    }else{
        Adventurer->maxHp =+ 5;
        Adventurer->currentHp =+ 15;
    }
    sleep(3);
}else {
    printf("The alter Has given you more strength!!! Yippie\n");
   Adventurer->damageValue = Adventurer->damageValue + 2;
    sleep(3);
}
}
int VictoryCondition()
{
 return 1;
}

    int FightOppenent(int hp,int dmg, char oppName[10], Player* Adventurer) // need to add the player stuct ass a parameter
{
    printf("you are fighting %s, the opponent has %i hp and deals %i damage\n", oppName,hp,dmg);
    while (hp >= 0  && Adventurer->currentHp >=0) // need to player hp as a variable 
    {
        int RandNummer = rand() % 16;
        unsigned int count = 0;
      
        while (RandNummer) {
            sleep(2);
            int oldcount = count;
            count += RandNummer & 1;
            RandNummer >>= 1;
            //_sleep(1);
            if(count == oldcount)
            {
               
                Adventurer->currentHp = Adventurer->currentHp - dmg;
                printf("\tYou got hit for %i damage and this is your remaining HP: %i\t\t ( %i )  \n",dmg,Adventurer->currentHp,0);

            }else{
               
                hp = hp - Adventurer->damageValue;  
                printf("\tYou hit the %s for %i damage and this is their remaining HP: %i\t\t ( %i )  \n", oppName,Adventurer->damageValue,hp,1);
            }
        }
        
    }
    if(Adventurer->currentHp <= 0 )
    {
        sleep(3);
        printf("OOH NOO You DIED WOMP WOMP\n");
        return 2;

    }else
    {
        sleep(3);
        printf("Well done Adventurer You WON, HeHe, on to the next room!!!\n");
        return 0;
    }

}
