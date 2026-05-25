#include "header.h"

// RUN LOGIN
void runLogin(void) {
	while (1) {
		startupMenu(&player);
		int login_choice;

		do {
			login_choice = loginUser(&player);
		} while (login_choice == 0);

		if (login_choice == 1) {
			break;
		}
	}
}

// USERNAME TAKEN
static bool usernameTaken(const char* username) {
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		return false;
	}

	char existing_username[20];
	char existing_password[20];
	int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;

	while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, existing_username, existing_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
		if (strcmp(existing_username, username) == 0) {
			fclose(file);
			return true;
		}
	}

	fclose(file);

	return false;
}

// USER ID
static int userId(void) {
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		return 1;
	}

	int max_id = 0;
	int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;
	char file_username[20], file_password[20];

	while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
		if (id > max_id) max_id = id;
	}

	fclose(file);
	return max_id + 1;
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

	user->id = userId();
	user->sort_order = SORT_BY_ID;
	user->game_settings = defaultGameSettings();

	return true;
}

// SAVE USER
static bool saveUser(const USER* user) {
	FILE* file = fopen(USER_FILE, "a");

	if (!file) {
		perror("fopen");
		return false;
	}

	bool success = fprintf(file, "%d %s %s %d %d %d %d %d %d %d\n",
		user->id,
		user->username,
		user->password,
		user->game_settings.volume,
		(int)user->game_settings.graphics,
		(int)user->game_settings.display_mode,
		(int)user->game_settings.resolution,
		(int)user->game_settings.frame_rate_limit,
		(int)user->game_settings.difficulty,
		(int)user->sort_order) > 0;

	fclose(file);

	if (!success) {
		printf("Could not save user data.\n");
	}

	return success;
}

// SAVE USER SETTINGS
bool saveUserSettings(const USER* user) {
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		perror("fopen");
		return false;
	}

	FILE* temp = fopen("temp.txt", "w");

	if (!temp) {
		fclose(file);
		perror("fopen");
		return false;
	}

	char file_username[20];
	char file_password[20];
	int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;

	while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {

		if (strcmp(file_username, user->username) == 0) {
			fprintf(temp, "%d %s %s %d %d %d %d %d %d %d\n",
				user->id,
				user->username,
				user->password,
				user->game_settings.volume,
				(int)user->game_settings.graphics,
				(int)user->game_settings.display_mode,
				(int)user->game_settings.resolution,
				(int)user->game_settings.frame_rate_limit,
				(int)user->game_settings.difficulty,
				(int)user->sort_order);
		}
		else {
			fprintf(temp, "%d %s %s %d %d %d %d %d %d %d\n", id, file_username, file_password, volume, graphics, display_mode, resolution, frl, difficulty, sort_order);
		}
	}

	fclose(file);
	fclose(temp);

	if (remove(USER_FILE) != 0 || rename("temp.txt", USER_FILE) != 0) {
		printf("Failed to update user file.\n");
		return false;
	}

	return true;
}

// LOAD USER
static bool loadUser(USER* user, const char* username) {
	freeUser(user);
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		return false;
	}

	char file_username[20];
	char file_password[20];
	int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;

	while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
		if (strcmp(file_username, username) == 0) {
			user->username = malloc(strlen(file_username) + 1);

			if (!user->username) { 
				fclose(file); 
				return false; 
			}

			strcpy(user->username, file_username);

			user->password = malloc(strlen(file_password) + 1);

			if (!user->password) { 
				fclose(file); 
				free(user->username); 
				user->username = NULL; 
				return false; 
			}

			strcpy(user->password, file_password);

			user->id = id;
			user->game_settings.volume = volume;
			user->game_settings.graphics = (GRAPHICS)graphics;
			user->game_settings.display_mode = (DISPLAY)display_mode;
			user->game_settings.resolution = (RESOLUTION)resolution;
			user->game_settings.frame_rate_limit = (FRL)frl;
			user->game_settings.difficulty = (DIFFICULTY)difficulty;
			user->sort_order = (SORT_ORDER)sort_order;

			fclose(file);
			return true;
		}
	}

	fclose(file);
	return false;
}

// LOG IN
int loginUser(USER* user) {
	char entered_username[20];
	char entered_password[20];

	printf("\nLOG IN\n");
	printf("Enter username (or type exit to go back): ");
	scanf("%19s", entered_username);
	if (strcmp(entered_username, "exit") == 0) {
		return -1;
	}

	bool user_found = loadUser(user, entered_username);

	printf("Enter password: ");
	scanf("%19s", entered_password);

	if (user_found && strcmp(user->password, entered_password) == 0) {
		current_sort = user->sort_order;
		printf("Welcome!\n");
		return 1;
	}

	printf("Incorrect username or password.\n");
	freeUser(user);
	return 0;
}

// CLEANUP
void freeUser(USER* user) {
	free(user->username);
	free(user->password);
	user->username = NULL;
	user->password = NULL;
	user->id = 0;
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
			break;
		}

		else if (startup_choice == 2) {
			if (createUser(user) && saveUser(user)) {
				printf("New user successfully created!\n");
				continue;
			}
		}
		else {
			printf("Enter a valid number.\n");
		}
	}
}

// COMPARATORS
static int compareById(const void* a, const void* b) {
	return ((ENTRY*)a)->id - ((ENTRY*)b)->id;
}

static int compareByUsername(const void* a, const void* b) {
	return strcmp(((ENTRY*)a)->username, ((ENTRY*)b)->username);
}

// USER LIST
static void listUsers(void) {
	FILE* file = fopen(USER_FILE, "r");

	if (!file) {
		perror("fopen");
		return;
	}

	int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;
	char file_username[20], file_password[20];
	int count = 0;

	while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
		count++;
	}

	if (count == 0) {
		printf("No users found.\n");
		fclose(file);
		return;
	}

	ENTRY* entries = malloc(count * sizeof(ENTRY));

	if (!entries) {
		printf("Memory allocation failed.\n");
		fclose(file);
		return;
	}

	rewind(file);
	int i = 0;

	while (i < count && fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &entries[i].id, entries[i].username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
		i++;
	}

	fclose(file);

	qsort(entries, count, sizeof(ENTRY), current_sort == SORT_BY_ID ? compareById : compareByUsername);

	printf("\nUSER LIST\n");

	for (int a = 0; a < count; a++) {
		printf("ID: %d | Username: %s\n", entries[a].id, entries[a].username);
	}

	free(entries);
}

// BINARY SEARCH (recursion)
static int binarySearchById(ENTRY* entries, int left, int right, int target_id) {
	if (left > right) {
		return -1;
	}

	int mid = left + (right - left) / 2;

	if (entries[mid].id == target_id) {
		return mid;
	}

	if (entries[mid].id < target_id) {
		return binarySearchById(entries, mid + 1, right, target_id);
	}
	else {
		return binarySearchById(entries, left, mid - 1, target_id);
	}
}

// SEARCH USERS
static void searchUsers(void) {
	int search_choice;

	while (1) {
		printf("\nSEARCH USERS\n");
		printf("1. Search By ID\n");
		printf("2. Search By Username\n");
		printf("3. Go Back\n");
		printf("Choose: ");

		if (scanf("%d", &search_choice) != 1) {
			printf("Enter a valid number.\n");
			clearBuffer();
			continue;
		}

		if (search_choice == 3) {
			break;
		}

		if (search_choice != 1 && search_choice != 2) {
			printf("Enter a valid number.\n");
			continue;
		}

		FILE* file = fopen(USER_FILE, "r");

		if (!file) {
			perror("fopen");
			break;
		}

		int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;
		char file_username[20], file_password[20];
		int count = 0;

		while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
			count++;
		}

		if (count == 0) {
			printf("No users found.\n");
			fclose(file);
			continue;
		}

		ENTRY* entries = malloc(count * sizeof(ENTRY));

		if (!entries) {
			printf("Memory allocation failed.\n");
			fclose(file);
			continue;
		}

		rewind(file);
		int i = 0;

		while (i < count && fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &entries[i].id, entries[i].username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
			i++;
		}

		fclose(file);

		if (search_choice == 1) {
			int search_id;
			printf("Enter ID: ");

			if (scanf("%d", &search_id) != 1) {
				printf("Enter a valid number.\n");
				clearBuffer();
				free(entries);
				continue;
			}

			qsort(entries, count, sizeof(ENTRY), compareById);

			int result = binarySearchById(entries, 0, count - 1, search_id);

			if (result != -1) {
				printf("\nID: %d | Username: %s\n", entries[result].id, entries[result].username);
			}
			else {
				printf("No users found.\n");
			}
		}
		else {
			char search_username[20];
			printf("Enter Username: ");
			scanf("%19s", search_username);

			bool found = false;

			for (int a = 0; a < count; a++) {
				if (strcmp(entries[a].username, search_username) == 0) {
					printf("\nID: %d | Username: %s\n", entries[a].id, entries[a].username);
					found = true;
					break;
				}
			}

			if (!found) {
				printf("No users found.\n");
			}
		}

		free(entries);
	}
}

// SORT USERS
void sortUsers(void) {
	int sort_choice;

	while (1) {
		printf("\nSORT USERS\n");
		printf("1. Sort By ID\n");
		printf("2. Sort By Username\n");
		printf("3. Go Back\n");
		printf("Choose: ");

		if (scanf("%d", &sort_choice) != 1) {
			printf("Enter a valid number.\n");
			clearBuffer();
			continue;
		}

		if (sort_choice == 1) {
			current_sort = SORT_BY_ID;
			player.sort_order = SORT_BY_ID;
			saveUserSettings(&player);
			printf("Successfully sorted by ID.\n");
			break;
		}
		else if (sort_choice == 2) {
			current_sort = SORT_BY_USERNAME;
			player.sort_order = SORT_BY_USERNAME;
			saveUserSettings(&player);
			printf("Successfully sorted by username.\n");
			break;
		}
		else if (sort_choice == 3) {
			break;
		}
		else {
			printf("Enter a valid number.\n");
		}
	}
}

// DELETE USER
static bool deleteUser(void) {
	char entered_username[20];
	char entered_password[20];
	char delete_choice[10];
	char file_username[20];
	char file_password[20];
	int id, volume, graphics, display_mode, resolution, frl, difficulty, sort_order;

	printf("\nDELETE USER\n");

	while (1) {
		printf("Enter username you wish to delete (or type exit to go back): ");
		scanf("%19s", entered_username);

		if (strcmp(entered_username, "exit") == 0) {
			return false;
		}

		if (!usernameTaken(entered_username)) {
			printf("User not found.\n");
			continue;
		}

		printf("Confirm password: ");
		scanf("%19s", entered_password);

		FILE* file = fopen(USER_FILE, "r");

		if (!file) {
			perror("fopen");
			return false;
		}

		bool found = false;

		while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
			if (strcmp(file_username, entered_username) == 0 && strcmp(file_password, entered_password) == 0) {
				found = true;
				break;
			}
		}

		fclose(file);

		if (!found) {
			printf("Incorrect username or password.\n");
			continue;
		}

		printf("Are you sure you want to delete user %s? [yes/no]: ", entered_username);
		scanf("%9s", delete_choice);

		if (strcmp(delete_choice, "yes") != 0) {
			continue;
		}

		file = fopen(USER_FILE, "r");

		if (!file) {
			perror("fopen");
			return false;
		}

		FILE* temp = fopen("temp.txt", "w");

		if (!temp) {
			fclose(file);
			perror("fopen");
			return false;
		}

		while (fscanf(file, "%d%19s%19s%d%d%d%d%d%d%d", &id, file_username, file_password, &volume, &graphics, &display_mode, &resolution, &frl, &difficulty, &sort_order) == 10) {
			if (strcmp(file_username, entered_username) == 0) {
				continue;
			}

			fprintf(temp, "%d %s %s %d %d %d %d %d %d %d\n", id, file_username, file_password, volume, graphics, display_mode, resolution, frl, difficulty, sort_order);
		}

		fclose(file);
		fclose(temp);

		if (remove(USER_FILE) != 0 || rename("temp.txt", USER_FILE) != 0) {
			printf("Failed to delete user.\n");
			return false;
		}

		printf("User %s deleted successfully.\n", entered_username);

		if (strcmp(entered_username, player.username) == 0) {
			freeUser(&player);
			return true;
		}

		return false;
	}
}

// USER SETTINGS
bool userSettingsMenu(void) {
	int user_settings_choice;

	while (1) {
		printf("\nUSER SETTINGS\n");
		printf("Current user: %s\n", player.username);
		printf("1. User List\n");
		printf("2. Search Users\n");
		printf("3. Sort Users\n");
		printf("4. Delete Users\n");
		printf("5. Logout\n");
		printf("6. Go Back\n");
		printf("Choose: ");

		if (scanf("%d", &user_settings_choice) != 1) {
			printf("Enter a valid number.\n");
			clearBuffer();
			continue;
		}

		switch (user_settings_choice) {
		case 1: {
			listUsers();
			break;
		}
		case 2: {
			searchUsers();
			break;
		}
		case 3: {
			sortUsers();
			break;
		}
		case 4: {
			if (deleteUser()) {
				return true;
			}
			break;
		}
		case 5: {
			char logout_choice[10];

			printf("Are you sure you want to logout? [yes/no]: ");
			scanf("%9s", logout_choice);

			if (strcmp(logout_choice, "yes") == 0) {
				freeUser(&player);
				printf("Logout successful. Goodbye!\n");
				return true;
			}
			break;
		}
		case 6: {
			return false;
		}
		default: {
			printf("Enter a valid number.\n");
			break;
		}
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
void gameSettingsMenu(GAME_SETTINGS* game_settings) {
	int game_settings_choice;

	while (1) {
		printf("\nGAME SETTINGS\n");
		printf("1. Volume\n");
		printf("2. Graphics\n");
		printf("3. Display Mode\n");
		printf("4. Resolution\n");
		printf("5. Frame Rate Limit\n");
		printf("6. Difficulty\n");
		printf("7. Go Back\n");
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
				printf("Current volume: %d\n", game_settings->volume);
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
					game_settings->volume = temp_volume;
					printf("Volume set to: %d\n", game_settings->volume);
					break;
				}
			}
			break;
		}
		case 2: {
			int temp_graphics;
			while (1) {
				printf("\nGRAPHICS SETTINGS\n");
				printf("Current graphics: %s\n", graphicsToString(game_settings->graphics));
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
					game_settings->graphics = (GRAPHICS)(temp_graphics - 1);
					printf("Graphics set to: %s\n", graphicsToString(game_settings->graphics));
					break;
				}
			}
			break;
		}
		case 3: {
			int temp_display_mode;
			while (1) {
				printf("\nDISPLAY MODE SETTINGS\n");
				printf("Current display mode: %s\n", displayModeToString(game_settings->display_mode));
				printf("1. Fullscreen\n");
				printf("2. Windowed\n");
				printf("3. Borderless Window\n");
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
					game_settings->display_mode = (DISPLAY)(temp_display_mode - 1);
					printf("Display mode set to: %s\n", displayModeToString(game_settings->display_mode));
					break;
				}
			}
			break;
		}
		case 4: {
			int temp_resolution;
			while (1) {
				printf("\nRESOLUTION SETTINGS\n");
				printf("Current resolution: %s\n", resolutionToString(game_settings->resolution));
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
					game_settings->resolution = (RESOLUTION)(temp_resolution - 1);
					printf("Resolution set to: %s\n", resolutionToString(game_settings->resolution));
					break;
				}
			}
			break;
		}
		case 5: {
			int temp_frl;
			while (1) {
				printf("\nFRAME RATE LIMIT SETTINGS\n");
				printf("Current frame rate limit: %s\n", frlToString(game_settings->frame_rate_limit));
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
					game_settings->frame_rate_limit = (FRL)(temp_frl - 1);
					printf("Frame rate limit set to: %s\n", frlToString(game_settings->frame_rate_limit));
					break;
				}
			}
			break;
		}
		case 6: {
			int temp_difficulty;
			while (1) {
				printf("\nDIFFICULTY SETTINGS\n");
				printf("Current difficulty: %s\n", difficultyToString(game_settings->difficulty));
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
					game_settings->difficulty = (DIFFICULTY)(temp_difficulty - 1);
					printf("Difficulty set to: %s\n", difficultyToString(game_settings->difficulty));
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