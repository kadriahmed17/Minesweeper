#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <ctype.h>
#include <time.h>
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

const char *help = "This game supports the following instructions:\n\
Mark: to mark a cell\n\
Open: to open a cell\n\
Exit: to abandon game\n\
~>  \0";

/*
    FALSE -> Covered cell
    TRUE -> Uncovered cell
*/

void build_field(int** board,int** status,int rows,int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            board[i][j] = NO_ENTRY;
            status[i][j] = DONT_SHOW;
        }
    }
}

int randomize_n_mines(int n_mines,int rows,int cols){
    srand(time(NULL));
    //min + [0..max] = [min..max+min] --> [min..max]
    int min = (rows * cols) / 10;
    int max = (rows * cols) / 4;
    n_mines = min + rand() % (max-min+1);
    return n_mines;
}

void build_mines(int** board,int rows,int cols,int n_mines){
    srand (time (NULL));
    while(n_mines > 0){
        board[rand() % rows][rand() % cols] = MINE;
         n_mines--;
    }
}

int calculate_adjacent_mines(int** board,int rows,int cols,int x, int y){
    int counter = 0;
    for(int i = -1; i <=1; i++){
        for(int j = -1; j <= 1; j++){
            if(x+i < rows && x+i >= 0 && y+j < cols && y+j >= 0){
                counter = (board[x+i][y+j] == MINE) ? counter+1 : counter;
            }
        }
    }
    return counter;
}

void show_mines(int** board,int** status,int rows,int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(board[i][j] == MINE) status[i][j] = SHOW;
        }
    }

}

void output_board(int** board, int** status,int rows,int cols){
    char c = 'A';
    printf("     ");
    for(int i = 0; i < cols; i++){
        printf("%d   ",(i+1));
    }
    printf("\n");

    for(int i = 0; i < rows; i++){
        printf("- %c |",c+i);
        for(int j = 0; j < cols; j++){
            switch(board[i][j]){
                case NO_ENTRY:
                    if(status[i][j] == MARKED) printf(" M |");
                    else printf(" - |");
                    break;
                case OPEN:
                    printf(" O |");
                    break;
                case MINE:
                    if(status[i][j] == SHOW) printf(" X |");
                    else if(status[i][j] == MARKED) printf(" M |");
                    else printf(" - |");
                    break;
                case FREED:
                    if(status[i][j] == SHOW_M_SURROUNDING){
                        int neighboors = calculate_adjacent_mines(board,rows,cols,i,j);
                        if(neighboors > 0)
                            printf(" %d |", neighboors);
                        else printf(" * |");
                    }
                    else if(status[i][j] == MARKED) printf(" M |");
                    else printf(" * |");
                    break;
            }
        }
        printf("\n-----");
        for(int k = 0; k < cols; k++) printf("----");
        printf("\n");
    }
}

int mark_cell(int** board, int** status,int rows,int cols){
    printf("Select a cell to mark:\t");
    char cell[10] = {0};
    fgets(cell,10,stdin);
    int xRow = cell[0];
    if(xRow < 'A'){
        printf("invalid input for X\n");
        exit(1);
    }
    xRow %= 'A';
    char rest[9] = {0};
    int j = 0;
    for(int i = 1; cell[i]; i++){
        if(isdigit(cell[i])) rest[j++] = cell[i]; 
    }
    int yCol = atoi(rest);
    if(yCol < 1){
        printf("invalid input for Y\n");
        exit(1);
    }
    if (board[xRow][yCol-1] == OPEN || board[xRow][yCol-1] == FREED){
        printf("cell is already opened\n");
    }
    else if(status[xRow][yCol-1] == MARKED){
        status[xRow][yCol-1] = DONT_SHOW;
        output_board(board,status,rows,cols);
    }
    else{
        status[xRow][yCol-1] = MARKED;
        output_board(board,status,rows,cols);
    }
    return GAME_ONGOING;
}

/*
 * return FALSE(0) if there's a mine or marked cell adjacent to the cell
 * otherwise return TRUE(1)
*/

int check_proximity(int** board,int** status,int rows,int cols,int x,int y){
    int counter = 0;
    for(int i = -1; i <= 1; i++){
        if(x+i >= 0 && x+i < rows){
            for(int j = -1; j <= 1; j++){
                if((i != 0 || j != 0) && y+j >= 0 && y+j < cols){
                    if(board[x+i][y+j] == MINE || status[x+i][y+j] == MARKED){
                        counter++;
                    }
                }
            }
        }
    }
    return counter;
}

void free_adjacent_cells(int** board,int** status, int rows,int cols,int x,int y){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            else if(x+i >= 0 && x+i < rows && y+j >= 0 && y+j < cols){
                if(board[x+i][y+j] != MINE && status[x+i][y+j] != MARKED){
                    if(board[x+i][y+j] != OPEN && board[x+i][y+j] != FREED){
                        if(check_proximity(board,status,rows,cols,x+i,y+j) > 0){
                            board[x+i][y+j] = FREED;
                            status[x+i][y+j] = SHOW_M_SURROUNDING;
                        }
                        else{
                            board[x+i][y+j] = FREED;
                            status[x+i][y+j] = SHOW;
                            free_adjacent_cells(board,status,rows,cols,x+i,y+j);
                        }
                    }
                }
            }
        }
    }
}

int check_winner(int** board,int** status,int rows,int cols,int n_mines){
    int counter = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(board[i][j] == MINE && status[i][j] == MARKED) counter++;
        }
    }
    if(rows*cols - n_mines == counter) return 1;
    else return 0;
}

int open_cell(int** board,int** status,int rows,int cols,int n_mines){
    printf("Select a cell to open:\t");
    char cell[10] = {0};
    fgets(cell,10,stdin);
    int xRow = cell[0];
    if(xRow < 'A'){
        printf("invalid input for X\n");
        exit(1);
    }
    xRow %= 'A';
    char rest[9] = {0};
    int j = 0;
    for(int i = 1; cell[i]; i++){
        if(isdigit(cell[i])) rest[j++] = cell[i]; 
    }
    int yCol = atoi(rest);
    if(yCol < 1){
        printf("invalid input for Y\n");
        exit(1);
    }


    if(status[xRow][yCol-1] == MARKED){
        printf("you can't open this marked cell!\n");
    }
    else if(board[xRow][yCol-1] == FREED || board[xRow][yCol-1] == OPEN){
        printf("cell is already opened!\n");
    }
    else if(board[xRow][yCol-1] != MINE){
        board[xRow][yCol-1] = OPEN;
        status[xRow][yCol-1] = SHOW;
        free_adjacent_cells(board,status,rows,cols,xRow,yCol-1);
        int win = check_winner(board,status,rows,cols,n_mines);
        if(win){
            show_mines(board,status,rows,cols);
            output_board(board,status,rows,cols);
            printf("you won!\n");
            return GAME_WON;
        }
        else output_board(board,status,rows,cols);
        return GAME_ONGOING;
    }
    else{
        show_mines(board,status,rows,cols);
        output_board(board,status,rows,cols);
        printf("you lost!\n");
        return GAME_LOST;
    }
}

void save_game(FILE* datei,char* path,int** board,int** status,int rows,int cols){
    datei = fopen(path,"a");
    char c = 'A';
    fprintf(datei,"     ");
    for(int i = 0; i < cols; i++){
        fprintf(datei,"%d   ",(i+1));
    }
    fprintf(datei,"\n");

    for(int i = 0; i < rows; i++){
        fprintf(datei,"- %c |",c+i);
        for(int j = 0; j < cols; j++){
            switch(board[i][j]){
                case OPEN: 
                    fprintf(datei," O |");
                    break;
                case NO_ENTRY:
                    if(status[i][j] == MARKED) fprintf(datei," M |");
                    else fprintf(datei," - |");
                    break;
                case MINE:
                    if(status[i][j] == SHOW) fprintf(datei," X |");
                    else if(status[i][j] == MARKED) fprintf(datei," M |");
                    else fprintf(datei," - |");
                    break;
                case FREED:
                    if(status[i][j] == SHOW_M_SURROUNDING){
                        int neighboors = calculate_adjacent_mines(board,rows,cols,i,j);
                        if(neighboors > 0)
                            fprintf(datei," %d |", neighboors);
                        else fprintf(datei," * |");
                    }
                    else if(status[i][j] == MARKED) fprintf(datei," M |");
                    else fprintf(datei," * |");
                    break;
            }
        }
        fprintf(datei,"\n-----");
        for(int k = 0; k < cols; k++) fprintf(datei,"----");
        fprintf(datei,"\n");
    }
    fclose(datei);
}

int start_play(int** board,int** status,int rows,int cols,int n_mines){
    printf("%s",help);
    char cell[10] = {0};
    fgets(cell,10,stdin);
    if(!strncmp(cell,"Mark",4)){
        return mark_cell(board,status,rows,cols);
    }
    else if(!strncmp(cell,"Open",4)){
        return open_cell(board,status,rows,cols,n_mines);
    }
    else if(!strncmp(cell,"Exit",4)){
        printf("Exiting....\n");
        exit(0);
    }
    else{
        printf("Invalid instruction!\n%s",help);
    }
}


/*
  board[][] : takes -> NO_ENTRY | MINE | OPEN | FREED
  status[][]: takes -> SHOW | DONT_SHOW | SHOW_M_SURROUNDINGS | MARKED
  */

int main(int argc, char** argv){

    if(argc < 4){
        printf("Not enough arguments passed!\n");
        exit(1);
    }
    
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);

    if(rows > 26 || rows < 10) {
        printf("invalid rows number!\n");
    }
    if(cols > 26 || cols < 10){
        printf("invalid cols number!\n");
    }

    int** board = (int**) malloc(sizeof(int*) * rows);
    if(board == NULL){
        printf("failed to allocate memory!\n");
        exit(1);
    }
    int** status = (int**) malloc(sizeof(int*) * rows);
    if(status == NULL){
        printf("failed to allocate memory!\n");
        exit(1);
    }

    for(int i = 0; i < rows; i++){
        board[i] = (int*) malloc(sizeof(int) * cols);
        status[i] = (int*) malloc(sizeof(int) * cols);
        if(board[i] == NULL || status[i] == NULL){
            printf("failed to allocate memory!\n");
            exit(1);
        }
    }

    int n_mines = 0;
    FILE* datei;


    build_field(board,status,rows,cols);

    n_mines = randomize_n_mines(n_mines,rows,cols);
    build_mines(board,rows,cols,n_mines);

    save_game(datei,argv[1],board,status,rows,cols);

    output_board(board,status,rows,cols);
    double time_elapsed = 0.0;
    //here startime
    time_t starttime = time(NULL);

    int sig = start_play(board,status,rows,cols,n_mines);
    while(sig != GAME_WON && sig != GAME_LOST){
        sig = start_play(board,status,rows,cols,n_mines);
    }
    // here endtime
    time_t endtime = time(NULL);

    time_elapsed += (double) (endtime-starttime);

    printf("\nThe game lasted : %lf s\n",time_elapsed);
    save_game(datei,argv[1],board,status,rows,cols);
    for(int i = 0; i < rows; i++){
        free(board[i]);
        free(status[i]);
    }
    free(board);
    free(status);
    
}
