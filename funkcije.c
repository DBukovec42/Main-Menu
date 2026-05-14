#include "funkcije.h"

void createUser(USER* u) {
	printf("USER CREATION\n");
	printf("Choose a new username: ");
	scanf("%19s", u->username);
	printf("Choose a new password: ");
	scanf("%19s", u->password);
}

bool loginUser(USER* u) {
	printf("\nLOG IN\n");
	printf("Enter username: ");
	scanf("%19s", u->entered_username);
	printf("Enter password: ");
	scanf("%19s", u->entered_password);

	if (strcmp(u->username, u->entered_username) == 0 && strcmp(u->password, u->entered_password) == 0) {
		printf("Welcome!\n");
		return true;
	}
	else {
		printf("Incorrect username or password.\n");
		return false;
	}
}