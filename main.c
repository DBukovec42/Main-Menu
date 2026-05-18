#include "header.h"

USER player;
GAME_SETTINGS settings;

int main(void) {

	player.username = NULL;
	player.password = NULL;
	settings = defaultGameSettings();

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

	// MAIN MENU

	int menu_choice;

	while (1) {

		printf("\nMAIN MENU\n");
		printf("1. User settings\n");
		printf("2. Game settings\n");
		printf("3. Exit\n");
		printf("Choose: ");

		if (scanf("%d", &menu_choice) != 1) {
			printf("Enter a valid number.\n");
			clearBuffer();
			continue;
		}

		switch (menu_choice) {
		case SETTINGS_USER: {
			printf("\nUSER SETTINGS\n");
			break;
		}

		case SETTINGS_GAME: {
			gameSettingsMenu(&settings);
			break;
		}

		case EXIT: {
			exitProgram();
			break;
		}
		default: {
			printf("Enter a valid number.\n");
			break;
			}
		}
	}
	return 0;
}