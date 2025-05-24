#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Dungeon.h"

// Simple JSON parser utilities

typedef struct {
    char* data;
    int position;
    int length;
} JsonParser;

void json_init(JsonParser* parser, char* data) {
    parser->data = data;
    parser->position = 0;
    parser->length = strlen(data);
}

void json_skip_whitespace(JsonParser* parser) {
    while (parser->position < parser->length && isspace(parser->data[parser->position])) {
        parser->position++;
    }
}

char json_peek(JsonParser* parser) {
    json_skip_whitespace(parser);
    if (parser->position >= parser->length) return '\0';
    return parser->data[parser->position];
}

char json_next(JsonParser* parser) {
    json_skip_whitespace(parser);
    if (parser->position >= parser->length) return '\0';
    return parser->data[parser->position++];
}

int json_parse_int(JsonParser* parser) {
    char ch;
    int value = 0;
    int sign = 1;
    
    ch = json_peek(parser);
    if (ch == '-') {
        sign = -1;
        json_next(parser);
    }
    
    while ((ch = json_peek(parser)) >= '0' && ch <= '9') {
        value = value * 10 + (ch - '0');
        json_next(parser);
    }
    
    return sign * value;
}

char* json_parse_string(JsonParser* parser) {
    if (json_next(parser) != '"') return NULL;
    
    char* start = parser->data + parser->position;
    int length = 0;
    
    while (parser->position < parser->length && parser->data[parser->position] != '"') {
        parser->position++;
        length++;
    }
    
    if (json_next(parser) != '"') return NULL;
    
    char* str = malloc(length + 1);
    strncpy(str, start, length);
    str[length] = '\0';
    return str;
}

void json_expect(JsonParser* parser, char expected) {
    char ch = json_next(parser);
    if (ch != expected) {
        fprintf(stderr, "JSON parse error: Expected '%c' but got '%c'\n", expected, ch);
        exit(1);
    }
}

// Game save/load functions

void save_int(FILE* file, const char* key, int value, int is_last) {
    fprintf(file, "    \"%s\": %d%s\n", key, value, is_last ? "" : ",");
}

void save_string(FILE* file, const char* key, const char* value, int is_last) {
    fprintf(file, "    \"%s\": \"%s\"%s\n", key, value, is_last ? "" : ",");
}

void save_game(const char* filename, DungeonRooms* rooms, int dungeon_size, Player player) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open save file");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"dungeon\": {\n");
    save_int(file, "size", dungeon_size, 0);
    fprintf(file, "    \"rooms\": [\n");

    // Save each room
    for (int i = 0; i < dungeon_size; i++) {
        fprintf(file, "      {\n");
        save_int(file, "roomNumber", rooms[i].RoomNumber, 0);
        save_int(file, "content", rooms[i].Content, 0);
        
        // Save connections
        fprintf(file, "        \"connections\": [");
        for (int d = 0; d < 4; d++) {
            if (rooms[i].doors[d] != NULL) {
                fprintf(file, "%d", rooms[i].doors[d]->RoomNumber);
            } else {
                fprintf(file, "null");
            }
            if (d < 3) fprintf(file, ", ");
        }
        fprintf(file, "]\n");
        
        fprintf(file, "      }%s\n", i < dungeon_size - 1 ? "," : "");
    }

    fprintf(file, "    ]\n");
    fprintf(file, "  },\n");

    // Save player data
    fprintf(file, "  \"player\": {\n");
    save_int(file, "maxHp", player.maxHp, 0);
    save_int(file, "currentHp", player.currentHp, 0);
    save_int(file, "damageValue", player.damageValue, 0);
    save_int(file, "currentRoomNumber", player.CurrentRoom->RoomNumber, 1);
    fprintf(file, "  }\n");

    fprintf(file, "}\n");
    fclose(file);
    printf("Game saved to %s\n", filename);
}

int find_key(JsonParser* parser, const char* key) {
    char* key_str = json_parse_string(parser);
    if (key_str && strcmp(key_str, key) == 0) {
        free(key_str);
        json_expect(parser, ':');
        return 1;
    }
    free(key_str);
    return 0;
}

DungeonRooms* load_rooms(JsonParser* parser, int* dungeon_size) {
    // Parse dungeon size
    if (!find_key(parser, "size")) return NULL;
    *dungeon_size = json_parse_int(parser);
    
    // Find rooms array
    if (json_next(parser) != ',' || !find_key(parser, "rooms")) return NULL;
    json_expect(parser, '[');
    
    // Allocate rooms array
    DungeonRooms* rooms = calloc(*dungeon_size, sizeof(DungeonRooms));
    int room_count = 0;
    
    // First pass: create rooms without connections
    while (json_peek(parser) != ']' && room_count < *dungeon_size) {
        json_expect(parser, '{');
        
        // Parse room number
        if (!find_key(parser, "roomNumber")) break;
        rooms[room_count].RoomNumber = json_parse_int(parser);
        json_expect(parser, ',');
        
        // Parse content
        if (!find_key(parser, "content")) break;
        rooms[room_count].Content = json_parse_int(parser);
        json_expect(parser, ',');
        
        // Initialize doors to NULL
        for (int d = 0; d < 4; d++) {
            rooms[room_count].doors[d] = NULL;
        }
        
        // Skip connections for now (we'll process in second pass)
        if (!find_key(parser, "connections")) break;
        json_expect(parser, '[');
        while (json_peek(parser) != ']') {
            if (json_peek(parser) == 'n') { // null
                parser->position += 4; // skip "null"
            } else {
                json_parse_int(parser); // just skip the number
            }
            if (json_peek(parser) == ',') json_next(parser);
        }
        json_expect(parser, ']');
        
        json_expect(parser, '}');
        if (json_peek(parser) == ',') json_next(parser);
        
        room_count++;
    }
    
    json_expect(parser, ']');
    return rooms;
}

void load_connections(JsonParser* parser, DungeonRooms* rooms, int dungeon_size) {
    // Rewind to rooms array
    parser->position = 0;
    find_key(parser, "dungeon");
    find_key(parser, "size");
    json_parse_int(parser);
    json_expect(parser, ',');
    find_key(parser, "rooms");
    json_expect(parser, '[');
    
    int room_idx = 0;
    while (json_peek(parser) != ']' && room_idx < dungeon_size) {
        json_expect(parser, '{');
        
        // Skip roomNumber and content
        find_key(parser, "roomNumber");
        json_parse_int(parser);
        json_expect(parser, ',');
        find_key(parser, "content");
        json_parse_int(parser);
        json_expect(parser, ',');
        
        // Process connections
        find_key(parser, "connections");
        json_expect(parser, '[');
        
        int door_idx = 0;
        while (json_peek(parser) != ']' && door_idx < 4) {
            if (json_peek(parser) == 'n') { // null
                parser->position += 4; // skip "null"
                rooms[room_idx].doors[door_idx] = NULL;
            } else {
                int connected_room_num = json_parse_int(parser);
                // Find the room with this number
                for (int i = 0; i < dungeon_size; i++) {
                    if (rooms[i].RoomNumber == connected_room_num) {
                        rooms[room_idx].doors[door_idx] = &rooms[i];
                        break;
                    }
                }
            }
            door_idx++;
            if (json_peek(parser) == ',') json_next(parser);
        }
        json_expect(parser, ']');
        
        json_expect(parser, '}');
        if (json_peek(parser) == ',') json_next(parser);
        
        room_idx++;
    }
}

Player load_player(JsonParser* parser, DungeonRooms* rooms, int dungeon_size) {
    Player player;
    
    // Find player object
    json_expect(parser, ',');
    if (!find_key(parser, "player")) {
        fprintf(stderr, "Player data not found in save file\n");
        exit(1);
    }
    json_expect(parser, '{');
    
    // Load player stats
    if (!find_key(parser, "maxHp")) exit(1);
    player.maxHp = json_parse_int(parser);
    json_expect(parser, ',');
    
    if (!find_key(parser, "currentHp")) exit(1);
    player.currentHp = json_parse_int(parser);
    json_expect(parser, ',');
    
    if (!find_key(parser, "damageValue")) exit(1);
    player.damageValue = json_parse_int(parser);
    json_expect(parser, ',');
    
    if (!find_key(parser, "currentRoomNumber")) exit(1);
    int current_room_num = json_parse_int(parser);
    json_expect(parser, '}');
    
    // Find the current room
    player.CurrentRoom = NULL;
    for (int i = 0; i < dungeon_size; i++) {
        if (rooms[i].RoomNumber == current_room_num) {
            player.CurrentRoom = malloc(sizeof(DungeonRooms));
            *player.CurrentRoom = rooms[i];
            break;
        }
    }
    
    if (!player.CurrentRoom) {
        fprintf(stderr, "Couldn't find player's current room in loaded dungeon\n");
        exit(1);
    }
    
    return player;
}

int load_game(const char* filename, DungeonRooms** rooms, Player* player) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open save file");
        return 0;
    }
    
    // Read file contents
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';
    
    // Initialize parser
    JsonParser parser;
    json_init(&parser, buffer);
    
    // Parse the JSON
    json_expect(&parser, '{');
    
    // Find dungeon object
    if (!find_key(&parser, "dungeon")) {
        fprintf(stderr, "Dungeon data not found in save file\n");
        free(buffer);
        return 0;
    }
    json_expect(&parser, '{');
    
    // Load rooms (first pass - without connections)
    int dungeon_size;
    *rooms = load_rooms(&parser, &dungeon_size);
    if (!*rooms) {
        fprintf(stderr, "Failed to load rooms\n");
        free(buffer);
        return 0;
    }
    
    // Load connections (second pass)
    load_connections(&parser, *rooms, dungeon_size);
    
    // Load player data
    *player = load_player(&parser, *rooms, dungeon_size);
    
    free(buffer);
    return dungeon_size;
}

// Example usage:
/*
int main() {
    // Initialize game
    int dungeon_size = 10;
    DungeonRooms* kamers = CreateDungeon(dungeon_size);
    Player Adventurer = InitilizePlayer();
    Adventurer.CurrentRoom = &kamers[0];
    
    // Save game
    save_game("savegame.json", kamers, dungeon_size, Adventurer);
    
    // Load game
    DungeonRooms* loaded_rooms = NULL;
    Player loaded_player;
    int loaded_size = load_game("savegame.json", &loaded_rooms, &loaded_player);
    
    if (loaded_size > 0) {
        // Use loaded game state...
        
        // Clean up
        free(loaded_rooms);
        free(loaded_player.CurrentRoom);
    }
    
    // Clean up original game
    free(kamers);
    free(Adventurer.CurrentRoom);
    
    return 0;
}
*/