/*Initialisierung von Funktionen für snake.c*/
#pragma once
#include <Windows.h>
#include "utilities.h"

//Level struct
struct level {
	struct koordinate masse;//x = höhe y = breite
	int anzahl;
	struct koordinate hindernis[255];
	struct koordinate futter;
};
struct level level;

//Schlange struct
struct schlange {
	int laenge;
	int richtung;
	int nachsteRichtung;
	struct koordinate kopf;
	struct koordinate element[255];
	BOOL schlangeTot;
};
struct schlange schlange;


//Geschwindigkeit der Schlange in ms und Input Delay
int snakeSpeed;

//Aktueller Punktestand
int score;


//Spiel starten
void runSnake();