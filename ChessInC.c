
#include "ChessInC.h"

struct chess_board game_board;

void chess_boardCreate(struct chess_board * board){    
    board->board_turn = WHITE;

    board->inCheck = EMPTY;
    for (int i = 0; i < CHESS_BOARD_SIZE * CHESS_BOARD_SIZE; i++){
        int row = i / CHESS_BOARD_SIZE;
        int col = i % CHESS_BOARD_SIZE;
        
        //Initializes Pawns
        if (row == 1 || row == 6){
            board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
            board->spaces[matrixSpaceAt(row,col)].residingPiece = PAWN;
            board->spaces[matrixSpaceAt(row,col)].notMoved = 1;
            if (row == 6){
                board->spaces[matrixSpaceAt(row,col)].pieceOwner = BLACK;
            }
             else if (row == 1){
                board->spaces[matrixSpaceAt(row,col)].pieceOwner = WHITE;
             }
        }
        
        //Initializes Other pieces
        else if (row == 0 || row == 7){
            
            if (col == 0 || col == 7){
                board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                board->spaces[matrixSpaceAt(row,col)].residingPiece = ROOK;
                if ((col == 0 && row == 7) || (col == 7 && row == 0)){
                    board->spaces[matrixSpaceAt(row,col)].notMoved = 1;
                }
            }

            else if (col == 1 || col == 6){
                board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                board->spaces[matrixSpaceAt(row,col)].residingPiece = KNIGHT;
                board->spaces[matrixSpaceAt(row,col)].notMoved = 0;
            }   

            else if (col == 2 || col == 5){
                board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                board->spaces[matrixSpaceAt(row,col)].residingPiece = BISHOP;
                board->spaces[matrixSpaceAt(row,col)].notMoved = 0;
            } 
            
            else if (col == 3 && row == 7){
                
                    board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                    board->spaces[matrixSpaceAt(row,col)].residingPiece = KING;
                    board->spaces[matrixSpaceAt(row,col)].notMoved = 1;
            }
            
            else if (col == 4 && row ==7){
                    board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                    board->spaces[matrixSpaceAt(row,col)].residingPiece = QUEEN;
                    board->spaces[matrixSpaceAt(row,col)].notMoved = 0;
            }
            
            else if (col == 3 && row ==0){
                board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                board->spaces[matrixSpaceAt(row,col)].residingPiece = QUEEN;
                board->spaces[matrixSpaceAt(row,col)].notMoved = 0; 
            }
            
            else if (col == 4 && row ==0){
                board->spaces[matrixSpaceAt(row,col)].isEmpty = 0;
                board->spaces[matrixSpaceAt(row,col)].residingPiece = KING;
                board->spaces[matrixSpaceAt(row,col)].notMoved = 1; 
            }

            if (row == 7) { 
                board->spaces[matrixSpaceAt(row,col)].pieceOwner = BLACK;
            }
            
            else if (row == 0){
                board->spaces[matrixSpaceAt(row,col)].pieceOwner = WHITE;
            }

        }

        //Initializing Empty spaces 
        else {
            board->spaces[matrixSpaceAt(row,col)].isEmpty = 1;
            board->spaces[matrixSpaceAt(row,col)].residingPiece = NO_PIECE;
            board->spaces[matrixSpaceAt(row,col)].pieceOwner = EMPTY;
            board->spaces[matrixSpaceAt(row,col)].notMoved = 0;
        }
    }
}

void chess_boardDelete(struct chess_board * board){
    //free(board.spaces);
    free(board);
}

void title_draw(){
    FILE *f = fopen("AsciiText/titleAscii.txt","r");
    if(f == NULL){
        printf("error opening text file\n");
    }
    char c;
    if(CLEAR_SCREEN_AFTER_MOVE){
        system("cls");
    }
    printf("\n\n\n");
    while( (c= fgetc(f)) != EOF){
        putchar(c);
    }
    fclose(f);
    printf("\n                                                                   Press Enter to Start a Game");
    char x;
    scanf("%c", &x);
}

void playAgain_draw(){
    FILE *f = fopen("AsciiText/playAgainAscii.txt","r");
    if(f == NULL){
        printf("error opening text file\n");
    }
    char c;
    system("cls");
    while( (c= fgetc(f)) != EOF){
        putchar(c);
    }
    fclose(f);
}

void getMovementsFromFile(FILE *file, char* movements){
    for (int i = 0; i<5; i++){
            if(i == 2){
                movements[i] = ' ';
            }
            else{
                movements[i] = fgetc(file);
            }
                
    }
    sleep(1);
    if(fgetc(file) == EOF){}

}

void chess_draw(struct chess_board * board){
    system("cls");
    printf("Black Captures:");
    for (int i = 0; i < board->numBlackCaptures; i++){
        printf(" %s ", PIECE_TO_STRING_BOARD(board->blackCaptures[i]));
    }
    printf("\n\n");
    printf(column_letters);
    for (int row = CHESS_BOARD_SIZE -1; row >= 0; row--){
        printf(rowLine_draw);
        printf("      %d  |",row+1);
        for (int col = 0; col < CHESS_BOARD_SIZE; col++){
            printf("  %s%s  |", PIECE_TO_STRING_BOARD(board->spaces[matrixSpaceAt(row,col)].residingPiece), PLAYER_TO_STRING_BOARD(board->spaces[matrixSpaceAt(row,col)].pieceOwner));
        }
        printf("  %d", row+1);
        printf("\n"); 
    }
    printf(rowLine_draw);
    printf(column_letters);


    printf("\nWhite Captures:");
    for (int i = 0; i < board->numWhiteCaptures; i++){
        printf(" %s ", PIECE_TO_STRING_BOARD(board->whiteCaptures[i]));
    }
    printf("\n\n");
}

void change_turn(struct chess_board * board){
    if(board->board_turn == WHITE){
        board->board_turn = BLACK;
    }
    else{
        board->board_turn = WHITE;
    }
}

enum moveErr pawn_move(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){

enum moveErr movementOutput;
struct chess_space startSpace = board->spaces[matrixSpaceAt(startMove_row, startMove_col)];
struct chess_space endSpace = board->spaces[matrixSpaceAt(endMove_row, endMove_col)];

//Pawn can move 2-1 spaces if in Original Position & moving to empty space
    if (endSpace.isEmpty){
        if (((startSpace.pieceOwner == WHITE && endMove_row == startMove_row + 1) 
           ||(startSpace.pieceOwner == BLACK && endMove_row == startMove_row -1)) 
           && startMove_col == endMove_col){
            movementOutput = VALID_PLACEMENT;
            
        }
        
        else if (startSpace.notMoved){
                if (((startSpace.pieceOwner == WHITE && endMove_row == startMove_row + 2) 
                   ||(startSpace.pieceOwner == BLACK && endMove_row == startMove_row - 2)) 
                   && startMove_col == endMove_col){
                    movementOutput = VALID_PLACEMENT;
                }
                else{
                    movementOutput = INVALID_PLACEMENT_MOVE_ERROR;
                }

            }
        else {
            movementOutput = INVALID_PLACEMENT_MOVE_ERROR;
        }
            
    }
        
    //If Pawn is Attacking, it can only attack in one space diagonally in front of it
    else if (!endSpace.isEmpty){
        if (((startSpace.pieceOwner == WHITE && endMove_row == startMove_row + 1) 
           ||(startSpace.pieceOwner == BLACK && endMove_row == startMove_row -1)) 
           &&(endMove_col == startMove_col + 1 || endMove_col == startMove_col - 1)){
                movementOutput = VALID_PLACEMENT;
            }
        else{
            movementOutput = INVALID_PLACEMENT_MOVE_ERROR;
        }
    }
    return movementOutput;
}

enum moveErr horizVert_check(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    //Checks for pieces in the way up 
    for (int i = startMove_row+1; i < endMove_row; i++ ){
        if (!board->spaces[matrixSpaceAt(i,startMove_col)].isEmpty){
            return PIECE_IN_PATH;
        }               
    }
    //Checks for pieces down
    for (int i = startMove_row-1; i > endMove_row; i-- ){
        if (!board->spaces[matrixSpaceAt(i,startMove_col)].isEmpty){
            return PIECE_IN_PATH;
        }
    }
    //Checks for pieces right
    for (int i = startMove_col+1; i < endMove_col; i++ ){
        if (!board->spaces[matrixSpaceAt(startMove_row,i)].isEmpty){
            return PIECE_IN_PATH;
        }
    }
    //Checks for pieces left
    for (int i = startMove_col-1; i > endMove_col; i-- ){
        if (!board->spaces[matrixSpaceAt(startMove_row,i)].isEmpty){
            return PIECE_IN_PATH;
        }
    }
    return VALID_PLACEMENT;

}

enum moveErr rook_move(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    struct chess_space startSpace = board->spaces[matrixSpaceAt(startMove_row, startMove_col)];
    struct chess_space endSpace = board->spaces[matrixSpaceAt(endMove_row, endMove_col)];
    if((abs(endMove_row-startMove_row) > 0 && abs(endMove_col-startMove_col) == 0) ||
       (abs(endMove_row-startMove_row) == 0 && abs(endMove_col-startMove_col) > 0)){
            //if (startSpace.notMoved && endSpace.residingPiece == KING && endSpace.notMoved){
            return horizVert_check(board, startMove_row, startMove_col, endMove_row, endMove_col);
        }
    else{
        return INVALID_PLACEMENT_MOVE_ERROR;
    }
    
}

enum moveErr knight_move(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    if ((abs(startMove_row-endMove_row) == 2 && abs(startMove_col-endMove_col) == 1) ||
        (abs(startMove_row-endMove_row) == 1 && abs(startMove_col-endMove_col) == 2)){
            return VALID_PLACEMENT;
        }
    return INVALID_PLACEMENT_MOVE_ERROR;
}

enum moveErr diagonal_check(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    for(int i = startMove_row+1, j = startMove_col+1; i < endMove_row && j < endMove_col; i++,j++){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                    return PIECE_IN_PATH;
            }
        }

        for(int i = startMove_row-1, j = startMove_col+1; i > endMove_row && j < endMove_col; i--,j++){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                    return PIECE_IN_PATH;
            }
        }

        for(int i = startMove_row+1, j = startMove_col-1; i < endMove_row && j > endMove_col; i++,j--){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                    return PIECE_IN_PATH;
            }
        }

        for(int i = startMove_row-1, j = startMove_col-1; i > endMove_row && j > endMove_col; i--,j--){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                    return PIECE_IN_PATH;
            }
        }

        return VALID_PLACEMENT;
}

enum moveErr bishop_move(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){    
    if(abs(startMove_row - endMove_row) == abs(startMove_col - endMove_col)){
        return diagonal_check(board, startMove_row, startMove_col, endMove_row, endMove_col);
    }
    else{
        return INVALID_PLACEMENT_MOVE_ERROR;
    }
}

enum moveErr queen_move(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    if ((abs(endMove_row-startMove_row) > 0 && abs(endMove_col-startMove_col) == 0) || 
        (abs(endMove_row-startMove_row) == 0 && abs(endMove_col-startMove_col) > 0)){
            return horizVert_check(board, startMove_row, startMove_col, endMove_row, endMove_col);
    }
    else if (abs(startMove_row - endMove_row) == abs(startMove_col - endMove_col)){
        return diagonal_check(board, startMove_row, startMove_col, endMove_row, endMove_col);
    }
    else {
        return INVALID_PLACEMENT_MOVE_ERROR;
    }

}

enum moveErr horizVert_checkChecker(struct chess_board * board,int kingRow, int kingCol, enum player ownerOfKing){
    //Checks for pieces in the way up 
    for (int i = kingRow+1; i < 8; i++ ){
        if (!board->spaces[matrixSpaceAt(kingRow,i)].isEmpty){
            if(board->spaces[matrixSpaceAt(kingRow,i)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(kingRow,i)].residingPiece == ROOK || 
                    board->spaces[matrixSpaceAt(kingRow,i)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
        }               
    }
    //Checks for pieces down
    for (int i = kingRow-1; i >- 0; i-- ){
        if (!board->spaces[matrixSpaceAt(kingRow,i)].isEmpty){
            if(board->spaces[matrixSpaceAt(kingRow,i)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(kingRow,i)].residingPiece == ROOK || 
                    board->spaces[matrixSpaceAt(kingRow,i)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                } 
                else{
                    break;
                }
        }
    }
    //Checks for pieces right
    for (int i = kingCol+1; i < 8; i++ ){
        if (!board->spaces[matrixSpaceAt(i, kingCol)].isEmpty){
            if(board->spaces[matrixSpaceAt(i, kingCol)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(i, kingCol)].residingPiece == ROOK || 
                    board->spaces[matrixSpaceAt(i, kingCol)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
        }
    }
    //Checks for pieces left
    for (int i = kingCol-1; i >= 0; i-- ){
        if (!board->spaces[matrixSpaceAt(i, kingCol)].isEmpty){
            if(board->spaces[matrixSpaceAt(i, kingCol)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(i, kingCol)].residingPiece == ROOK || 
                    board->spaces[matrixSpaceAt(i, kingCol)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
        }
    }
    return VALID_PLACEMENT;

}

enum moveErr diagonal_checkChecker(struct chess_board * board,int kingRow, int kingCol, enum player ownerOfKing){
    for(int i = kingRow+1, j = kingCol+1; i < 8 && j < 8; i++,j++){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                if(board->spaces[matrixSpaceAt(i,j)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(i,j)].residingPiece == BISHOP || 
                    board->spaces[matrixSpaceAt(i,j)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
            }
        }

        for(int i = kingRow-1, j = kingCol+1; i >= 0 && j < 8; i--,j++){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                if(board->spaces[matrixSpaceAt(i,j)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(i,j)].residingPiece == BISHOP || 
                    board->spaces[matrixSpaceAt(i,j)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
            }
        }

        for(int i = kingRow+1, j = kingCol-1; i < 8 && j >= 0; i++,j--){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                if(board->spaces[matrixSpaceAt(i,j)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(i,j)].residingPiece == BISHOP || 
                    board->spaces[matrixSpaceAt(i,j)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
            }
        }

        for(int i = kingRow-1, j = kingCol-1; i >= 0 && j >= 0; i--,j--){
            if (!board->spaces[matrixSpaceAt(i,j)].isEmpty){
                if(board->spaces[matrixSpaceAt(i,j)].pieceOwner != ownerOfKing &&
                    (board->spaces[matrixSpaceAt(i,j)].residingPiece == BISHOP || 
                    board->spaces[matrixSpaceAt(i,j)].residingPiece == QUEEN) ){
                    return IN_CHECK_MOVE_ERROR;
                }
                else{
                    break;
                }
            }
        }

        return VALID_PLACEMENT;
}

/*
enum moveErr knight_checkChecker(struct chess_Board * board, int kingRow, int kingCol, enum player ownerOfKing){
    return VALID_PLACEMENT;
}
*/


// must be called after movement in made, movement must be stimulated or
// after movement it made and then put it back if it causes a check
enum moveErr inCheckChecker(struct chess_board * board, int kingRow, int kingCol){
    enum player ownerOfKing = board->spaces[matrixSpaceAt(kingRow,kingCol)].pieceOwner;
    if(horizVert_checkChecker(board, kingRow, kingCol, ownerOfKing) == IN_CHECK_MOVE_ERROR){
        return IN_CHECK_MOVE_ERROR;
    }
    if(diagonal_checkChecker(board, kingRow, kingCol, ownerOfKing) == IN_CHECK_MOVE_ERROR){
        return IN_CHECK_MOVE_ERROR;
    }
    return VALID_PLACEMENT;
}

enum moveErr king_move(struct chess_board * board,int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    if ((abs(endMove_row-startMove_row) == 1 && abs(endMove_col-startMove_col) == 1) ||
        (abs(endMove_row - startMove_row) == 0 && abs(endMove_col-startMove_col) == 1)||
        (abs(endMove_row - startMove_row) == 1 && abs(endMove_col-startMove_col) == 0)){
            return VALID_PLACEMENT;
    }
    return INVALID_PLACEMENT_MOVE_ERROR;
}


enum moveErr chess_move(struct chess_board * board, int startMove_row, int startMove_col,
                    int endMove_row, int endMove_col){
    
    enum moveErr movementOutput;

    //Checking if the input movement is within the board
    if (startMove_row < 0 || startMove_col < 0 || endMove_row < 0 || startMove_col < 0 || 
            startMove_row > 7 || startMove_col > 7 || endMove_row > 7 || startMove_col > 7){
                return MOVEMENT_OUT_OF_BOUNDS;
            }

    struct chess_space startSpace = board->spaces[matrixSpaceAt(startMove_row, startMove_col)];
    struct chess_space endSpace = board->spaces[matrixSpaceAt(endMove_row, endMove_col)];

    //Checking if the space chosen to move has a piece in it 
    if (startSpace.isEmpty){
        return START_MOVE_EMPTY;
    }
    else if (startSpace.pieceOwner != board->board_turn){
        return MOVING_WRONG_PLAYER;
    }
    //Checking if Player it Trying to Castle
    else if(endSpace.pieceOwner == startSpace.pieceOwner){
        if(endSpace.residingPiece == KING && endSpace.notMoved && startSpace.notMoved){
        }
        else{
            return TARGETING_OWN_PIECE;
        }
    }
    else if(endSpace.pieceOwner == startSpace.pieceOwner && startSpace.residingPiece == ROOK && endSpace.residingPiece == PAWN){
        return TARGETING_OWN_PIECE;
    }


    //Check if Movement is valid for chosen piece
    switch (startSpace.residingPiece){
    case NO_PIECE:
        movementOutput = START_MOVE_EMPTY;
        break;
    case PAWN:
        movementOutput = pawn_move(board, startMove_row, startMove_col, endMove_row, endMove_col);
        break;
    case ROOK:
        movementOutput = rook_move(board, startMove_row, startMove_col, endMove_row, endMove_col);
        break;
    case KNIGHT:
        movementOutput = knight_move(board, startMove_row, startMove_col, endMove_row, endMove_col);
        break;
    case BISHOP:
        movementOutput = bishop_move(board, startMove_row, startMove_col, endMove_row, endMove_col);
        break;
    case QUEEN:
        movementOutput = queen_move(board, startMove_row, startMove_col, endMove_row, endMove_col);
        break;
    case KING:
        movementOutput = king_move(board, startMove_row, startMove_col, endMove_row, endMove_col);
        break;
    }

    if(board->board_turn == WHITE){
        //inCheckChecker(board, )
    }


    if (movementOutput == VALID_PLACEMENT){
        enum piece movedPiece = startSpace.residingPiece; 
        enum player movedPiecePlayer = startSpace.pieceOwner;

        if(!board->spaces[matrixSpaceAt(endMove_row, endMove_col)].isEmpty){
            if(board->spaces[matrixSpaceAt(endMove_row, endMove_col)].pieceOwner == BLACK){
                board->whiteCaptures[board->numWhiteCaptures] = board->spaces[matrixSpaceAt(endMove_row, endMove_col)].residingPiece;
                board->numWhiteCaptures++;
            }
            else{
                board->blackCaptures[board->numBlackCaptures] = board->spaces[matrixSpaceAt(endMove_row, endMove_col)].residingPiece;
                board->numBlackCaptures++;
            }
        }

        if (startSpace.residingPiece == ROOK && endSpace.residingPiece == KING && startSpace.pieceOwner == endSpace.pieceOwner){
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].notMoved = 0;
            board->spaces[matrixSpaceAt(endMove_row, endMove_col)].residingPiece = movedPiece;
            board->spaces[matrixSpaceAt(endMove_row, endMove_col)].pieceOwner = movedPiecePlayer;
            board->spaces[matrixSpaceAt(endMove_row, endMove_col)].isEmpty = 0;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].residingPiece = KING;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].pieceOwner = movedPiecePlayer;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].isEmpty = 0;
        }
        else {
            if(endSpace.pieceOwner != startSpace.pieceOwner && endSpace.residingPiece == KING){
                movementOutput = WINNING_MOVE;
            }
            //Allows for Pawn Promotion
            if(startSpace.residingPiece == PAWN && 
            ((endMove_row == 7 && startSpace.pieceOwner == WHITE)||
             (endMove_row == 0 && startSpace.pieceOwner == BLACK))){
                board->spaces[matrixSpaceAt(endMove_row, endMove_col)].residingPiece = QUEEN;
            }
            else{
                board->spaces[matrixSpaceAt(endMove_row, endMove_col)].residingPiece = movedPiece;
            }
            board->spaces[matrixSpaceAt(endMove_row, endMove_col)].pieceOwner = movedPiecePlayer;
            board->spaces[matrixSpaceAt(endMove_row, endMove_col)].isEmpty = 0;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].residingPiece = NO_PIECE;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].pieceOwner = EMPTY;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].isEmpty = 1;
            board->spaces[matrixSpaceAt(startMove_row, startMove_col)].notMoved = 0;
        }
        
    }
    return movementOutput;
}

void chess_game(){
    char x;
    int continueGame = 1;
    char movements[256];

    struct chess_board * the_board = malloc(sizeof(struct chess_board));
    the_board->whiteCaptures = malloc(sizeof(enum piece) * 16);
    the_board->numWhiteCaptures = 0;
    the_board->blackCaptures = malloc(sizeof(enum piece) * 16);
    the_board->numBlackCaptures = 0;
    enum moveErr movementOutput;
    chess_boardCreate(the_board);
    chess_draw(the_board);
    /*
    FILE *f = fopen("sampleChessGame.txt","r");
    if(f == NULL){
        printf("error opening text file\n");
    }
    char c;
    */
    
    while(continueGame){
        if(1){
            printf("%s Enter Movement (in from \"a2 a3\") or q to Quit Game:", PLAYER_TO_STRING_TEXT(the_board->board_turn));
            fgets(movements, 256, stdin);
            if(movements[0] == 'q'){
                continueGame = 0;
                break;
            }
        }
        else{
            //getMovementsFromFile(f,movements);
        
        }
        //printf("%s\n", movements);
        //printf("actual moves: row %d col %d row %d col %d\n", movements[1] - '1', tolower(movements[0]) - 'a', movements[4]-'1', tolower(movements[3] - 'a'));
        
        switch(chess_move(the_board, movements[1] - '1', tolower(movements[0]) - 'a', movements[4]-'1', tolower(movements[3] - 'a'))){
        
        case VALID_PLACEMENT:
        //printf("%s\n", MOVEERR_TO_STRING(VALID_PLACEMENT));
        change_turn(the_board);
        chess_draw(the_board);
        break;

        case MOVEMENT_OUT_OF_BOUNDS:
        printf("%s\n", MOVEERR_TO_STRING(MOVEMENT_OUT_OF_BOUNDS));
        break;
        
        case START_MOVE_EMPTY:
        printf("%s\n", MOVEERR_TO_STRING(START_MOVE_EMPTY));
        break;
        
        case TARGETING_OWN_PIECE:
        printf("%s\n", MOVEERR_TO_STRING(TARGETING_OWN_PIECE));
        break;

        case MOVING_WRONG_PLAYER:
        printf("%s\n", MOVEERR_TO_STRING(MOVING_WRONG_PLAYER));
        break;
        
        case INVALID_PLACEMENT_MOVE_ERROR:
        printf("%s\n", MOVEERR_TO_STRING(INVALID_PLACEMENT_MOVE_ERROR));
        break;

        case PIECE_IN_PATH:
        printf("%s\n", MOVEERR_TO_STRING(PIECE_IN_PATH));
        break;
        
        case IN_CHECK_MOVE_ERROR:
        printf("%s\n", MOVEERR_TO_STRING(IN_CHECK_MOVE_ERROR));
        break; 

        case WINNING_MOVE:
        chess_draw(the_board);
        //printf("actual moves: row %d col %d\n", movements[4]-'1', tolower(movements[3] - 'a'));
        printf("%s%s", PLAYER_TO_STRING_TEXT(the_board->board_turn), MOVEERR_TO_STRING(WINNING_MOVE));
        fflush(stdin);
        char x;
        scanf("%c", &x);
        printf("scan successful\n");
        continueGame = 0;
        break; 
        }
        
        
    }
    //fclose(f);
    free(the_board);
    //chess_boardDelete(the_board);
}



int main (int argc, char ** argv){
    SetConsoleTitle("EXTREME_CHESS_ONLINE");    
    int playAgain = 1;
    char buffer[256];
    
    title_draw();
    
    while(playAgain){
        chess_game();
        playAgain_draw();
        printf("\n                 Press Enter to play another Game or q to quit: ");
        fflush(stdin);
        fgets(buffer, 256, stdin);
        if(buffer[0] == 'q'){
            playAgain = 0;
        }
    }
}
