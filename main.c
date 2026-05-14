#include "funkcije.h"

int main(void) {

	USER player;

	// USER CREATION

	createUser(&player);

	// LOG IN

	while (loginUser(&player) == false);

	// MAIN MENU

	int menu_choice;
	int volume = 1;

	while (1) {

		printf("\nMAIN MENU\n");
		printf("1. User settings\n");
		printf("2. Game settings\n");
		printf("3. Exit\n");
		printf("Choose: ");

		if (scanf("%d", &menu_choice) != 1) {
			printf("Enter a valid number.\n");
			while (getchar() != '\n'); continue;
			continue;
		}

		switch (menu_choice) {
		case 1: {
			printf("\nUSER SETTINGS\n");
			break;
		}

		case 2: {
			int game_settings_choice;
			int game_settings_running = 1;

			while (game_settings_running) {
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
					while (getchar() != '\n'); continue;
					continue;
				}
				switch (game_settings_choice) {
				case 1: {
					int temp_volume;
					while (1) {
						printf("\nCurrent volume: %d\n", volume);
						printf("Enter desired volume [1-10]: ");
						if (scanf("%d", &temp_volume) != 1) {
							printf("Enter a valid number.\n");
							while (getchar() != '\n'); continue;
							continue;
						}
						if (temp_volume < 1 || temp_volume > 10) {
							printf("Enter a valid number.\n");
						}
						else {
							volume = temp_volume;
							printf("Successfully modified volume to: %d\n", volume);
							break;
						}
					}
					break;
				}
				case 2: {
					printf("Graphics settings\n");
					break;
				}
				case 3: {
					printf("Display mode settings\n");
					break;
				}
				case 4: {
					printf("Resolution settings\n");
					break;
				}
				case 5: {
					printf("Frame rate limit settings\n");
					break;
				}
				case 6: {
					printf("Difficulty settings\n");
					break;
				}
				case 7: {
					game_settings_running = 0;
					break;
				}
				default: {
					printf("Enter a valid number.\n");
					break;
				}
				}
			}
			break;
		}

		case 3: {
			char exit_choice[10];

			printf("Are you sure you want to exit? [yes/no]: ");
			scanf("%9s", exit_choice);

			if (strcmp(exit_choice, "yes") == 0) {
				printf("Exit succesfull. Goodbye!");
				return 0;
			}
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