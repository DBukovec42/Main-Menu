#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
	char username[20];
	char password[20];
	char entered_username[20];
	char entered_password[20];
} USER;

void createUser(USER* u);
bool loginUser(USER* u);