#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Dungeon.h"

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
    while (parser->position < parser->length && 
          (isspace(parser->data[parser->position]) || 
           parser->data[parser->position] == '\n' ||
           parser->data[parser->position] == '\r')) {
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
    if (!str) return NULL;
    
    strncpy(str, start, length);
    str[length] = '\0';
    return str;
}

void json_expect(JsonParser* parser, char expected) {
    char ch = json_next(parser);
    if (ch != expected) {
        fprintf(stderr, "JSON parse error at position %d: Expected '%c' but got '%c'\n", 
                parser->position, expected, ch);
        fprintf(stderr, "Context: %.*s\n", 20, parser->data + parser->position - 10);
        exit(1);
    }
}

void save_int(FILE* file, const char* key, int value, int is_last) {
    fprintf(file, "    \"%s\": %d%s\n", key, value, is_last ? "" : ",");
}

void save_game(const char* filename, DungeonRooms* rooms, int dungeon_size, Player player) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open save file for writing");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"dungeon\": {\n");
    save_int(file, "size", dungeon_size, 0);
    fprintf(file, "    \"rooms\": [\n");

    for (int i = 0; i < dungeon_size; i++) {
        fprintf(file, "      {\n");
        save_int(file, "roomNumber", rooms[i].RoomNumber, 0);
        save_int(file, "content", rooms[i].Content, 0);
        save_int(file, "isVisited", rooms[i].Isvisited, 0);
        
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

    fprintf(file, "  \"player\": {\n");
    save_int(file, "maxHp", player.maxHp, 0);
    save_int(file, "currentHp", player.currentHp, 0);
    save_int(file, "damageValue", player.damageValue, 0);
    save_int(file, "currentRoomNumber", player.CurrentRoom->RoomNumber, 1);
    fprintf(file, "  }\n");

    fprintf(file, "}\n");
    
    if (fclose(file) != 0) {
        perror("Failed to properly close save file");
    }
    printf("Game successfully saved to %s\n", filename);
}

int find_key(JsonParser* parser, const char* key) {
    char* key_str = json_parse_string(parser);
    if (!key_str) return 0;
    
    int result = (strcmp(key_str, key) == 0);
    free(key_str);
    
    if (result) {
        json_expect(parser, ':');
        return 1;
    }
    return 0;
}

int load_game(const char* filename, DungeonRooms** rooms, Player* player) {
    printf("Attempting to load save file: %s\n", filename);
    
    // Open file with error checking
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open save file");
        return 0;
    }

    // Get file size with error checking
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(file);
        return 0;
    }

    long length = ftell(file);
    if (length < 0) {
        perror("ftell failed");
        fclose(file);
        return 0;
    }

    if (length == 0) {
        fprintf(stderr, "Save file is empty\n");
        fclose(file);
        return 0;
    }

    rewind(file);

    // Read file contents
    char* buffer = malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed for file buffer\n");
        fclose(file);
        return 0;
    }

    size_t bytes_read = fread(buffer, 1, length, file);
    if (bytes_read != (size_t)length) {
        fprintf(stderr, "File read incomplete: %zu of %ld bytes read\n", bytes_read, length);
        free(buffer);
        fclose(file);
        return 0;
    }
    buffer[length] = '\0';

    if (fclose(file) != 0) {
        perror("Warning: Failed to close file properly");
    }

    // Debug: Print first 200 characters
    printf("File contents (first 200 chars):\n%.200s\n", buffer);

    // Initialize parser
    JsonParser parser;
    json_init(&parser, buffer);

    // Start parsing JSON
    json_expect(&parser, '{');

    // Find dungeon object
    if (!find_key(&parser, "dungeon")) {
        fprintf(stderr, "Missing 'dungeon' object in save file\n");
        free(buffer);
        return 0;
    }
    json_expect(&parser, '{');

    // Parse dungeon size
    if (!find_key(&parser, "size")) {
        fprintf(stderr, "Missing 'size' in dungeon data\n");
        free(buffer);
        return 0;
    }
    
    int dungeon_size = json_parse_int(&parser);
    if (dungeon_size <= 0) {
        fprintf(stderr, "Invalid dungeon size: %d\n", dungeon_size);
        free(buffer);
        return 0;
    }

    // Skip comma after size
    json_skip_whitespace(&parser);
    if (json_peek(&parser) == ',') json_next(&parser);

    // Find rooms array
    if (!find_key(&parser, "rooms")) {
        fprintf(stderr, "Missing 'rooms' array\n");
        free(buffer);
        return 0;
    }
    json_expect(&parser, '[');

    // Allocate rooms
    *rooms = calloc(dungeon_size, sizeof(DungeonRooms));
    if (!*rooms) {
        fprintf(stderr, "Failed to allocate memory for rooms\n");
        free(buffer);
        return 0;
    }

    // Parse rooms
    int room_index = 0;
    while (json_peek(&parser) != ']' && room_index < dungeon_size) {
        json_expect(&parser, '{');

        // Parse room properties
        while (json_peek(&parser) != '}') {
            char* key = json_parse_string(&parser);
            if (!key) {
                fprintf(stderr, "Failed to parse room property key\n");
                free(*rooms);
                free(buffer);
                return 0;
            }

            json_expect(&parser, ':');
            
            if (strcmp(key, "roomNumber") == 0) {
                (*rooms)[room_index].RoomNumber = json_parse_int(&parser);
            } 
            else if (strcmp(key, "content") == 0) {
                (*rooms)[room_index].Content = json_parse_int(&parser);
            } 
            else if (strcmp(key, "isVisited") == 0) {
                (*rooms)[room_index].Isvisited = json_parse_int(&parser);
            } 
            else if (strcmp(key, "connections") == 0) {
                // Parse connections array
                json_expect(&parser, '[');

                int conn_index = 0;
                while (json_peek(&parser) != ']' && conn_index < 4) {
                    if (json_peek(&parser) == 'n') { // null
                        json_next(&parser); // 'n'
                        json_next(&parser); // 'u'
                        json_next(&parser); // 'l'
                        json_next(&parser); // 'l'
                        (*rooms)[room_index].doors[conn_index] = NULL;
                    } 
                    else {
                        int room_num = json_parse_int(&parser);
                        // Find and connect to the room with this number
                        for (int i = 0; i < dungeon_size; i++) {
                            if ((*rooms)[i].RoomNumber == room_num) {
                                (*rooms)[room_index].doors[conn_index] = &(*rooms)[i];
                                break;
                            }
                        }
                    }
                    conn_index++;
                    
                    // Skip comma if present
                    json_skip_whitespace(&parser);
                    if (json_peek(&parser) == ',') json_next(&parser);
                }
                json_expect(&parser, ']');
            }

            free(key);
            
            // Skip comma if present
            json_skip_whitespace(&parser);
            if (json_peek(&parser) == ',') json_next(&parser);
        }
        json_expect(&parser, '}');
        room_index++;

        // Skip comma if present
        json_skip_whitespace(&parser);
        if (json_peek(&parser) == ',') json_next(&parser);
    }
    json_expect(&parser, ']');
    json_expect(&parser, '}');

    // Parse player data
    json_skip_whitespace(&parser);
    if (json_peek(&parser) == ',') json_next(&parser);

    if (!find_key(&parser, "player")) {
        fprintf(stderr, "Missing player data\n");
        free(*rooms);
        free(buffer);
        return 0;
    }
    json_expect(&parser, '{');

    // Initialize player with default values
    *player = InitilizePlayer();
    
    while (json_peek(&parser) != '}') {
        char* key = json_parse_string(&parser);
        if (!key) {
            fprintf(stderr, "Failed to parse player property key\n");
            free(*rooms);
            free(buffer);
            return 0;
        }

        json_expect(&parser, ':');
        
        if (strcmp(key, "maxHp") == 0) {
            player->maxHp = json_parse_int(&parser);
        } 
        else if (strcmp(key, "currentHp") == 0) {
            player->currentHp = json_parse_int(&parser);
        } 
        else if (strcmp(key, "damageValue") == 0) {
            player->damageValue = json_parse_int(&parser);
        } 
        else if (strcmp(key, "currentRoomNumber") == 0) {
            int room_num = json_parse_int(&parser);
            // Find the room
            for (int i = 0; i < dungeon_size; i++) {
                if ((*rooms)[i].RoomNumber == room_num) {
                    // Free existing current room if it exists
                    if (player->CurrentRoom) {
                        free(player->CurrentRoom);
                    }
                    player->CurrentRoom = malloc(sizeof(DungeonRooms));
                    if (!player->CurrentRoom) {
                        fprintf(stderr, "Failed to allocate current room\n");
                        free(key);
                        free(*rooms);
                        free(buffer);
                        return 0;
                    }
                    memcpy(player->CurrentRoom, &(*rooms)[i], sizeof(DungeonRooms));
                    break;
                }
            }
            if (!player->CurrentRoom) {
                fprintf(stderr, "Couldn't find player's current room %d\n", room_num);
                free(key);
                free(*rooms);
                free(buffer);
                return 0;
            }
        }

        free(key);
        
        // Skip comma if present
        json_skip_whitespace(&parser);
        if (json_peek(&parser) == ',') json_next(&parser);
    }
    json_expect(&parser, '}');
    json_expect(&parser, '}');

    free(buffer);
    printf("Successfully loaded save file with %d rooms\n", dungeon_size);
    return dungeon_size;
}