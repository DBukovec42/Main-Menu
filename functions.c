#include "header.h"

// BUFFER CLEAR
void clearBuffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// USERNAME TAKEN
static bool usernameTaken(const char* username) {
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		return false;
	}

	char existing_username[20];
	bool taken = fscanf(file, "%19s", existing_username) == 1 && strcmp(existing_username, username) == 0;
	fclose(file);

	return taken;
}

// USER CREATION
static bool createUser(USER* user) {
	freeUser(user);
	char user_input[20];
	printf("\nUSER CREATION\n");

	while (1) {
		printf("Choose a new username (or type exit to go back): ");
		scanf("%19s", user_input);

		if (strcmp(user_input, "exit") == 0) {
			return false;
		}

		if (usernameTaken(user_input)) {
			printf("User with that username already exists.\n\n");
			continue;
		}
		break;
	}

	user->username = malloc(strlen(user_input) + 1);

	if (!user->username) {
		printf("Memory allocation failed.\n");
		return false;
	}
	strcpy(user->username, user_input);

	printf("Choose a new password: ");
	scanf("%19s", user_input);

	user->password = malloc(strlen(user_input) + 1);

	if (!user->password) {
		printf("Memory allocation failed.\n");
		free(user->username);
		user->username = NULL;
		return false;
	}
	strcpy(user->password, user_input);

	return true;
}

// SAVE USER
static bool saveUser(const USER* user) {
	FILE* file = fopen(USER_FILE, "w");

	if (!file) {
		printf("Could not open file.\n");
		return false;
	}

	bool success = fprintf(file, "%s\n%s\n", user->username, user->password) > 0;
	fclose(file);

	if (!success) {
		printf("Could not save user data.\n");
	}

	return success;
}

// LOAD USER
static bool loadUser(USER* user) {
	freeUser(user);
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		return false;
	}

	char user_input[20];

	if (fscanf(file, "%19s", user_input) != 1) {
		fclose(file); return false;
	}
	user->username = malloc(strlen(user_input) + 1);

	if (!user->username) { 
		fclose(file); return false;
	}
	strcpy(user->username, user_input);

	if (fscanf(file, "%19s", user_input) != 1) {
		fclose(file);
		free(user->username);
		user->username = NULL;
		return false;
	}
	user->password = malloc(strlen(user_input) + 1);

	if (!user->password) {
		fclose(file);
		free(user->username);
		user->username = NULL;
		return false;
	}
	strcpy(user->password, user_input);

	fclose(file);
	return true;
}

// LOG IN
int loginUser(const USER* user) {
	char entered_username[20];
	char entered_password[20];

	printf("\nLOG IN\n");
	printf("Enter username (or type exit to go back): ");
	scanf("%19s", entered_username);
	if (strcmp(entered_username, "exit") == 0) {
		return -1;
	}
	printf("Enter password: ");
	scanf("%19s", entered_password);

	if (strcmp(user->username, entered_username) == 0 && strcmp(user->password, entered_password) == 0) {
		printf("Welcome!\n");
		return 1;
	}
	else {
		printf("Incorrect username or password.\n");
		return 0;
	}
}

// CLEANUP
void freeUser(USER* user) {
	free(user->username);
	free(user->password);
	user->username = NULL;
	user->password = NULL;
}

// STARTUP MENU
void startupMenu(USER* user) {
	int startup_choice;

	while (1) {
		printf("\nSTARTUP MENU\n");
		printf("1.Log In\n");
		printf("2.Create New User\n");
		printf("Choose: ");

		if (scanf("%d", &startup_choice) != 1) {
			printf("Enter a valid number.\n");
			clearBuffer();
			continue;
		}

		if (startup_choice == 1) {
			if (!loadUser(user)) {
				printf("No existing users found.\n");
				continue;
			}
			break;
		}
		else if (startup_choice == 2) {
			if (createUser(user) && saveUser(user)) {
				printf("New user successfully created!\n");
			}
		}
		else {
			printf("Enter a valid number.\n");
		}
	}
}

// GRAPHICS TO STRING
static const char* graphicsToString(GRAPHICS graphics) {
	static const char* const graphics_text[] = { "Low", "Medium", "High" };

	if (graphics >= GRAPHICS_LOW && graphics <= GRAPHICS_HIGH) {
		return graphics_text[graphics];
	}
	return "Unknown";
}

// DISPLAY MODE TO STRING
static const char* displayModeToString(DISPLAY display) {
	static const char* const display_mode[] = { "Fullscreen", "Windowed", "Borderless Window" };

	if (display >= DISPLAY_FULLSCREEN && display <= DISPLAY_BORDERLESS_WINDOW) {
		return display_mode[display];
	}
	return "Unknown";
}

// RESOLUTION TO STRING
static const char* resolutionToString(RESOLUTION resolution) {
	static const char* const resolutions[] = { "640x480", "1280x720", "1920x1080" };

	if (resolution >= RESOLUTION_640X480 && resolution <= RESOLUTION_1920X1080) {
		return resolutions[resolution];
	}
	return "Unknown";
}

// FRAME RATE LIMIT TO STRING
static const char* frlToString(FRL frl) {
	static const char* const frame_rate_limit[] = { "30", "60", "120", "Unlimited" };

	if (frl >= FRL_30 && frl <= FRL_UNLIMITED) {
		return frame_rate_limit[frl];
	}
	return "Unknown";
}

// DIFFICULTY TO STRING
static const char* difficultyToString(DIFFICULTY difficulty) {
	static const char* const difficulties[] = { "Easy", "Normal", "Hard", "Insane" };

	if (difficulty >= DIFFICULTY_EASY && difficulty <= DIFFICULTY_INSANE) {
		return difficulties[difficulty];
	}
	return "Unknown";
}

// GAME SETTINGS
void gameSettingsMenu(GAME_SETTINGS* settings) {
	int game_settings_choice;

	while (1) {
		printf("\nGAME SETTINGS\n");
		printf("1. Volume\n");
		printf("2. Graphics\n");
		printf("3. Display mode\n");
		printf("4. Resolution\n");
		printf("5. Frame rate limit\n");
		printf("6. Difficulty\n");
		printf("7. Go back\n");
		printf("Choose: ");

		if (scanf("%d", &game_settings_choice) != 1) {
			printf("Enter a valid number.\n");
			clearBuffer();
			continue;
		}
		switch (game_settings_choice) {
		case 1: {
			int temp_volume;
			while (1) {
				printf("\nVOLUME SETTINGS\n");
				printf("Current volume: %d\n", settings->volume);
				printf("Enter desired volume [1-10]: ");

				if (scanf("%d", &temp_volume) != 1) {
					printf("Enter a valid number.\n");
					clearBuffer();
					continue;
				}

				if (temp_volume < 1 || temp_volume > 10) {
					printf("Enter a valid number.\n");
				}
				else {
					settings->volume = temp_volume;
					printf("Volume set to: %d\n", settings->volume);
					break;
				}
			}
			break;
		}
		case 2: {
			int temp_graphics;
			while (1) {
				printf("\nGRAPHICS SETTINGS\n");
				printf("Current graphics: %s\n", graphicsToString(settings->graphics));
				printf("1. Low\n");
				printf("2. Medium\n");
				printf("3. High\n");
				printf("Choose: ");

				if (scanf("%d", &temp_graphics) != 1) {
					printf("Enter a valid number.\n");
					clearBuffer();
					continue;
				}

				if (temp_graphics < 1 || temp_graphics > 3) {
					printf("Enter a valid number.\n");
				}
				else {
					settings->graphics = (GRAPHICS)(temp_graphics - 1);
					printf("Graphics set to: %s\n", graphicsToString(settings->graphics));
					break;
				}
			}
			break;
		}
		case 3: {
			int temp_display_mode;
			while (1) {
				printf("\nDISPLAY MODE SETTINGS\n");
				printf("Current display mode: %s\n", displayModeToString(settings->display_mode));
				printf("1. Fullscreen\n");
				printf("2. Windowed\n");
				printf("3. Borderless window\n");
				printf("Choose: ");

				if (scanf("%d", &temp_display_mode) != 1) {
					printf("Enter a valid number.\n");
					clearBuffer();
					continue;
				}

				if (temp_display_mode < 1 || temp_display_mode > 3) {
					printf("Enter a valid number.\n");
				}
				else {
					settings->display_mode = (DISPLAY)(temp_display_mode - 1);
					printf("Display mode set to: %s\n", displayModeToString(settings->display_mode));
					break;
				}
			}
			break;
		}
		case 4: {
			int temp_resolution;
			while (1) {
				printf("\nRESOLUTION SETTINGS\n");
				printf("Current resolution: %s\n", resolutionToString(settings->resolution));
				printf("1. 640x480\n");
				printf("2. 1280x720\n");
				printf("3. 1920x1080\n");
				printf("Choose: ");

				if (scanf("%d", &temp_resolution) != 1) {
					printf("Enter a valid number.\n");
					clearBuffer();
					continue;
				}

				if (temp_resolution < 1 || temp_resolution > 3) {
					printf("Enter a valid number.\n");
				}
				else {
					settings->resolution = (RESOLUTION)(temp_resolution - 1);
					printf("Resolution set to: %s\n", resolutionToString(settings->resolution));
					break;
				}
			}
			break;
		}
		case 5: {
			int temp_frl;
			while (1) {
				printf("\nFRAME RATE LIMIT SETTINGS\n");
				printf("Current frame rate limit: %s\n", frlToString(settings->frame_rate_limit));
				printf("1. 30\n");
				printf("2. 60\n");
				printf("3. 120\n");
				printf("4. Unlimited\n");
				printf("Choose: ");

				if (scanf("%d", &temp_frl) != 1) {
					printf("Enter a valid number.\n");
					clearBuffer();
					continue;
				}

				if (temp_frl < 1 || temp_frl > 4) {
					printf("Enter a valid number.\n");
				}
				else {
					settings->frame_rate_limit = (FRL)(temp_frl - 1);
					printf("Frame rate limit set to: %s\n", frlToString(settings->frame_rate_limit));
					break;
				}
			}
			break;
		}
		case 6: {
			int temp_difficulty;
			while (1) {
				printf("\nDIFFICULTY SETTINGS\n");
				printf("Current difficulty: %s\n", difficultyToString(settings->difficulty));
				printf("1. Easy\n");
				printf("2. Normal\n");
				printf("3. Hard\n");
				printf("4. Insane\n");
				printf("Choose: ");

				if (scanf("%d", &temp_difficulty) != 1) {
					printf("Enter a valid number.\n");
					clearBuffer();
					continue;
				}

				if (temp_difficulty < 1 || temp_difficulty > 4) {
					printf("Enter a valid number.\n");
				}
				else {
					settings->difficulty = (DIFFICULTY)(temp_difficulty - 1);
					printf("Difficulty set to: %s\n", difficultyToString(settings->difficulty));
					break;
				}
			}
			break;
		}
		case 7: {
			return;
		}
		default: {
			printf("Enter a valid number.\n");
			break;
		}
		}
	}
}

// DEFAULT GAME SETTINGS
GAME_SETTINGS defaultGameSettings(void) {
	return (GAME_SETTINGS) {
		.volume = 1,
		.graphics = GRAPHICS_LOW,
		.display_mode = DISPLAY_FULLSCREEN,
		.resolution = RESOLUTION_640X480,
		.frame_rate_limit = FRL_30,
		.difficulty = DIFFICULTY_EASY
	};
}

// EXIT PROGRAM
void exitProgram(void) {
	char exit_choice[10];

	printf("Are you sure you want to exit? [yes/no]: ");
	scanf("%9s", exit_choice);

	if (strcmp(exit_choice, "yes") == 0) {
		freeUser(&player);
		printf("Exit successful. Goodbye!\n");
		exit(0);
	}
}