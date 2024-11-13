#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = NULL;

    fp = fopen(filename, "r");
    board = malloc(ROW_SIZE * sizeof(int *));
    for (int i = 0; i < ROW_SIZE; i++) {
        board[i] = malloc(COL_SIZE * sizeof(int));
    }
    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COL_SIZE; j++) {
            fscanf(fp, "%d,", &board[i][j]);
        }
    }
    fclose(fp);

    /* print the board
    for (int i = 0; i < ROW_SIZE; i++) {
        printf("Row %d: ", i);
        for (int j = 0; j < COL_SIZE; j++) {
            printf("%d,", board[i][j]);
        }
        printf("\n");
    }
    */

    return board;
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    
    // replace this comment with your code
    return 0;
}
