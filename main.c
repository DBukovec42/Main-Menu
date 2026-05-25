#include "header.h"

USER player;
SORT_ORDER current_sort = SORT_BY_ID;

int main(void) {

	while (1) {

		runLogin();

		bool logged_out = false;

		while (!logged_out) {
			int menu_choice;

			printf("\nMAIN MENU\n");
			printf("1. User Settings\n");
			printf("2. Game Settings\n");
			printf("3. Exit\n");
			printf("Choose: ");

			if (scanf("%d", &menu_choice) != 1) {
				printf("Enter a valid number.\n");
				clearBuffer();
				continue;
			}

			switch (menu_choice) {
			case SETTINGS_USER: {
				logged_out = userSettingsMenu();
				break;
			}

			case SETTINGS_GAME: {
				gameSettingsMenu(&player.game_settings);
				saveUserSettings(&player);
				break;
			}

			case EXIT_MENU: {
				exitProgram();
				break;
			}
			default: {
				printf("Enter a valid number.\n");
				break;
			}
			}
		}
	}
	return 0;
}