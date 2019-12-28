#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define EMPTY ' '
#define VISITED (char)35
#define W -200
#define L -100

static int row, col, level, num_mines;
int opened, end=0;

//Set the game and display arrays.
void initialize(char **game, char **display) {
    
    int i=0, j=0, k=0;
    
    for (i=0; i<row; i++) {
        
        for (j=0; j<col; j++) {
            
            game[i][j]=0;
            display[i][j]='$';
        }
    }
    i=0;
    for (k=0; k<=num_mines; k++) {
        
        //Generate a random position based on row and col, check if there is already a mine, and keep generating a random position until there is no mine on that position.
        while (game[i][j]==42) {
            
            i=rand()%row;
            j=rand()%col;
        }
        //Set a mine on a new random position.
        game[i][j]=42;
    }
    
    //Update all the remaining positions with numbers/empty tags.
    for (i=0; i<row; i++) {
        
        for (j=0; j<col; j++) {
            
            if (game[i][j]==42) {
              
                if(i > 0 && j > 0 && game[i - 1][j - 1] != 42) game[i - 1][j - 1]++;
                if(i > 0 && game[i - 1][j] != 42) game[i - 1][j]++;
                if(i > 0 && j < col-1 && game[i - 1][j + 1] != 42) game[i - 1][j + 1]++;
                if(j > 0 && game[i][j - 1] != 42) game[i][j - 1]++;
                if(j < col-1 && game[i][j + 1] != 42) game[i][j + 1]++;
                if(i < row-1 && j > 0 && game[i + 1][j - 1] != 42) game[i + 1][j - 1]++;
                if(i < row-1 && game[i + 1][j] != 42) game[i + 1][j]++;
                if(i < row-1 && j < col-1 && game[i + 1][j + 1] != 42) game[i + 1][j + 1]++;
            }
        }
    }
}

//Opens all the empty cells, and displays a number whenever the cell is not empty.
void cleanup(char **game, char **display, int i, int j) {
    
    if(i<0 || i>=row || j<0 || j>=col || game[i][j]==VISITED) return;
    
    //Check the current box.
    if(game[i][j]==0) {
        
        //Display EMPTY character if cell is empty.
        opened++;
        display[i][j]=EMPTY;
        game[i][j]=VISITED;
    }
    else {
        //Otherwise, print the corresponding number of mines surrounding that cell.
        opened++;
        display[i][j]='0'+game[i][j];
        game[i][j]=VISITED;
        return;
    }
    
    //Recursive calls for cleaning up adjacent places.
    cleanup(game, display, i-1, j-1);
    cleanup(game, display, i-1, j);
    cleanup(game, display, i-1, j+1);
    cleanup(game, display, i, j-1);
    cleanup(game, display, i, j+1);
    cleanup(game, display, i+1, j-1);
    cleanup(game, display, i+1, j);
    cleanup(game, display, i+1, j+1);
}

int play(char **game, char **display) {
    
    int x, y, i, j;
    
    //Promt the user to enter the coordinates to open a cell.
    printf("\nEnter the coordinates of a tile to open: ");
    scanf("%d %d", &x, &y);
    
    //Validate the coordinates.
    if (x<0 || x>=row || y<0 || y>=col) {
        
        printf("\nINVALID COORDINATES ENTERED!! PLEASE ENTER A VALID COMBINATION.\nRedirecting.......\n");
        sleep(3);
        return 0;
    }
    
    //If the cell is already opened, do nothing else; return 0.
    if (display[x][y]!='$') return 0;
    
    //Check if the entered cell is a mine and thus, the game is lost.
    if (game[x][y]==42) {
        
        //Converts every mine cell in game array to a mine in display array for printing.
        for (i=0; i<row; i++)
            for (j=0; j<col; j++)
                if (game[i][j]==42)
                    display[i][j]='*';
        display[x][y]='!'; //Special character for opened mine.
        return L; //Return "Game Lost!".
    }
    
    //If game is going on, open the current and adjacent cells.
    cleanup(game, display, x, y);
    
    //Check if the game has been won after the cleanup.
    if (opened==row*col-num_mines) {
        
        //Converts every mine cell in game array to a mine in display array for printing.
        for (i=0; i<row; i++)
            for (j=0; j<col; j++)
                if (game[i][j]==42)
                    display[i][j]='*';
        return W; //Return "Game Won!".
    }
    
    return 0; //The game goes on.
}

//Prints game board.
void show(char **display) {
     
    int i, j;
    
    //Header with column numbers
    printf("M I N E S W E E P E R\n **** Mines: %d ****\n\n   ", num_mines);
    for(i=0; i<col; i++)
        printf("%d ", i);
    
    printf("\n\n");
    
    //Board with row numbers
    for(i=0; i<row; i++) {
        printf("%d|", i);
        for(j=0; j<col; j++) {
            if (display[i][j]=='*')
                printf("%2c", display[i][j]);
            else if (display[i][j]=='^')
                printf("%2c", display[i][j]);
            else if (display[i][j]=='_')
                printf("%2c", display[i][j]);
            else printf("%2c", display[i][j]);
        }
        printf("\n |\n");
    }
}

//Create arrays using malloc.
char **create_matrix(int rows, int cols) {
    
    char **temp;
    temp=(char **)malloc(sizeof(*temp)*rows);
    
    for(int i=0; i<row; ++i)
        temp[i]=(char *)malloc(sizeof(**temp)*cols);
    
    return temp;
}

//Destroy the memory after use.
void destroy_matrix(char **matrix, int cols) {
    
    for(int i=0; i<cols; ++i)
        free(matrix[i]);
    
    free(matrix);
}

int main() {
    
    printf("\n  M I N E S W E E P E R\n\n");
    printf("Enter the number of rows in the game (MAX 20): ");
    scanf("%d", &row);
    printf("Enter the number of columns in the game (MAX 20): ");
    scanf("%d", &col);
    printf("Enter the difficulty level (1-10): ");
    scanf("%d", &level);
    
    if (row*col > 0 && row <= 20 && col <= 20 && level >= 1 && level <= 10) {
        
        //Depending on the level, number of mines is set. 35% for level 1 - 80% for level 10.
        num_mines = row*col*(0.8-((10-level)*0.05));
        srand(time(NULL));
        
        //Allocate memory space to the 2-D arrays.
        char ** game = create_matrix(row, col);
        char ** display = create_matrix(row, col);
        
        //Set the game and display arrays.
        initialize(game, display);
        
        //Play the game until it is won/lost.
        do {
            
            //Display the game board at the beginning. Keep displaying until the end is reached.
            show(display);
        } while (!(end=play(game, display)));
        
        //Display the final board status.
        show(display);
        
        //Game over. Deallocate the memory space for the arrays.
        destroy_matrix(game, col);
        destroy_matrix(display, col);
        
        //Decide if the player has lost or won.
        if (end==L) printf("\nOOPS! GAME LOST! BETTER LUCK NEXT TIME.\n\n");
        else printf("\nCONGRATULATIONS! YOU BEAT THE GAME!\n\n");
    }
    else {
        
        printf("\nInvalid input!!\nExiting the program...\n\n");
        return 0;
    }
    
    return 0;
}
