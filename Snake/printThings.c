/*utilities.c fur Snake-Game*/
#include <stdlib.h> //rand()
#include <stdio.h> //printf()

#include "snake.h" //structs + initialisierung
#include "utilities.h" //gotoXY etc


//Level
void printLevel() {	
	for (int i = 0; i < level.anzahl; i++) {
		gotoXY(level.hindernis[i]);
		printf("°");//Mauer zeichen mit diesem Zeichen
	}
}


//Punkte
void printPunkte() {
	gotoXY(level.futter);//Punkt setzen
	printf("@");
	
	struct koordinate punktePos = { level.masse.x / 4, level.masse.y + 2 };
	gotoXY(punktePos);//Punktestand anzeigen
	printf("Punktzahl: %i", score);
}


//Snake
void printSnake() { //Schlange zum ersten mal Drucken
	for (int i = 1; i < schlange.laenge; i++) {
		gotoXY(schlange.element[i]);
		printf("+");
	}
	gotoXY(schlange.kopf);
	printf("0");
}

void printSnakeDelta(struct koordinate letztesElementVorBewegung) { //Schlange drucken + hinterstes löschen
	gotoXY(letztesElementVorBewegung);
	printf(" ");//hinterstes Zeichen löschen
	gotoXY(schlange.kopf);
	printf("0");//Kopf der Schlange	
	gotoXY(schlange.element[1]);
	printf("+");//Kopf der Schlange	
}