#include "Chess.h"

const int AI_PLAYER = 1;
const int HUMAN_PLAYER = -1;

Chess::Chess()
{
}

Chess::~Chess()
{
}

//
// make a chess piece for the player
//
Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("chess/") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;
    //
    // we want white to be at the bottom of the screen so we need to reverse the board
    //
    char piece[2];
    piece[1] = 0;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            ImVec2 position((float)(pieceSize * x + pieceSize), (float)(pieceSize * (_gameOptions.rowY - y) + pieceSize));
            _grid[y][x].initHolder(position, "boardsquare.png", x, y);
            _grid[y][x].setGameTag(0);
            piece[0] = bitToPieceNotation(y,x);
            _grid[y][x].setNotation(piece);
        }
    }

    
    for(int player = 0; player < 2; player++) //sets white pieces, then black
    {
        for(int i = 0; i < 8; i++) //set up pawns
        {
            Bit* pawn_bit = PieceForPlayer(player, Pawn);
            
            if(player == 0) //white
            {
                pawn_bit->setPosition(_grid[1][i].getPosition());
                pawn_bit->setParent(&_grid[1][i]);
                pawn_bit->setGameTag(Pawn);
                _grid[1][i].setBit(pawn_bit);          
            }

            if(player == 1) //black
            {
                pawn_bit->setPosition(_grid[6][i].getPosition());
                pawn_bit->setParent(&_grid[6][i]);
                pawn_bit->setGameTag(Pawn);
                _grid[6][i].setBit(pawn_bit);
            }

        }

        for(int i = 0; i < 2; i++) //set non pawns
        {


            Bit* rook_bit = PieceForPlayer(player, Rook); 
            Bit* knight_bit = PieceForPlayer(player, Knight);
            Bit* bishop_bit = PieceForPlayer(player, Bishop);
                  
            if (player == 0) //white 
            {
                if (i == 0) //first rook, knight, and bishop, and queen
                {
                    rook_bit->setPosition(_grid[0][0].getPosition());
                    rook_bit->setParent(&_grid[0][0]);
                    rook_bit->setGameTag(Rook);
                    _grid[0][0].setBit(rook_bit);

                    knight_bit->setPosition(_grid[0][1].getPosition());
                    knight_bit->setParent(&_grid[0][1]);
                    knight_bit->setGameTag(Knight);
                    _grid[0][1].setBit(knight_bit);

                    bishop_bit->setPosition(_grid[0][2].getPosition());
                    bishop_bit->setParent(&_grid[0][2]);
                    bishop_bit->setGameTag(Bishop);
                    _grid[0][2].setBit(bishop_bit);


                    Bit* queen_bit = PieceForPlayer(player, Queen);
                    queen_bit->setPosition(_grid[0][3].getPosition());
                    queen_bit->setParent(&_grid[0][3]);
                    queen_bit->setGameTag(Queen);
                    _grid[0][3].setBit(queen_bit);

                }

                if (i == 1) //second rook, knight, and bishop, and king
                {
                    rook_bit->setPosition(_grid[0][7].getPosition());
                    rook_bit->setParent(&_grid[0][7]);
                    rook_bit->setGameTag(Rook);
                    _grid[0][7].setBit(rook_bit);

                    knight_bit->setPosition(_grid[0][6].getPosition());
                    knight_bit->setParent(&_grid[0][6]);
                    knight_bit->setGameTag(Knight);
                    _grid[0][6].setBit(knight_bit);

                    bishop_bit->setPosition(_grid[0][5].getPosition());
                    bishop_bit->setParent(&_grid[0][5]);
                    bishop_bit->setGameTag(Bishop);
                    _grid[0][5].setBit(bishop_bit);

                    Bit* king_bit = PieceForPlayer(player, King);
                    king_bit->setPosition(_grid[0][4].getPosition());
                    king_bit->setParent(&_grid[0][4]);
                    king_bit->setGameTag(King);
                    _grid[0][4].setBit(king_bit);
                }
            }

            if (player == 1) //black
            {
                if (i == 0) //first rook,knight, and bishop
                {
                    rook_bit->setPosition(_grid[7][0].getPosition());
                    rook_bit->setParent(&_grid[7][0]);
                    rook_bit->setGameTag(Rook);
                    _grid[7][0].setBit(rook_bit);

                    knight_bit->setPosition(_grid[7][1].getPosition());
                    knight_bit->setParent(&_grid[7][1]);
                    knight_bit->setGameTag(Knight);
                    _grid[7][1].setBit(knight_bit);

                    bishop_bit->setPosition(_grid[7][2].getPosition());
                    bishop_bit->setParent(&_grid[7][2]);
                    bishop_bit->setGameTag(Bishop);
                    _grid[7][2].setBit(bishop_bit);

                    Bit* queen_bit = PieceForPlayer(player, Queen);
                    queen_bit->setPosition(_grid[7][3].getPosition());
                    queen_bit->setParent(&_grid[7][3]);
                    queen_bit->setGameTag(Queen);
                    _grid[7][3].setBit(queen_bit);
                }

                if (i == 1) //second rook, knight, and bishop
                {
                    rook_bit->setPosition(_grid[7][7].getPosition());
                    rook_bit->setParent(&_grid[7][7]);
                    rook_bit->setGameTag(Rook);
                    _grid[7][7].setBit(rook_bit);

                    knight_bit->setPosition(_grid[7][6].getPosition());
                    knight_bit->setParent(&_grid[7][6]);
                    knight_bit->setGameTag(Knight);
                    _grid[7][6].setBit(knight_bit);

                    bishop_bit->setPosition(_grid[7][5].getPosition());
                    bishop_bit->setParent(&_grid[7][5]);
                    bishop_bit->setGameTag(Bishop);
                    _grid[7][5].setBit(bishop_bit);


                    Bit* king_bit = PieceForPlayer(player, King);
                    king_bit->setPosition(_grid[7][4].getPosition());
                    king_bit->setParent(&_grid[7][4]);
                    king_bit->setGameTag(King);
                    _grid[7][4].setBit(king_bit);
                }

            }
            
        }

    }
    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    return true;
}


bool Chess::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst)
{
    //initial position of piece
    int starting_column = src.getPosition().x / 64; //divide by 64 to get index from 1-8
    int starting_row = 10 - (src.getPosition().y /64); //divide by 64, then subtract the result from 10 to get 1-8 index

    //destination for piece
    int destination_column = dst.getPosition().x / 64;
    int  destination_row = 10 - (dst.getPosition().y /64);


    if(bit.gameTag() == Pawn)
    {
        //white pawn can move forward one 
        if (destination_row == starting_row + 1 && destination_column == starting_column)
        {
            if(!dst.empty() || bit.getOwner()->playerNumber() == 1) //cant move to a non-empty spot
            {
                return false;
            }
            return true;
        }

        //white pawn can move forward 2 in the starting row
        if(starting_row == 2 && bit.getOwner()->playerNumber() == 0)
        {
            if(destination_row == starting_row + 2 && destination_column == starting_column)
            {
                if(!dst.empty() ||!_grid[destination_row - 2][destination_column-1].empty()) //
                {
                    return false;
                }
                return true;
            }
        }

        //black pawn can move forward one
        if (destination_row == starting_row - 1 && destination_column == starting_column)
        {
            if(!dst.empty() || bit.getOwner()->playerNumber() == 0) //cant move to a non-empty spot
            {
                return false;
            }

            return true;
        }

        //black pawn can move forward 2 from starting row
        if(starting_row == 7 && bit.getOwner()->playerNumber() == 1)
        {
            if(destination_row == starting_row - 2 && destination_column == starting_column)
            {
                if(!dst.empty() || !_grid[destination_row][destination_column - 1].empty())
                {
                    return false;
                }
                return true;
            }
        }

        //pawn can capture
        if(starting_column + 1 == destination_column || starting_column - 1 == destination_column) // capturing can happen either left of right
        {
            //white pawn
            if(starting_row + 1 == destination_row) //capture has to happen one row above
            {

                if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() == 1) //if pawn is white and piece to capture is black
                {
                    dst.destroyBit();
                    return true;
                }
            }

            //black pawn
            if(starting_row - 1 == destination_row) //capture has to happen one row below
            {
                if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() == 0)
                {
                    dst.destroyBit();
                    return true;
                }
            }
            return false;
        }
    }

    if(bit.gameTag() == Rook)
    {
        if(starting_column == destination_column || starting_row == destination_row) //rooks can move vertically or horizontally
        {
            if(starting_column == destination_column) //vertical movement
            {
                if(starting_row < destination_row) //upwards movement
                {
                    int row_index_of_blocking_piece = 9; //set to an out of bounds value initially

                    for(int row_index = starting_row; row_index < destination_row; row_index++) //look for a piece in the path to the destination
                    {
                        if(_grid[row_index - 1][destination_column-1].bit()!= nullptr) //checks if a piece is present
                        {
                            if(starting_row == row_index) //ensures that the rook itself isnt considered a piece thats in the way
                            {
                                continue;
                            }
                            row_index_of_blocking_piece = row_index;
                        }
                    }
                    if(destination_row > row_index_of_blocking_piece) //if the destination is above the blocking piece, dont let it move past
                    {
                        return false;
                    }
                }

                if(starting_row > destination_row) //downwards movement
                {
                    int row_index_of_blocking_piece = -1;

                    for(int row_index = starting_row; row_index > destination_row; row_index-- )
                    {
                        if(_grid[row_index - 1][destination_column-1].bit()!= nullptr) //checks if a piece is present
                        {
                            if(starting_row == row_index) //ensures that the rook itself isnt considered a piece thats in the way
                            {
                                continue;
                            }
                            row_index_of_blocking_piece = row_index;
                        }
                    }
                    if(destination_row < row_index_of_blocking_piece)
                    {
                        return false;
                    }
                }
            }

            

            if(starting_row == destination_row) //horizontal movement
            {
                if(starting_column < destination_column) //moving right 
                {
                    int column_index_of_blocking_piece = 9;

                    for(int col_index = starting_column; col_index < destination_column; col_index++)
                    {
                        if(_grid[destination_row-1][col_index-1].bit()!=nullptr) //check for present piece of same color
                        {
                            if(starting_column == col_index) //esnures taht the rook itself isnt considered in the way
                            {
                                continue;
                            }
                            column_index_of_blocking_piece = col_index;
                        }
                    }
                    if(destination_column > column_index_of_blocking_piece)
                    {
                        return false;
                    }

                }

                if(starting_column > destination_column) //moving left
                {
                    int column_index_of_blocking_piece = 0;

                    for(int col_index = starting_column; col_index > destination_column; col_index--)
                    {
                        if(_grid[destination_row-1][col_index-1].bit()!=nullptr) //check for present piece of same color
                        {
                            if(starting_column == col_index) //esnures taht the rook itself isnt considered in the way
                            {
                                continue;
                            }
                            column_index_of_blocking_piece = col_index;
                        }
                    }
                    if(destination_column < column_index_of_blocking_piece)
                    {
                        return false;
                    }

                }
            }
        }
    if(starting_row == destination_row || starting_column == destination_column)
    {
        if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
        {
        dst.destroyBit();
        }
        return true;
    }
    return false;
    }

    if(bit.gameTag() == Bishop)
    {
        int row_difference = abs(destination_row - starting_row);
        int column_difference = abs(destination_column - starting_column);

        if(row_difference == column_difference) //if the move is diagonal
        {
            int row_step = (destination_row > starting_row) ? 1 : -1; //direction of row movement
            int col_step = (destination_column > starting_column) ? 1 : -1; //direction of column movement

            int row_index_of_blocking_piece = 9; //set to an out of bounds value
            int column_index_of_blocking_piece = 9; //set to an out of bounds value 

            int current_row = starting_row + row_step;
            int current_column = starting_column + col_step;

            while(current_row != destination_row && current_column != destination_column) //check all squares along diagonal path
            {
                if(_grid[current_row - 1][current_column -1].bit() != nullptr) //check if a piece is present
                {
                    //hold index of blocking piece
                    row_index_of_blocking_piece = current_row;
                    column_index_of_blocking_piece = current_column;
                    break; 
                }

                current_column += col_step;
                current_row += row_step;
            }

            if (row_index_of_blocking_piece != 9 && column_index_of_blocking_piece != 9 && //only checks for blocking pieces if one is found
            ((destination_row > row_index_of_blocking_piece && row_step > 0) ||            //if a blocking piece is found, this ensures that 
             (destination_row < row_index_of_blocking_piece && row_step < 0) ||            //the bishop cannot jump pieces along any diagonal
             (destination_column > column_index_of_blocking_piece && col_step > 0) ||
             (destination_column < column_index_of_blocking_piece && col_step < 0)))
        {
            return false;
        }

            if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
            {
            dst.destroyBit();
            }
            return true; 
        }
        return false;
    }

    if(bit.gameTag() == Knight)
    {
        int row_difference = abs(destination_row - starting_row);
        int column_difference = abs(destination_column - starting_column);

        if( (row_difference == 2 && column_difference == 1) || //l shaped movement checks
            (row_difference == 1 && column_difference == 2) )
        {
            if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
            {
                dst.destroyBit();
            }
            return true;
        }
        return false;
    }

    if(bit.gameTag() == King)
    {
        int row_difference = abs(destination_row - starting_row);
        int column_difference = abs(destination_column - starting_column);

        if(row_difference <= 1 && column_difference <= 1) //only can move one square in any direction
        {
            if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
            {
                dst.destroyBit();
            }
            return true;
        }
        return false;
    }

    if(bit.gameTag() == Queen)
{
    int row_difference = abs(destination_row - starting_row);
    int column_difference = abs(destination_column - starting_column);

    // Queen can move either like a rook (same row or column) or like a bishop (diagonally)
    if((starting_row == destination_row || starting_column == destination_column) || // rook movement
       (row_difference == column_difference)) // bishop movement
    {
        if(starting_column == destination_column) // vertical movement 
        {
            if(starting_row < destination_row) // upwards movement
            {
                int row_index_of_blocking_piece = 9;

                for(int row_index = starting_row; row_index < destination_row; row_index++)
                {
                    if(_grid[row_index - 1][destination_column-1].bit() != nullptr)
                    {
                        if(starting_row == row_index)
                        {
                            continue;
                        }
                        row_index_of_blocking_piece = row_index;
                    }
                }
                if(destination_row > row_index_of_blocking_piece)
                {
                    return false;
                }
            }

            if(starting_row > destination_row) // downwards movement
            {
                int row_index_of_blocking_piece = -1;

                for(int row_index = starting_row; row_index > destination_row; row_index--)
                {
                    if(_grid[row_index - 1][destination_column-1].bit() != nullptr)
                    {
                        if(starting_row == row_index)
                        {
                            continue;
                        }
                        row_index_of_blocking_piece = row_index;
                    }
                }
                if(destination_row < row_index_of_blocking_piece)
                {
                    return false;
                }
            }
        }

        if(starting_row == destination_row) // horizontal movement 
        {
            if(starting_column < destination_column) // moving right
            {
                int column_index_of_blocking_piece = 9;

                for(int col_index = starting_column; col_index < destination_column; col_index++)
                {
                    if(_grid[destination_row-1][col_index-1].bit() != nullptr)
                    {
                        if(starting_column == col_index)
                        {
                            continue;
                        }
                        column_index_of_blocking_piece = col_index;
                    }
                }
                if(destination_column > column_index_of_blocking_piece)
                {
                    return false;
                }
            }

            if(starting_column > destination_column) // moving left
            {
                int column_index_of_blocking_piece = 0;

                for(int col_index = starting_column; col_index > destination_column; col_index--)
                {
                    if(_grid[destination_row-1][col_index-1].bit() != nullptr)
                    {
                        if(starting_column == col_index)
                        {
                            continue;
                        }
                        column_index_of_blocking_piece = col_index;
                    }
                }
                if(destination_column < column_index_of_blocking_piece)
                {
                    return false;
                }
            }
        }

        if(row_difference == column_difference) // diagonal movement 
        {
            int row_step = (destination_row > starting_row) ? 1 : -1;
            int col_step = (destination_column > starting_column) ? 1 : -1;

            int row_index_of_blocking_piece = 9;
            int column_index_of_blocking_piece = 9;

            int current_row = starting_row + row_step;
            int current_column = starting_column + col_step;

            while(current_row != destination_row && current_column != destination_column)
            {
                if(_grid[current_row - 1][current_column - 1].bit() != nullptr)
                {
                    row_index_of_blocking_piece = current_row;
                    column_index_of_blocking_piece = current_column;
                    break;
                }

                current_column += col_step;
                current_row += row_step;
            }

            if (row_index_of_blocking_piece != 9 && column_index_of_blocking_piece != 9 &&
                ((destination_row > row_index_of_blocking_piece && row_step > 0) ||
                 (destination_row < row_index_of_blocking_piece && row_step < 0) ||
                 (destination_column > column_index_of_blocking_piece && col_step > 0) ||
                 (destination_column < column_index_of_blocking_piece && col_step < 0)))
            {
                return false;
            }
        }

        
        if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()) //capture piece if there is one
        {
            dst.destroyBit();
        }
        return true;
    }
    return false;
}
    return false;
}

void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) 
{
    bit.setPosition(dst.getPosition());
    bit.setParent(&dst);
    dst.setBit(&bit);
    return;
    

}

//
// free all the memory used by the game on the heap
//
void Chess::stopGame()
{
}

Player* Chess::checkForWinner()
{
    // check to see if either player has won
    return nullptr;
}

bool Chess::checkForDraw()
{
    // check to see if the board is full
    return false;
}

//
// add a helper to Square so it returns out FEN chess notation in the form p for white pawn, K for black king, etc.
// this version is used from the top level board to record moves
//
const char Chess::bitToPieceNotation(int row, int column) const {
    if (row < 0 || row >= 8 || column < 0 || column >= 8) {
        return '0';
    }

    const char* wpieces = { "?PNBRQK" };
    const char* bpieces = { "?pnbrqk" };
    unsigned char notation = '0';
    Bit* bit = _grid[row][column].bit();
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag() & 127];
    } else {
        notation = '0';
    }
    return notation;
}

//
// state strings
//
std::string Chess::initialStateString()
{
    return stateString();
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string Chess::stateString()
{
    std::string s;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            s += bitToPieceNotation(y, x);
        }
    }
    return s;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void Chess::setStateString(const std::string &s)
{
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            int index = y * _gameOptions.rowX + x;
            int playerNumber = s[index] - '0';
            if (playerNumber) {
                _grid[y][x].setBit(PieceForPlayer(playerNumber - 1, Pawn));
            } else {
                _grid[y][x].setBit(nullptr);
            }
        }
    }
}


//
// this is the function that will be called by the AI
//
void Chess::updateAI() 
{
}

