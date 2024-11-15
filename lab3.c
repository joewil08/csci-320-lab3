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

void* validate(void* arg) {
    param_struct* p = (param_struct*) arg;
    worker_validation[p->id] = 1;
    int validNums[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // 0 will turn to 1 when corresponding number is found

    // check the numbers in rows/columns/subgrids
    for (int row = p->starting_row; row <= p->ending_row; row++) {
        for (int col = p->starting_col; col <= p->ending_col; col++) {
            int num = sudoku_board[row][col];
            validNums[num - 1] = 1;
        }
    }

    // if a number is missing, then the thread is invalid
    for (int i = 0; i < 9; i++) {
        if (validNums[i] != 1) {
            worker_validation[p->id] = 0;
        }
    }
}

int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    
    tid = (pthread_t*)malloc(NUM_OF_THREADS * sizeof(pthread_t));
    parameter = (param_struct*)malloc(NUM_OF_THREADS * sizeof(param_struct));
    worker_validation = (int*)malloc(NUM_OF_THREADS * sizeof(int));
    int threadNumber = 0;

    // create threads for each row
    for (int i = 0; i < ROW_SIZE; i++) {
        parameter[threadNumber].id = threadNumber;
        parameter[threadNumber].starting_row = i;
        parameter[threadNumber].starting_col = 0;
        parameter[threadNumber].ending_row = i;
        parameter[threadNumber].ending_col = COL_SIZE - 1;
        pthread_create(&(tid[threadNumber]), NULL, validate, &(parameter[threadNumber]));
        threadNumber++;
    }

    // create threads for each column
    for (int i = 0; i < COL_SIZE; i++) {
        parameter[threadNumber].id = threadNumber;
        parameter[threadNumber].starting_row = 0;
        parameter[threadNumber].starting_col = i;
        parameter[threadNumber].ending_row = ROW_SIZE - 1;
        parameter[threadNumber].ending_col = i;
        pthread_create(&(tid[threadNumber]), NULL, validate, &(parameter[threadNumber]));
        threadNumber++;
    }

    // create threads for each subgrid
    int row = 0;
    int col = 0;
    for (int i = 0; i < NUM_OF_SUBGRIDS; i++) {
        parameter[threadNumber].id = threadNumber;
        parameter[threadNumber].starting_row = row;
        parameter[threadNumber].starting_col = col;
        parameter[threadNumber].ending_row = row + 2;
        parameter[threadNumber].ending_col = col + 2;
        pthread_create(&(tid[threadNumber]), NULL, validate, &(parameter[threadNumber]));
        threadNumber++;

        // move to next subgrid without going out of bounds
        if (col >= 6) {
            row += 3;
            col = 0;
        } else {
            col += 3;
        }
    }

    // check if there is an invalid row/col/subgrid
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(tid[i], NULL);
        if (worker_validation[i] != 1) {
            return 0; // the board is invalid
        }
    }
    
    free(worker_validation);
    free(tid);

    return 1; // the board is valid
}


