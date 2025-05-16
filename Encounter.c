#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int Encounter(int contentID) // need to add player struct as a parameter
{

    int victorycondition = 0;
    switch (contentID)
    {
 
        case 1:
        // gain HP
        BuffEncounter(0);
        break;
        case 2:
        // gain DMG
        BuffEncounter(1);
        break;
        case 3:
        // fight Goblin
        FightEncounter(0);
        break;
        case 4:
        // fight ORC
        FightEncounter(1);
        break;
        case 6:
        // treasure / victory
        victorycondition = VictoryCondition();
        break;
    default:
        break;
    }
    return victorycondition;
}

void FightEncounter(int enemyID)
{
    srand(time(NULL));
if(enemyID == 0)
{

    FightOppenent(10,2,"Goblin");
    // fight Goblin
}else{
    FightOppenent(18,6,"ORC");
    // fight orc
}
}
void BuffEncounter(int BuffId)
{
if(BuffId == 0){
    // gain HP
}else {
    // Gain Dmg
}
}
int VictoryCondition()
{
 return 1;
}

FightOppenent(int hp,int dmg, char oppName) // need to add the player stuct ass a parameter
{

    while (hp != 0 ) // need to player hp as a variable 
    {
        int RandNummer = rand() % 17;
        unsigned int count = 0;
        while (RandNummer) {
            int oldcount = count;
            count += RandNummer & 1;
            RandNummer >>= 1;

            if(count == oldcount)
            {
                // oppAttack
                void OpponentAttack();
            }else{
                //player attack
                void playerAttack();
                
            }
        }
        return count;
    }
    
    printf(" you are fighting %s, the opponent has %i hp and deals %i damage\n", oppName,hp,dmg);

    
}

void playerAttack()
{

}
void OpponentAttack()
{

}
