/*Initialisierung von Funktionen für utilities.c*/
#pragma once
#include <Windows.h>

//Koordinaten X Y
struct koordinate {
	int x;
	int y;
};

//Bildschirmeingaben löschen
void cls();

//Grösse von Fenster festlegen
void adjustWindowSize(int lange, int breite);

//Konsolen Schrift ändern auf Quadrat > TRUE
void consoleFont(BOOL);

//Konsolen Titel setzen > char
void consoleTitle(LPCSTR textTitel);

//Start Countdown bei koordinate + Zeit bis 0
void countdown(struct koordinate, int zeit);

//Cursor in Konsole zeigen
void setCursorAttrib(BOOL);

//Setze Cursor zu Position
void gotoXY(struct koordinate);