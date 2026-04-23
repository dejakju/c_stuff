/*
    $VER:       fen.c 1.0
    $DATE:      2026-04-23 (2026-04-23)
    $AUTHOR:    Goran (dejakju@gmail.com)

** Description: Display any chess position by a FEN string
*
*   A full FEN string actually consists of six fields separated by spaces:
* 
*       1. Piece Placement.
*       2. Active Color: w or b.
*       3. Castling Rights: K, Q, k, q, or -.
*       4. En Passant Target: A square (like e3) or -.
*       5. Halfmove Clock: Number of moves since the last capture or pawn advance (for the 50-move rule).
*       6. Fullmove Number: The count of completed turns.
*
*   Default FEN start position: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


typedef struct
{
    char active_color;
    char castling[5];
    char en_passant[3];
    int halfmove;
    int fullmove;
} gamestate_t;


bool
validate_and_parse(const char* fen, char board[8][8], gamestate_t* state)
{
    int i = 0, rank = 0, file = 0;
    const char* valid_pieces = "prnbqkPRNBQK";

    // 1. Parse Board
    while (fen[i] != ' ' && fen[i] != '\0') {
        if (fen[i] == '/') {
            if (file != 8) return false;
            rank++; file = 0;
        } else if (isdigit(fen[i])) {
            int val = fen[i] - '0';
            if (val < 1 || (file + val) > 8) return false;
            for (int s = 0; s < val; s++) board[rank][file++] = ' ';
        } else if (strchr(valid_pieces, fen[i])) {
            if (file >= 8) return false;
            board[rank][file++] = fen[i];
        } else return false;
        i++;
    }
    if (rank != 7 || file != 8) return false;

    // 2. Parse Extra Fields
    // Using sscanf to grab the remaining 5 fields (NOTE: it's a gotcha...)
    if (sscanf(&fen[i], " %c %4s %2s %d %d", 
        &state->active_color, state->castling, 
        state->en_passant, &state->halfmove, &state->fullmove) != 5) {
        return false;
    }
    return true;
}


void
display(char board[8][8], gamestate_t state)
{
    printf("\n  +---+---+---+---+---+---+---+---+\n");
    for (int i = 0; i < 8; i++) {
        printf("%d |", 8 - i);
        for (int j = 0; j < 8; j++) printf(" %c |", board[i][j]);
        printf("\n  +---+---+---+---+---+---+---+---+\n");
    }
    printf("    a   b   c   d   e   f   g   h\n\n");

    printf(">> Side to move: %s\n", (state.active_color == 'w') ? "White" : "Black");
    printf(">> Castling:     %s\n", state.castling);
    printf(">> En Passant:   %s\n", state.en_passant);
    printf(">> Halfmove:     %d (Rule of 50)\n", state.halfmove);
    printf(">> Fullmove:     %d\n\n", state.fullmove);
}


int
main(void)
{
    char fen[256];
    char board[8][8];
    gamestate_t state;

    printf("Enter Full FEN: ");
    if (!fgets(fen, sizeof(fen), stdin)) return 1;
    fen[strcspn(fen, "\n")] = 0;

    // Default the board to empty before parsing
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            board[i][j] = ' ';

    if (validate_and_parse(fen, board, &state)) {
        display(board, state);
    } else {
        printf("Error: Invalid or incomplete FEN string.\n");
    }

    return 0;
}
