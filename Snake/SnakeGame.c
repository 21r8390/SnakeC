// Snake.c 	von Manuel Schumacher

//Headers
#include "snake.h"
#include "highscore.h"

int main() {
	do{
		//Spiel starten
		runSnake();
	} while (frageNochmals(score));
	return 0;
}