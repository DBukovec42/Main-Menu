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
	char* username;
	char* password;
} USER;

typedef enum {
	SETTINGS_USER = 1,
	SETTINGS_GAME,
	EXIT
} MENU;

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
	RESOLUTION_640X480 = 0,
	RESOLUTION_1280X720,
	RESOLUTION_1920X1080,
} RESOLUTION;

typedef enum {
	FRL_30 = 0,
	FRL_60,
	FRL_120,
	FRL_UNLIMITED
} FRL;

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
	RESOLUTION resolution;
	FRL frame_rate_limit;
	DIFFICULTY difficulty;
} GAME_SETTINGS;

extern USER player;
extern GAME_SETTINGS settings;

void clearBuffer(void);
int loginUser(const USER* user);
void startupMenu(USER* user);
void freeUser(USER* user);
void gameSettingsMenu(GAME_SETTINGS* settings);
GAME_SETTINGS defaultGameSettings(void);
void exitProgram(void);