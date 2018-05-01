
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pigpio.h>
#include "8x8font.h"
#include "74hc595_functions.h"

#define BOARD_ROW 8     // board의 Row
#define PADDING 1;     // 문자간의 기본간격

uint8_t *board[BOARD_ROW];      // board
int     board_size;
char	text[255];
char	font[62] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";



// board에 문자 writing, margin-이전 문자와의 간격
void boardWriter(int fontidx, int margin);
void dot(int,int);
void callbacl(void);
int cnt;

int main(void)
{
    int total_col;

    scanf("%s",text);
    for(int i=0; i < sizeof(text); i++)
    {
	    for(int j = 0; j < 62; j++)
	    {
		    if(text[i] == font[j])
		    {
			    boardWriter(j,2);
		    }
	    }
    }

    for(int i = 0 ; i < BOARD_ROW ; i++){
        for(int j = 0 ; j < board_size ; j++)
	{
	    if(board[i][j] == 32)
	    {
		    board[i][j]=0;
	    }
            printf("%d ", board[i][j]);
	}
        printf("\n");
    }

    total_col = sizeof(board) / 8;

    int ret;
    ret = init();
    if(ret == 0)
    {
	    return 0;
    }

    gpioSetTimerFunc(0,50,callback);

    while(1)
    {
	    for(int i = 0; i < 8; i++)
	    {
		    for(int j = 0; k < 8 ; j++)
		    {
			    if(board[i][(j+cnt)%total_col] == 1)
				    dot(i+1,j+1);
		    }
	    }
    }

    release();
    return 0;
}

void boardWriter(int fontidx, int margin)
{
    int i, j;
    static int board_offset = 0; // 현재 문자의 offset
    uint8_t mask;
    uint8_t *new_board[BOARD_ROW];
    int total_col = sizeof(board) / 8;

    int new_board_size = board_offset + margin + FONT_COL + PADDING;


    for(i = 0 ; i < BOARD_ROW ; i++){
        new_board[i] = (uint8_t*)calloc(1, new_board_size);
        memset(new_board[i], ' ', new_board_size);
        memcpy(new_board[i], board[i], board_size);
        free(board[i]);
        board[i] = new_board[i];
    }

    board_size = new_board_size;
    board_offset += margin;

    // writing on the board
    for(i = 0 ; i < FONT_ROW ; i++){      // row
        for(j = 0 ; j < FONT_COL ; j++){  // col
            mask = 0b1 << (7-j);
            if((mask & FONT88[fontidx][i]))
                board[i][j+board_offset] = 1;
            else
                board[i][j+board_offset] = 0;
        }
    }
    board_offset = board_size;;
}

void dot(int row, int col)
{
    uint8_t row8, col8;
    uint16_t tmp;
    row8 = ~(1 << (8-row));
    col8 = 1 << (8-col);
    tmp = (row8<<8) | col8;
    set16(tmp);
}

void callback(void)
{
	cnt++;
}
