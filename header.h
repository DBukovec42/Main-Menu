#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#define USER_FILE "user.txt"

typedef struct {
	char username[20];
	char password[20];
} USER;

typedef enum {
	GRAPHICS_LOW = 0,
	GRAPHICS_MEDIUM,
	GRAPHICS_HIGH,
} GRAPHICS;

typedef enum {
	DISPLAY_FULLSCREEN = 0,
	DISPLAY_WINDOWED,
	DISPLAY_BORDERLESS_WINDOW,
} DISPLAY;

typedef enum {
	DIFFICULTY_EASY = 0,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_INSANE
} DIFFICULTY;

typedef struct {
	int volume;
	GRAPHICS graphics;
	DISPLAY display_mode;
	int resolution;
	int frame_rate_limit;
	DIFFICULTY difficulty;
} GAME_SETTINGS;

void clearBuffer(void);
int loginUser(const USER* user);
void startupMenu(USER* user);
void gameSettingsMenu(GAME_SETTINGS* settings);
GAME_SETTINGS defaultGameSettings(void);
void exitProgram(void);