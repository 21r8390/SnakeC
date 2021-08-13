/*snake.c fur Snake-Game*/
#include <conio.h>
#include <time.h>

#include "snake.h"
#include "printThings.h"
#include "utilities.h"

#pragma region Initialisieren ...
#define MAX_SNAKE_LENGTH 255
enum bewegung { runter = 1, rauf, links, rechts };

//Geschwindigkeit der Schlange in ms und Input Delay
int snakeSpeed = 3;

//Initialisierung von Schlange
void snakeInit();

//Initialisierung von Rand
void levelInit();

//Richtung der Schlange setzen
void setSnakeDirection();

//Move the snake in direction
void moveSnake();

//Wenn ein Punkt gegessen wird die Schlange grösser
void snakePunktDazu();

//Position von Futter neu bestimmen
void futterSetzten();

//Test ob Schlange Wand berührt
void testBeruehrung();

//Test ob etwas etwas berührt
BOOL hatBeruehrung(struct koordinate*, struct koordinate*, int, int);
#pragma endregion

void runSnake() {
	int ticks = 0; //Verzögerung
	score = 0;

	//Aussehen
	cls();
	consoleFont(TRUE);
	setCursorAttrib(FALSE);
	consoleTitle("Snake-Game     Move with WASD");
	adjustWindowSize(39, 35);

	//Inits
	snakeInit();
	levelInit();
	futterSetzten();

	//Erster Druck
	printLevel();
	printPunkte();
	printSnake();

	//Spiel starten
	countdown(schlange.kopf, 3);
	do {
		setSnakeDirection(); //->Sleep();
		ticks += 10;
		if (ticks >= 30) {
			struct koordinate letztesElementVorBewegung;
			memcpy(&letztesElementVorBewegung, &schlange.element[schlange.laenge - 1], sizeof(letztesElementVorBewegung));
			moveSnake();
			printSnakeDelta(letztesElementVorBewegung);

			testBeruehrung();
			printPunkte();
			ticks = 0;
		}
	} while (!schlange.schlangeTot);

	//Am Schluss alles wieder auf Standart setzten
	cls();
	consoleFont(FALSE);
	adjustWindowSize(30, 120);
	setCursorAttrib(TRUE);
}

void snakeInit() {
	schlange.schlangeTot = FALSE;
	schlange.nachsteRichtung = links;
	schlange.richtung = schlange.nachsteRichtung;

	//Schlange
	schlange.laenge = 4;
	for (int i = 0; i < schlange.laenge; i++) { //Schlangenelemente initialisieren
		schlange.element[i].x = 20 + i;
		schlange.element[i].y = 20;
	}
	for (int i = schlange.laenge; i < MAX_SNAKE_LENGTH; i++) { //255 MAX Grösse
		schlange.element[i].x = 0;
		schlange.element[i].y = 0;
	}
	schlange.kopf = schlange.element[0];
}

//Wieso ist das Verknüpft?

void levelInit() {
	level.masse.x = 35;
	level.masse.y = 35;
	level.anzahl = 0;
	for (int i = 0; i < level.masse.y; i++) { //linke + rechte Seite
		level.hindernis[level.anzahl].x = 0;
		level.hindernis[level.anzahl].y = i;
		level.anzahl++;
		level.hindernis[level.anzahl].x = level.masse.x;
		level.hindernis[level.anzahl].y = i;
		level.anzahl++;
	}
	for (int i = 0; i < level.masse.x + 1; i++) { //oberste + unterste Linie
		level.hindernis[level.anzahl].x = i;
		level.hindernis[level.anzahl].y = 0;
		level.anzahl++;
		level.hindernis[level.anzahl].x = i;
		level.hindernis[level.anzahl].y = level.masse.y;
		level.anzahl++;
	}
}

void setSnakeDirection() { //Test ob eine Taste gerückt wird / Mit ESC Spiel beenden
	if (_kbhit()) {
		switch (_getch()) {
		case 'W':
		case 'w':
			if (schlange.richtung != runter) {
				schlange.nachsteRichtung = rauf;
			}
			break;
		case 'A':
		case 'a':
			if (schlange.richtung != rechts) {
				schlange.nachsteRichtung = links;
			}
			break;
		case 'S':
		case 's':
			if (schlange.richtung != rauf) {
				schlange.nachsteRichtung = runter;
			}
			break;
		case 'D':
		case 'd':
			if (schlange.richtung != links) {
				schlange.nachsteRichtung = rechts;
			}
			break;
		case VK_ESCAPE:
			schlange.schlangeTot = TRUE;
			break;
		}
	}
	Sleep(snakeSpeed); //wait
}

void moveSnake() { //Schlange bewegen in richtung, wenn keine anderung dann in die selbe Richtung
	schlange.richtung = schlange.nachsteRichtung;
	schlange.kopf = schlange.element[0];
	switch (schlange.richtung) {
	case runter: schlange.kopf.y++;	break;
	case rauf: schlange.kopf.y--;		break;
	case links: schlange.kopf.x--;		break;
	case rechts: schlange.kopf.x++;	break;
	}
	for (int i = schlange.laenge - 1; i >= 1; i--) { //Elemente nachziehen
		schlange.element[i].x = schlange.element[i - 1].x;
		schlange.element[i].y = schlange.element[i - 1].y;
	}

	schlange.element[0] = schlange.kopf;
}

void snakePunktDazu() {
	schlange.element[schlange.laenge].x = schlange.element[schlange.laenge - 1].x;
	schlange.element[schlange.laenge].y = schlange.element[schlange.laenge - 1].y;
	schlange.laenge++;
}

BOOL hatBeruehrung(struct koordinate* hinderniss, struct koordinate* zuTesten, int anzahlTests, int startValue) {
	BOOL hatTreffer = FALSE;

	for (int i = startValue; i < anzahlTests; i++) {
		if (hinderniss[i].x == zuTesten->x && hinderniss[i].y == zuTesten->y)
			hatTreffer = TRUE;
	}
	return hatTreffer;
}

void testBeruehrung() {

	//Hinderniss
	schlange.schlangeTot = hatBeruehrung(&level.hindernis[0], &schlange.kopf, level.anzahl, 0);

	//Punkte
	if (hatBeruehrung(&schlange.kopf, &level.futter, 1, 0)) {
		score++;
		snakePunktDazu();
		futterSetzten();
		printPunkte();
	}

	//Schlange
	if (!schlange.schlangeTot)
		schlange.schlangeTot = hatBeruehrung(&schlange.element[0], &schlange.kopf, level.anzahl, 1);

	if (schlange.schlangeTot) {
		level.futter.x = 0;
		level.futter.y = 0;
	}
}

void futterSetzten() {
	BOOL gelungen = FALSE;

	//Position herausfinden
	srand((unsigned)time(NULL));//nicht zu oft aufrufen !> sehr langsam
	do {

		//random generate Number
		if (level.masse.x > 1 && level.masse.y > 1) {
			level.futter.x = rand() % (level.masse.x - 1) + 1;
			level.futter.y = rand() % (level.masse.y - 1) + 1;
		}

		//Test ob erstellen Gelungen
		gelungen = !hatBeruehrung(&schlange.element[0], &level.futter, schlange.laenge, 0) && !hatBeruehrung(&level.hindernis[0], &level.futter, level.anzahl, 0);
	} while (!gelungen);
}

/*

			if (durchlauf>= 1000) {

				//cls();
				adjustWindowSize(39, 35);
				consoleFont(TRUE);
				setCursorAttrib(FALSE);
				printLevel();
				printSnake();
				durchlauf = 0;
			}

*/
