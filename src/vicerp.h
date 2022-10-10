#ifndef VICERP_H
#define VICERP_H

#include <stdint.h>

#define SERVER_NAME "ViceRP"
#define SERVER_GAMEMODE "TDM RP 0.2.1"

#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34

#define BIND_KEY_1 1
#define BIND_KEY_2 2

#define COLOR_GREEN 0x33AA33AA
#define COLOR_RED 0xFF0000FF
#define COLOR_GREY 0xAFAFAFAA
#define COLOR_WHITE 0xFFFFFFAA
#define COLOR_YELLOW 0xFFFF00AA
#define COLOR_BLUE 0x0000FFFF

#define MAX_PLAYER_NAME 64
#define UNKNOWN_KILLER_ID -1

#define COST_WE 10
#define COST_SPAWN 50
#define COST_DM 5
#define COST_REPAIR 10
#define COST_HEAL 5
#define COST_ARMOUR 20
#define MONEY_KILLER 200
#define MONEY_DAY 1000
#define MONEY_START 5000
#define COST_NITRO 5

uint8_t on_player_command(int32_t player_id, const char* msg);

#endif