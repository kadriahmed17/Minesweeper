
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
/* Board entries */

#define NO_ENTRY -1
#define MINE 0
#define OPEN 1 // by player
#define FREED 2 // by consequence

/* Status entries */

#define SHOW 11
#define DONT_SHOW -11
#define SHOW_M_SURROUNDING 10
#define MARKED 1

/* Game signals */

#define GAME_WON 111
#define GAME_LOST 110
#define GAME_ONGOING 112

const char *help =
		"This game supports the following instructions:\n\
Mark: to mark a cell\n\
Open: to open a cell\n\
Exit: to abandon game\n\
~>  \0";

// Funktionsname: build_field
// Kurzbeschreibung der Aufgabe der Funktion : Spielfeld initialisieren
// Eingabeparameter mit zulässigem Wertebereich: board , status ,rows , cols
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 03/12/2020
void build_field(int** board, int** status, int rows, int cols) {
	int i = 0; //initialisierung und deklarierung
	int j = 0;
	//Spielfeld initialisieren
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			board[i][j] = NO_ENTRY;
			status[i][j] = DONT_SHOW;
		}
	}
}
// Funktionsname: randomize_n_mines
// Kurzbeschreibung der Aufgabe der Funktion : Zufaellige Anzahl von Minen bekommen, die insgesamt zwischen 10% und 25% des Spielfeldes belegen sein sollen
// Eingabeparameter mit zulässigem Wertebereich: n_mines ,rows ,cols
// Rückgabeparameter mit zulässigem Wertebereich: n_mines
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 03/12/2020
void randomize_n_mines(int* n_mines, int rows, int cols) {
	int min = 0;
	int max = 0;
	srand(time(NULL));
	min = (rows * cols) / 10; //Minimum Anzahl von Minen die 10% des Spielfeldes belegen
	max = (rows * cols) / 4; //Maximum Anzahl von Minen 25% des Spielfeldes belegen
	*n_mines = min + rand() % (max - min + 1); //ZUfaellige Anzahl von Minen bekommen
}
// Funktionsname: build_mines
// Kurzbeschreibung der Aufgabe der Funktion : Die Minen werden in Zufaellige Feldern aufgeteilt
// Eingabeparameter mit zulässigem Wertebereich: board ,rows ,cols ,n_mines
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 03/12/2020
void build_mines(int** board, int rows, int cols, int n_mines) {
	srand(time(NULL)); // srand initialisieren
	//Die minen sollen auf die feldern zufaellig verteilt werden
	while (n_mines > 0) {
		board[rand() % rows][rand() % cols] = MINE;
		n_mines--;
	}
}
// Funktionsname: calculate_adjacent_mines
// Kurzbeschreibung der Aufgabe der Funktion: Eine Funktion zum Zählen der Anzahl von Minen in den angrenzenden Felder von feld
// Eingabeparameter mit zulässigem Wertebereich: board , rows ,cols ,x ,y
// Rückgabeparameter mit zulässigem Wertebereich: counter
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 03/12/2020
int calculate_adjacent_mines(int** board, int rows, int cols, int x, int y) {
	int counter = 0; //Initialisierung und deklarierung
	int i = -1;
	int j = -1;
// Zählen der Anzahl von Minen in den angrenzenden Felder
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (x + i < rows && x + i >= 0 && y + j < cols && y + j >= 0) { // Überprüfen , ob die Nachbarkoordinaten sind innerhalb der Array-Grenzen
				counter = (board[x + i][y + j] == MINE) ? counter + 1 : counter;
			}
		}
	}
	return counter;
}
// Funktionsname: show_mines
// Kurzbeschreibung der Aufgabe der Funktion : Minen auf dem Spielfeld anzeigen
// Eingabeparameter mit zulässigem Wertebereich: board , status ,rows,cols
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 03/12/2020
void show_mines(int** board, int** status, int rows, int cols) {
	int i = 0; //initialisierung und deklarierung
	int j = 0;
	//Alle Minen auf dem Spielfeld sollen angezeigt werden
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			if (board[i][j] == MINE)
				status[i][j] = SHOW;
		}
	}

}
// Funktionsname: output_board
// Kurzbeschreibung der Aufgabe der Funktion : Das Spielfeld auf dem Bildschirm ausgeben
// Eingabeparameter mit zulässigem Wertebereich: board,status,rows,cols
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohmaed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 06/12/2020
void output_board(int** board, int** status, int rows, int cols) {
	char c = 'A';
	printf("     ");
	for (int i = 0; i < cols; i++) {
		printf(" %d  ", (i + 1)); //  Zahlen für die Spaltenkoordinaten ausgeben
	}
	printf("\n");
    
	for (int i = 0; i < rows; i++) {
		printf("- %c |", c + i); //Buchstaben für die Zeilenkoordinaten
		for (int j = 0; j < cols; j++) {
			switch (board[i][j]) {
			case NO_ENTRY:
				if (status[i][j] == MARKED) //wenn des markiertes feld No Entry ist
					printf(" M |");
				else
					printf(" - |"); //beim spielbegin sollen aller feldern verdeckt werden
				break;
			case OPEN:
				printf(" O |");
				break;
			case MINE:
				if (status[i][j] == SHOW) //Alle Minen auf dem Spielfeld sollen angezeigt werden ( Im Fall verlieren oder gewinen )
					printf(" X |");
				else if (status[i][j] == MARKED) //wenn das markiertes feld ein Mine ist
					printf(" M |");
				else
					printf(" - |"); //beim spielbegin sollen aller feldern verdeckt werden
				break;
			case FREED:
				if (status[i][j] == SHOW_M_SURROUNDING) { //wenn es Mine oder Markiertes Feld darum liegt
					int neighboors = calculate_adjacent_mines(board, rows, cols,
							i, j);
					if (neighboors > 0)//wenn es eine Mine herum liegt
						printf(" %d |", neighboors);
					else
						printf(" * |");//wenn es markiertes feld herum liegt
				} else if (status[i][j] == MARKED)
					printf(" M |");
				else
					printf(" * |");//wenn es keine Mine oder markiertes feld darum liegt
				break;
			}
		}
		printf("\n-----");
		for (int k = 0; k < cols; k++)
			printf("----");
		printf("\n");
	}
}

// Funktionsname: check_winner
// Kurzbeschreibung der Aufgabe der Funktion : Funktion um zu wissen, dass man gewonnen hat oder nicht
// Eingabeparameter mit zulässigem Wertebereich: board,status,rows,cols,n_mines
// Rückgabeparameter mit zulässigem Wertebereich: 1 oder 0
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 23/12/2020
int check_winner(int** board, int** status, int rows, int cols, int n_mines) {
	int counter = 0; //Initialisierung und deklarierung
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			if (board[i][j] == MINE && status[i][j] == MARKED)//wenn das feld eine markierte Mine
				counter++;
		}
	}
	if (n_mines == counter)
		return 1;
	else
		return 0;
}

// Funktionsname: mark_cell
// Kurzbeschreibung der Aufgabe der Funktion : Feld markieren / Markierung entfernen
// Eingabeparameter mit zulässigem Wertebereich: board ,status ,rows,cols
// Rückgabeparameter mit zulässigem Wertebereich: GAME_ONGOING
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 06/12/2020
int mark_cell(int** board, int** status, int rows, int cols,int n_mines) {
	int j = 0; //Initialisierung und deklarierung von j
	char cell[10] = ""; //Initialisierung und deklarierung von cell
	int xRow = 0;
	int i = 0;
	int yCol = 0;
	char rest[9] = { 0 }; //Initialisierung und deklarierung von rest

	printf("Select a cell to mark:\t");
	fgets(cell, 10, stdin); //einlesen von standard input und speichern es in Cell
	xRow = cell[0];	
	xRow %= 'A'; // xRow = xRow % 'A' = xRow % 64
    if (xRow < 0 || xRow >= rows) {
		printf("invalid input for X\n");
	    return GAME_ONGOING;	
	}
	for (i = 1; cell[i]; i++) {
		if (isdigit(cell[i])) //Uberprufen ob das Charakter ein zahl Charakter ist
			rest[j++] = cell[i];
	}
	yCol = atoi(rest); //Integer wert von string bekommen
	if (yCol < 1 || yCol > cols) {
		printf("invalid input for Y\n");
	    return GAME_ONGOING;	
	}
	// Falle von Markieren ein Feld
	if (board[xRow][yCol - 1] == OPEN || board[xRow][yCol - 1] == FREED) { //wenn das feld schon aufgedeckt ist
		printf("cell is already opened\n");
	} else if (status[xRow][yCol - 1] == MARKED) { //Ist ein Feld bereits markiert, dann wird die Markierung bei erneutem Markieren entfernt
		status[xRow][yCol - 1] = DONT_SHOW;
		output_board(board, status, rows, cols);
	} else {
		status[xRow][yCol - 1] = MARKED; //das feld markieren
		output_board(board, status, rows, cols);
        int win = check_winner(board, status, rows, cols, n_mines);
		if (win) { //wenn das Spiel gewonnen ist
			show_mines(board, status, rows, cols);
			output_board(board, status, rows, cols);
			printf("you won!\n");
			return GAME_WON;
		} else
			//wenn das Spiel noch nicht gewonnen ist
			output_board(board, status, rows, cols);
		return GAME_ONGOING;
	}
	return GAME_ONGOING;
}

// Funktionsname: check_proximity
// Kurzbeschreibung der Aufgabe der Funktion: Uberprufen ob es eine mine oder ein markiertes feld herum gibt und wie viel von denen gibt
// Eingabeparameter mit zulässigem Wertebereich: board , status ,rows , cols ,x,y
// Rückgabeparameter mit zulässigem Wertebereich: counter
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 22/12/2020
int check_proximity(int** board, int** status, int rows, int cols, int x, int y) {
	int counter = 0; //initialisierung und deklarierung
	int i = 0;
	int j = 0;
	for (i = -1; i <= 1; i++) {
		if (x + i >= 0 && x + i < rows) { // Überprüfen , ob die Nachbarkoordinaten sind innerhalb der Array-Grenzen
			for (j = -1; j <= 1; j++) {
				if ((i != 0 || j != 0) && y + j >= 0 && y + j < cols) { // Überprüfen , ob die Nachbarkoordinaten sind innerhalb der Array-Grenzen , das gewaehltes Feld wird nicht betrachtet
					if (board[x + i][y + j] == MINE || status[x + i][y + j] == MARKED) { //wenn ein Feld markiert oder darauf eien Mine gibt
						counter++;
					}
				}
			}
		}
	}
	return counter;
}
// Funktionsname: free_adjacent_cells
// Kurzbeschreibung der Aufgabe der Funktion : Aufdecken angrenzende Felder
// Eingabeparameter mit zulässigem Wertebereich:board , status ,rows , cols ,x,y
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 23/12/2020
void free_adjacent_cells(int** board, int** status, int rows, int cols, int x, int y) {
	int i = 0; //initialisierung und deklarierung
	int j = 0;
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) //Das Gewaehltes feld wird nicht betrachtet , springen zu naechste iteration
				continue;
			else if (x + i >= 0 && x + i < rows && y + j >= 0 && y + j < cols) { // Überprüfen , ob die Nachbarkoordinaten sind innerhalb der Array-Grenzen
				if (board[x + i][y + j] != MINE && status[x + i][y + j] != MARKED) {//wenn das feld weder eine Mine oder markiert ist
					if (board[x + i][y + j] != OPEN && board[x + i][y + j] != FREED) {//wenn das feld noch nicht aufgedeckt ist
						if (check_proximity(board, status, rows, cols, x + i, y + j) > 0) { //Wenn ein markiertes feld oder eine Mine herum liegt
                            board[x + i][y + j] = FREED;
							status[x + i][y + j] = SHOW_M_SURROUNDING;
						} else {
							board[x + i][y + j] = FREED;
							status[x + i][y + j] = SHOW;
							free_adjacent_cells(board, status, rows, cols, x + i, y + j);
						}
					}
				}
			}
		}
	}
}

// Funktionsname:open_cell
// Kurzbeschreibung der Aufgabe der Funktion: Feld aufdecken
// Eingabeparameter mit zulässigem Wertebereich: board ,status,rows,cols,n_mines
// Rückgabeparameter mit zulässigem Wertebereich: GAME_LOST oder GAME_ONGOING oder GAME_WON
// Autor: Mohamed Aziz Labbouz
// Version: eclipse
// Datum der letzten Änderung: 23/12/2020
int open_cell(int** board, int** status, int rows, int cols, int n_mines) {
	int j = 0; //Initialisierung und deklarierung von j
	char cell[10] = { 0 }; //Initialisierung und deklarierung von cell
	int xRow = 0;
	int i = 0;
	char rest[9] = { 0 }; //Initialisierung und deklarierung von rest
	int yCol = 0;
	printf("Select a cell to open:\t");
	fgets(cell, 10, stdin); //einlesen von standard input und speichern es in Cell
	xRow = cell[0];
    xRow %= 'A';
    if (xRow < 0 || xRow >= rows) {
		printf("invalid input for X\n");
		return GAME_ONGOING;
	}

	for (i = 1; cell[i]; i++) {
		if (isdigit(cell[i])) //Uberprufen ob das Charakter ein zahl Charakter ist
			rest[j++] = cell[i];
	}
	yCol = atoi(rest); //Integer wert von string bekommen
	if (yCol < 1 || yCol > cols) {
		printf("invalid input for Y\n");
	    return GAME_ONGOING;
    }
	// Falle von Oeffnen ein Feld

	//Wenn ein feld bereits markiert ist
	if (status[xRow][yCol - 1] == MARKED) {
		printf("you can't open this marked cell!\n");
	}
	//wenn das feld schon aufgedeckt ist
	else if (board[xRow][yCol - 1] == FREED || board[xRow][yCol - 1] == OPEN) {
		printf("cell is already opened!\n");
	}
	//wenn Auf dem feld keine Mine gibt
	else if (board[xRow][yCol - 1] != MINE) {
		board[xRow][yCol - 1] = OPEN;
		status[xRow][yCol - 1] = SHOW;
		free_adjacent_cells(board, status, rows, cols, xRow, yCol - 1);
        output_board(board,status,rows,cols);    
	}
	//wenn auf dem Feld eine Mine gibt
	else {
		show_mines(board, status, rows, cols);
		output_board(board, status, rows, cols);
		printf("you lost!\n");
		return GAME_LOST;
	}
}
// Funktionsname: save_game
// Kurzbeschreibung der Aufgabe der Funktion : Spielfeld abspeichern
// Eingabeparameter mit zulässigem Wertebereich: datei,path,board,status,rows,cols
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohamed Aziz Labbouz
// Version: Eclipse
// Datum der letzten Änderung: 23/12/2020
void save_game(FILE* datei, char* path, int** board, int** status, int rows,
		int cols) {
	datei = fopen(path, "a"); //datei Oeffnen
	char c = 'A';
	fprintf(datei, "     ");
	for (int i = 0; i < cols; i++) {
		fprintf(datei, "%d   ", (i + 1)); //Zahlen für die Spaltenkoordinaten in file schreiben
	}
	fprintf(datei, "\n");

	for (int i = 0; i < rows; i++) {
		fprintf(datei, "- %c |", c + i); // Buchstaben für die Zeilenkoordinaten in file schreiben
		for (int j = 0; j < cols; j++) {
			switch (board[i][j]) {
			case OPEN:
				fprintf(datei, " O |");
				break;
			case NO_ENTRY:
				if (status[i][j] == MARKED)
					fprintf(datei, " M |");
				else
					fprintf(datei, " - |"); //beim spielbegin sollen aller feldern verdeckt werden
				break;
			case MINE:
				if (status[i][j] == SHOW)
					fprintf(datei, " X |");
				else if (status[i][j] == MARKED)
					fprintf(datei, " M |");
				else
					fprintf(datei, " - |"); //beim spielbegin sollen aller minen verdeckt werden
				break;
			case FREED:
				if (status[i][j] == SHOW_M_SURROUNDING) {
					int neighboors = calculate_adjacent_mines(board, rows, cols,
							i, j);
					if (neighboors > 0)
						fprintf(datei, " %d |", neighboors);
					else
						fprintf(datei, " * |");
				} else if (status[i][j] == MARKED)
					fprintf(datei, " M |");
				else
					fprintf(datei, " * |");
				break;
			}
		}
		fprintf(datei, "\n-----");
		for (int k = 0; k < cols; k++)
			fprintf(datei, "----");
		fprintf(datei, "\n");
	}
	fclose(datei); //datei schliessen
}
// Funktionsname: start_play
// Kurzbeschreibung der Aufgabe der Funktion : Waehlen entweder Markieren oder Oeffnen
// Eingabeparameter mit zulässigem Wertebereich: board,status,rows,cols,n_mines
// Rückgabeparameter mit zulässigem Wertebereich: keine
// Autor: Mohamed Aziz Labbouz
// Version: eclipŝe
// Datum der letzten Änderung: 22/12/2020
int start_play(int** board, int** status, int rows, int cols, int n_mines) {
	printf("%s", help);
	char cell[10] = { 0 }; //initialisierung und deklarierung
	fgets(cell, 10, stdin); //einlesen von standard input und speichern es in Cell
	if (!strncmp(cell, "Mark", 4)) { //Vergleichen zwischen cell und Mark
		return mark_cell(board, status, rows, cols,n_mines);
	} else if (!strncmp(cell, "Open", 4)) { //Vergleichen zwischen cell und Open
		return open_cell(board, status, rows, cols, n_mines);
	} else if (!strncmp(cell, "Exit", 4)) { //Vergleichen zwischen cell und Exit
		printf("Exiting....\n");
		exit(0);
	} else {
		printf("Invalid instruction!\n%s", help);
	}
}

/*
 board[][] : nimmt -> NO_ENTRY | MINE | OPEN | FREED
 status[][]: nimmt -> SHOW | DONT_SHOW | SHOW_M_SURROUNDINGS | MARKED
 */

int main(int argc, char** argv) {
	int n_mines = 0; //initialisierung und deklarierung
	FILE* datei=NULL;
	if (argc < 4) {
		printf("Not enough arguments passed!\n");
		exit(1);
	}

	int rows = atoi(argv[2]); // Integer wert bekommen
	int cols = atoi(argv[3]); //Integer wert bekommen

	if (rows > 26 || rows < 10) { //Anzahl der Zeilen soll zwischen 10 und 26 sein
		printf("invalid rows number!\n");
	}
	if (cols > 26 || cols < 10) { //Anzahl der Spalten soll zwischen 10 und 26 sein
		printf("invalid cols number!\n");
	}
	//Speicher reservieren
	int** board = (int**) malloc(sizeof(int*) * rows);
	if (board == NULL) {
		printf("failed to allocate memory!\n");
		exit(1);
	}
	int** status = (int**) malloc(sizeof(int*) * rows);
	if (status == NULL) {
		printf("failed to allocate memory!\n");
		exit(1);
	}
	for (int i = 0; i < rows; i++) {
		board[i] = (int*) malloc(sizeof(int) * cols);
		status[i] = (int*) malloc(sizeof(int) * cols);
		if (board[i] == NULL || status[i] == NULL) {
			printf("failed to allocate memory!\n");
			exit(1);
		}
	}

	build_field(board, status, rows, cols);

	randomize_n_mines(&n_mines, rows, cols);
	build_mines(board, rows, cols, n_mines);

	save_game(datei, argv[1], board, status, rows, cols);

	output_board(board, status, rows, cols);
	double time_elapsed = 0.0; //initialisierung und deklarierung
	//Spieldauer bestimmen
	time_t starttime = time(NULL);  //StartZeit

	int sig = start_play(board, status, rows, cols, n_mines);
	while (sig != GAME_WON && sig != GAME_LOST) {
		sig = start_play(board, status, rows, cols, n_mines);
	}
	time_t endtime = time(NULL);  //EndZeit

	time_elapsed += ((double) (endtime - starttime));
    /*These functions return the smallest integral value that is not less than x.

       For example, ceil(0.5) is 1.0, and ceil(-0.5) is 0.0.
     */
    printf("elapsed = %lf\n",time_elapsed);
    int minutes =  (int)ceil(time_elapsed) / 60;
    int seconds = (int)ceil(time_elapsed) % 60;

	printf("\nThe game lasted : %d Minuten %d Sekunden\n", minutes,seconds); //Spieldauer auf dem Bildschirm ausgeben
    
	save_game(datei, argv[1], board, status, rows, cols);
	for (int i = 0; i < rows; i++) {
		free(board[i]); //Speicher freigeben
		free(status[i]); //Speicher freigeben
	}
	free(board); //Speicher freigeben
	free(status); //Speicher freigeben

}
