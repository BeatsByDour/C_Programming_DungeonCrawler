#include "DungeonCrawler.c"
#include "saveFunctions.c"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define Clear() printf("\e[1;1H\e[2J")

int isNumber(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int endGame = 0;
    while (endGame == 0) {
        int SaveFileInfo = 0;
        DungeonRooms* kamers = NULL;
        Player Adventurer;
        int wincon = 0;
        int loseCon = 0;
        int dungeonSize = 0;
        int actualDungeonSize = 0;
        int loadSuccessful = 0;
        int autoMode = 0; // Flag for automatic mode from command line

        // Handle command line arguments
        if (argc > 1) {
            const char* arg = argv[1];
            
            // Check if argument is a JSON file
            if (strlen(arg) > 5 && strcmp(arg + strlen(arg) - 5, ".json") == 0) {
                printf("Attempting to load save file: %s\n", arg);
                actualDungeonSize = load_game(arg, &kamers, &Adventurer);
                loadSuccessful = (actualDungeonSize > 0);
                
                if (loadSuccessful) {
                    autoMode = 1;
                    // Determine difficulty based on loaded size
                    if (actualDungeonSize == 10) dungeonSize = 1;
                    else if (actualDungeonSize == 25) dungeonSize = 2;
                    else if (actualDungeonSize == 50) dungeonSize = 3;
                    printf("Successfully loaded game from %s\n", arg);
                } else {
                    printf("Failed to load %s, falling back to interactive mode\n", arg);
                }
            }
            // Check if argument is a number (1, 2, or 3)
            else if (isNumber(arg)) {
                dungeonSize = atoi(arg);
                if (dungeonSize >= 1 && dungeonSize <= 3) {
                    autoMode = 1;
                    SaveFileInfo = 0; // Force new game
                    printf("Starting new game with dungeon size level %d\n", dungeonSize);
                } else {
                    printf("Invalid dungeon size %d (must be 1-3), falling back to interactive mode\n", dungeonSize);
                }
            }
        }

        // Interactive mode if no valid command line argument
        if (!autoMode) {
            printf("Do you want to load a previous saveFile?\n\t (0) No\n\t (1) Yes\n");
            scanf(" %i", &SaveFileInfo);
            
            if(SaveFileInfo == 1) {
                // Try to load the default game
                actualDungeonSize = load_game("saveFile.json", &kamers, &Adventurer);
                loadSuccessful = (actualDungeonSize > 0);
                
                if (!loadSuccessful) {
                    printf("Failed to load save file, starting new game instead\n");
                    SaveFileInfo = 0; // Fall through to new game creation
                } else {
                    // Determine the difficulty based on dungeon size
                    if (actualDungeonSize == 10) dungeonSize = 1;
                    else if (actualDungeonSize == 25) dungeonSize = 2;
                    else if (actualDungeonSize == 50) dungeonSize = 3;
                }
            }
        }
        Clear();
        if(SaveFileInfo == 0 && !autoMode) {
            // Create new game interactively
            dungeonSize = PrintGameStart();
            printf("hello DungeonCrawler \n");
        }
        
        // Set actual dungeon size based on difficulty
        if (SaveFileInfo == 0) {
            switch (dungeonSize) {
                case 1: actualDungeonSize = 10; break;
                case 2: actualDungeonSize = 25; break;
                case 3: actualDungeonSize = 50; break;
                default: actualDungeonSize = 10; break; // Default to easy
            }
            
            if (!loadSuccessful) { // Only create new dungeon if we didn't load one
                kamers = CreateDungeon(dungeonSize);
                Adventurer = InitilizePlayer(); 
                Adventurer.CurrentRoom = &kamers[0];
            }
        }
        Clear();
        // Main game loop
        while (wincon == 0 && loseCon == 0) {
            Adventurer.CurrentRoom->Isvisited = 1;
            // Clear();
            printf("You have %i / %i HP\nYou Deal %i Damage\nYou are in Room %i\n\n",
                  Adventurer.currentHp, Adventurer.maxHp, Adventurer.damageValue, 
                  Adventurer.CurrentRoom->RoomNumber);
            
            // Print room connections
            int connectionCount = CountConnections(Adventurer.CurrentRoom);
            switch (connectionCount) {
                case 2:
                    printf("Hello Adventurer, you are currently in room %i.\nWhere do you want to go?\n"
                          "This room has doors: \n\t(1) room %i \n\t(2) room %i\n", 
                          Adventurer.CurrentRoom->RoomNumber,
                          Adventurer.CurrentRoom->doors[0]->RoomNumber,
                          Adventurer.CurrentRoom->doors[1]->RoomNumber);
                    break;
                case 3:
                    printf("Hello Adventurer, you are currently in room %i.\nWhere do you want to go?\n"
                          "This room has doors: \n\t(1) room %i \n\t(2) room %i \n\t(3) room %i\n", 
                          Adventurer.CurrentRoom->RoomNumber,
                          Adventurer.CurrentRoom->doors[0]->RoomNumber,
                          Adventurer.CurrentRoom->doors[1]->RoomNumber,
                          Adventurer.CurrentRoom->doors[2]->RoomNumber);
                    break;
                case 4:
                    printf("Hello Adventurer, you are currently in room %i.\nWhere do you want to go?\n"
                          "This room has doors: \n\t(1) room %i \n\t(2) room %i \n\t(3) room %i \n\t(4) room %i\n", 
                          Adventurer.CurrentRoom->RoomNumber,
                          Adventurer.CurrentRoom->doors[0]->RoomNumber,
                          Adventurer.CurrentRoom->doors[1]->RoomNumber,
                          Adventurer.CurrentRoom->doors[2]->RoomNumber,
                          Adventurer.CurrentRoom->doors[3]->RoomNumber);
                    break;
            }
            
            int nextRoom = 0;
            scanf(" %i", &nextRoom);

            // Enternewroom function returns a 0,1 or 2, these numbers represent if the game is won, lost or still going 
            Clear();
            int ConCheck = EnterNewRoom(nextRoom, Adventurer.CurrentRoom, &Adventurer);

            // check if the game is won, lost or still going 
            switch (ConCheck) {
                case 1: wincon = 1; break;
                case 2: loseCon = 1; break;
                default: break;
            }
            Clear();
            // Only offer save if game isn't over
            if(!wincon && !loseCon) {
                int WantToSave = 0;
                printf("Well done Completing this room!\nDo You want to save the game ?\n\t (0) no\n\t (1) Yes\n");
                scanf(" %i", &WantToSave);
                if(WantToSave == 1) {
                    save_game("saveFile.json", kamers, actualDungeonSize, Adventurer);
                }
            }
            Clear();
            getchar();
        }

        // Game over messages
        if(loseCon == 1) {
            printf("Player died - game over!\n");
        }
        if(wincon == 1) {
            printf("Congratulations! You won the game!\n");
        }

        // Clean up
        FreePlayer(&Adventurer);
        FreeRooms(kamers, actualDungeonSize);

            // Play again prompt
            int repeatAnswer = 0; 
            printf("Game Over!\nDo you want to play Again?\n\t (1) Yes\n\t(2) No, quit the game\n");
            scanf(" %i", &repeatAnswer);
            
            while (repeatAnswer != 1 && repeatAnswer != 2) {
                printf("Invalid input! Please enter 1 to play again or 2 to quit: ");
                scanf(" %i", &repeatAnswer);
            }
            
            if(repeatAnswer == 2) {
                endGame = 1;
            }
    }
    return 0;
}