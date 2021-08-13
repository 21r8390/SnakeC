/*utilities.c fur Snake-Game*/
#include <Windows.h> //TRUE & FALSE
#include <stdio.h> //printf
#include "snake.h"

int errorHanlder = TRUE;

void gotoXY(struct koordinate);

void cls() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) //aktueller Buffer auslesen
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
		return;
	SetConsoleCursorPosition(hConsole, coordScreen);
}

void adjustWindowSize(int lange, int breite) { //Grosse von Fenster festlegen
	/*
	char befehl[30] = { 0 };
	strcat_s(befehl, sizeof(befehl), "mode con lines=");
	strcat_s(befehl, sizeof(befehl), lange);
	strcat_s(befehl, sizeof(befehl), "cols=");
	strcat_s(befehl, sizeof(befehl), breite);
	*/

	if (errorHanlder) {
		//Da sonst Buffer voll ist und nicht ändert
		errorHanlder = FALSE;
		adjustWindowSize(250, 250);
		errorHanlder = TRUE;
	}

	for (int i = 0; i < 3; i++) {
		SMALL_RECT sr_window = { 0, 0, breite, lange };
		COORD extension = { breite + 1, lange + 1 };
		HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &sr_window); //damit Buffer weggeht
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), extension); //scrollbar
		SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &sr_window); //grosse + scrollbar komplett werg
	}
}

void consoleFont(BOOL quadrat) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi = { 0 };
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	
	if (quadrat) {
		cfi.dwFontSize.Y = 15;
		cfi.dwFontSize.X = cfi.dwFontSize.Y;
	}
	else {
		cfi.dwFontSize.Y = 16;
		cfi.dwFontSize.X = 8;
	}

	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, _countof(cfi.FaceName), L"Consolas");
	SetCurrentConsoleFontEx(consoleHandle, TRUE, &cfi);
}

void consoleTitle(LPCSTR textTitel) {
	SetConsoleTitleA(textTitel);
}

void setCursorAttrib(BOOL zustand) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = zustand;
	SetConsoleCursorInfo(consoleHandle, &info); //hide Cursor
}

void countdown(struct koordinate koordinate, int zeit) {
	koordinate.y -= 2;
	gotoXY(koordinate);

	for (int i = zeit; i > 0; i--) {
		printf("%i\b", i);
		Sleep(1000);
	}
	printf("Go!");
	Sleep(800);
	printf("\b\b\b\b     ");
}

void gotoXY(struct koordinate koordinate) {
	COORD coord;
	coord.X = koordinate.x;
	coord.Y = koordinate.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}