/*snake.c fur Snake-Game*/
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "utilities.h"

//Highscore struct
struct highscore {
	int punkte;
	char name[15];
	char timestamp[40];
};

char directory[MAX_PATH - sizeof("highscore.txt")];

void readHighscore(struct highscore*, int);
BOOL printHighscore(struct highscore*);
void showHighscore();

void readHighscore(struct highscore* highscore, int score) {
	FILE* saveLog;

	//Pfad setzen für Highscore
	GetCurrentDirectoryA(sizeof(directory), directory);
	strcat_s(directory, sizeof(directory), "\\highscore.txt");

	//Damit man es von Extern ohne Hinzufügen öffenen kann
	if (score > 0) {
		char name[9] = { 0 }, timeC[30], buffer[BUFSIZ] = { 0 };

		//neue Eingabe einlesen
		time_t timeNow = time(NULL); //damit echtzeit genommen wird
		ctime_s(timeC, sizeof(timeC), &timeNow); //Timestamp
		timeC[strcspn(timeC, "\n")] = 0; //damit kein Enter eingelesen wird

		do {
			cls();

			printf("\nWie heisst du?\n> ");
			fgets(buffer, sizeof(buffer), stdin); //da bei Scanf ein Overflow -> Bei Overflow von fgets wird das nächste Mal übersprungen

			if (strlen(buffer) < sizeof(name) && buffer[0] != '\n') {
				strcpy_s(name, sizeof(name), buffer); //buffer in name einfügen
				name[strcspn(name, "\n")] = 0; //Damit '\0' gesetzt wird und Enter nicht eingelesen
			}
			else if(buffer[0] != '\n') { //Zu viele Zeichen eingegeben
				for (int i = 0; i < 9; i++)
					name[i] = buffer[i];
			}
			name[8] = 0; //=> '\0'
		} while (name[0] == 0); //damit nicht nur Enter
		
		highscore[10].punkte = score;
		strcpy_s(highscore[10].name, sizeof(highscore[10].name), name);
		strcpy_s(highscore[10].timestamp, sizeof(highscore[10].timestamp), timeC);
	}
	else {
		//Damit sicher nicht gespeichert und wenn dann als ungültiges
		highscore[10].punkte = -1;
		strcpy_s(highscore[10].name, sizeof(highscore[10].name), "<Name>");
		strcpy_s(highscore[10].timestamp, sizeof(highscore[10].timestamp), "<Datum>");
	}

	//Restliche Resultate einlesen aus Datei
	if (fopen_s(&saveLog, directory, "r+t") == 0) {//write + text
		int durchlauf = 0;
		char lineBuffer[255], test[255] = { 0 };
		char* token = NULL, * nextToken = NULL;

		while (durchlauf < 10) {
			fgets(lineBuffer, sizeof(lineBuffer), saveLog); //Zeile holen...
			//Überprüfen ob Ende der Datei
			if (!feof(saveLog)) {
				token = strtok_s(lineBuffer, ";", &nextToken);
				highscore[durchlauf].punkte = atoi(token); // Name verarbeiten...
			}
			if (!feof(saveLog)) {
				token = strtok_s(NULL, ";", &nextToken);
				strcpy_s(highscore[durchlauf].name, sizeof(highscore[durchlauf].name), token); // Punktezahl verarbeiten...
			}
			if (!feof(saveLog)) {
				token = strtok_s(NULL, ";", &nextToken);
				strcpy_s(highscore[durchlauf].timestamp, sizeof(highscore[durchlauf].timestamp), token); // Timestamp verarbeiten...
			}

			durchlauf++;
		}
		fclose(saveLog);
	}
}

BOOL printHighscore(struct highscore* highscore) { 	//Score einlesen > Top10 drucken + Datei erstellen
	FILE* saveLog;

	if ((fopen_s(&saveLog, directory, "w+t")) == 0 && saveLog) {//write + text
		int anzahl = 0, i = 0;
		BOOL eingesetzt = FALSE;

		//Resultat einfügen
		cls();
		printf("\n\n\n\n%24c Highscoreliste:\n\n\n\n", 0);
		while (anzahl < 10) {
			if ((highscore[i].punkte < highscore[10].punkte) && !eingesetzt) {
				fprintf_s(saveLog, "%i;%s;%s;\n", highscore[10].punkte, &highscore[10].name, &highscore[10].timestamp);
				printf("\t   %4i %11s %28s\n\n", highscore[10].punkte, &highscore[10].name, &highscore[10].timestamp); //in Konsole anzeigen
				eingesetzt = TRUE; //resultat nur einmal einfügen
				anzahl++;
			}

			if (highscore[i].punkte > 0) {
				if (anzahl < 10) {
					fprintf_s(saveLog, "%i;%s;%s;\n", highscore[i].punkte, &highscore[i].name, &highscore[i].timestamp);
					printf("\t   %4i %11s %28s\n\n", highscore[i].punkte, &highscore[i].name, &highscore[i].timestamp);
				}
			}
			else {
				printf("\n\n");
				fprintf_s(saveLog, "-1;<Name>;<Datum>;\n");
			}
			i++;
			anzahl++;
		}
		fflush(saveLog); //damit sofort eingeschrieben und nicht in Buffer
		fclose(saveLog);

		printf("\n\n%22c Erneut Spielen? (y/n)\n", 0);
		fflush(stdin);
		if (highscore[10].punkte > 0) { //In showHighscore nich abfragen
			if (_getch() == 'y')
				return TRUE;
			else
				return FALSE;
		}
		else
			return 0;//nichts zurückgeben, da nichts damit gemacht wird
	}
	else {
		printf("\n\nFehler beim Oeffnen/Erstellen der Datei mit W+T\n %s\n", directory);
		Sleep(4000);
		return FALSE;
	}
}

int frageNochmals(int score) {
	struct highscore highscore[11] = { 0 };
	int antwort = 0;
	
	adjustWindowSize(36, 67);

	printf("\n\n\n\t\t\t  Du bist gestorben\n\t\t\t      Punkte: %i\n\n\n\t\t\tErneut Spielen? (y/n)\n", score);
	if (score > 0)
		printf("\n\t\t     Highscore mit (h) speichern\n\n");
	else
		printf("\n\t\t     Highscore mit (h) anzeigen\n\n");

	while (antwort == 0) {
		if (_kbhit()) {
			switch (_getch()) {
			case 'y':case 'Y':
				antwort = 1;
				break;
			case 'h':case 'H':
				if (score > 0) {
					readHighscore(highscore, score); //Ink. neues Resultat
					if (printHighscore(highscore))
						antwort = 1;
					else antwort = 2;
				}
				else
					showHighscore();
				break;
			case 'n':case 'N':
				antwort = 2;
				break;
			}
		}
	}
	return antwort == 1;
}

void showHighscore() {
	struct highscore highscore[11] = { 0 };
	readHighscore(highscore, 0);
	printHighscore(highscore);
}