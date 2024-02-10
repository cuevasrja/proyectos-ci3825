# include "board.h"
# include <stdio.h>

int main() {
    Board* board = newBoard();
    printBoard(board);
    return 0;
}