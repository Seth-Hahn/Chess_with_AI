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

static std::map<char, int> evaluateScores = {
        {'P', 100}, {'p', -100},
        {'N', 200}, {'n', -200},
        {'B', 230}, {'b', -230},
        {'R', 400}, {'r', -400},
        {'Q', 900}, {'q', -900},
        {'K', 2000}, {'k', -2000},
        {'0', 0} 
}; 

void Chess::FENtoBoard(std::string FEN_string)
{
    std::istringstream fenStream(FEN_string); //split out the board position of the FEN string
    std::string board_representation;         //other parts (ie. castling) can be handled later on
    fenStream >> board_representation;

    int row = 0;
    int col = 0;

    for(char c : board_representation) //main board set up
    {
        if(c == '/') //transition to next row of the board
        {
            row++;
            col = 0;
        }

        else if (isdigit(c)) //if c is a digit, then there are that many empty spaces in a row on the board
        {
            col += c - '0';
        }

        else
        {
            Bit* piece = nullptr; //create piece for player
            bool isWhite = isupper(c); //determine whether the piece is white or black
            int GameTag; 
            switch(tolower(c)) //determine the appropriate biece and set it
            {
                case 'p': 
                    piece = PieceForPlayer(isWhite ? 0 : 1, Pawn); 
                    GameTag = Pawn;
                    break;
                case 'r': 
                    piece = PieceForPlayer(isWhite ? 0 : 1, Rook); 
                    GameTag = Rook;
                    piece->movedFromStart(); //set to true to set up castling later
                    break;
                case 'n': 
                    piece = PieceForPlayer(isWhite ? 0 : 1, Knight);
                    GameTag = Knight; 
                    break;
                case 'b': 
                    piece = PieceForPlayer(isWhite? 0 : 1, Bishop);
                    GameTag = Bishop; 
                    break;
                case 'q': 
                    piece = PieceForPlayer(isWhite ? 0 : 1, Queen);
                    GameTag = Queen; 
                    break;
                case 'k': 
                    piece = PieceForPlayer(isWhite ? 0 : 1, King); 
                    GameTag = King;
                    piece->movedFromStart();
                    break;
            }

            if(piece != nullptr)
            {
                int boardRow = 7 - row; //flips the board position so black is on top, white on bottom
                piece->setPosition(_grid[boardRow][col].getPosition());
                piece->setParent(&_grid[boardRow][col]);
                piece->setGameTag(GameTag);
                _grid[boardRow][col].setBit(piece);
            }

            col++;

        }

    }

    //process the remaining parts of the string if they exist
    std::string active_player; //player to go next
    std::string castle_availability; //which castles are possible
    std::string en_passant; //where en passant is available
    std::string half_move_clock; //number of moves made since last pawn advance or piece capture
    std::string full_move_number; //number of turns completed in the game

    //split the remaining fen string into its seperate parts
    fenStream >> active_player >> castle_availability >> en_passant >> half_move_clock >> full_move_number;

    //set active player
    if(active_player == "w" || active_player == "b" )
    {
        if(active_player == "w") //white turn 
        {
            //Turns not implemented yet
        }
        
        else //black turn
        {

        }

        //castle availability
        for(char C : castle_availability)
        {
            switch(C)
            {
                case 'K': //white king side 
                    _grid[0][4].bit()->notMoved(); 
                    _grid[0][7].bit()->notMoved(); 
                    break;
                
                case 'Q': //white queen side
                    _grid[0][4].bit()->notMoved();
                    _grid[0][0].bit()->notMoved(); 
                    break;

                case 'k': //black king side
                    _grid[7][4].bit()->notMoved();
                    _grid[7][7].bit()->notMoved();
                    break;
                
                case 'q': //black queen side
                    _grid[7][4].bit()->notMoved();
                    _grid[7][0].bit()->notMoved();
                    break;

                case '-':
                    break;
            }
        }

        //en passant targets
        int column_index = -1;
        int row_index = -1;
        for(char c : en_passant) //parse en passant target to get row and column
        {
            if(c == '-')
            {
                continue;
            }

            if(isalpha(c))
            {
                column_index = c - 'a';
            }

            else if (isdigit(c))
            {
                row_index = c - '1';
            }
        }

        //set passantable pawn
        if(row_index == 3) //white pawn
        {
            set_passant(_grid[4][column_index].bit());
        }

        if(row_index == 6) //black pawn
        {
            set_passant(_grid[5][column_index].bit());
        }

        //half move clock
        set_half_moves( stoi (half_move_clock) );

        //full move number
        setCurrentTurnNo( stoi(full_move_number) + 1 ); //sets current turn number to full move number + 1 
                                                        //full moves is how many moves have been made
    }                                                   //so the current turn should be one more than that
    
    return;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    setAIPlayer(AI_PLAYER);
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

    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"); 
    set_white_king_square(&_grid[0][4]);
    set_black_king_square(&_grid[7][4]);
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

bool Chess::isKingInCheck(int playerNumber) 
{
    // Select the correct king's square based on player number
    BitHolder* kingSquare = (playerNumber == 0) ? White_King_Square : Black_King_Square;
    
    if (!kingSquare) return false; // Sanity check
    
    // Get king's coordinates 
    int kingRow = 10 - (kingSquare->getPosition().y / 64);
    int kingCol = kingSquare->getPosition().x / 64;
    
    // Directions to check for attacking pieces
    // Rook and Queen movements (horizontal and vertical)
    int rowDirections[4] = {0, 0, 1, -1};
    int colDirections[4] = {1, -1, 0, 0};
    
    // Check rook and queen attacks along horizontal and vertical lines
    for (int i = 0; i < 4; ++i) {
        int currentRow = kingRow;
        int currentCol = kingCol;
        
        while (true) {
            currentRow += rowDirections[i];
            currentCol += colDirections[i];
            
            // Break if out of board bounds
            if (currentRow < 1 || currentRow > 8 || currentCol < 1 || currentCol > 8) 
                break;
            
            BitHolder& checkSquare = _grid[currentRow - 1][currentCol - 1];
            
            // If square is not empty
            if (!checkSquare.empty()) {
                Bit* piece = checkSquare.bit();
                
                // Check for rook or queen of opposite color
                if (piece->getOwner()->playerNumber() != playerNumber) {
                    if (piece->gameTag() == Rook || piece->gameTag() == Queen) {
                        return true;
                    }
                }
                
                // Stop scanning this direction if a piece is found
                break;
            }
        }
    }
    
    // Diagonal directions for bishop and queen
    int diagRowDirections[4] = {1, 1, -1, -1};
    int diagColDirections[4] = {1, -1, 1, -1};
    
    // Check bishop and queen attacks along diagonals
    for (int i = 0; i < 4; ++i) {
        int currentRow = kingRow;
        int currentCol = kingCol;
        
        while (true) {
            currentRow += diagRowDirections[i];
            currentCol += diagColDirections[i];
            
            // Break if out of board bounds
            if (currentRow < 1 || currentRow > 8 || currentCol < 1 || currentCol > 8) 
                break;
            
            BitHolder& checkSquare = _grid[currentRow - 1][currentCol - 1];
            
            // If square is not empty
            if (!checkSquare.empty()) {
                Bit* piece = checkSquare.bit();
                
                // Check for bishop or queen of opposite color
                if (piece->getOwner()->playerNumber() != playerNumber) {
                    if (piece->gameTag() == Bishop || piece->gameTag() == Queen) {
                        return true;
                    }
                }
                
                // Stop scanning this direction if a piece is found
                break;
            }
        }
    }
    
    // Knight check
    int knightMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    
    for (auto& move : knightMoves) {
        int checkRow = kingRow + move[0];
        int checkCol = kingCol + move[1];
        
        // Check if knight move is within board
        if (checkRow >= 1 && checkRow <= 8 && checkCol >= 1 && checkCol <= 8) {
            BitHolder& checkSquare = _grid[checkRow - 1][checkCol - 1];
            
            if (!checkSquare.empty()) {
                Bit* piece = checkSquare.bit();
                
                // Check for knight of opposite color
                if (piece->getOwner()->playerNumber() != playerNumber && 
                    piece->gameTag() == Knight) {
                    return true;
                }
            }
        }
    }
    
    // Pawn check (different for white and black)
    int pawnRowDirection = (playerNumber == 0) ? 1 : -1;
    int pawnCheckRows[2] = {kingRow + pawnRowDirection, kingRow + pawnRowDirection};
    int pawnCheckCols[2] = {kingCol - 1, kingCol + 1};
    
    for (int i = 0; i < 2; ++i) {
        int checkRow = pawnCheckRows[0];
        int checkCol = pawnCheckCols[i];
        
        // Check if pawn move is within board
        if (checkRow >= 1 && checkRow <= 8 && checkCol >= 1 && checkCol <= 8) {
            BitHolder& checkSquare = _grid[checkRow - 1][checkCol - 1];
            
            if (!checkSquare.empty()) {
                Bit* piece = checkSquare.bit();
                
                // Check for pawn of opposite color
                if (piece->getOwner()->playerNumber() != playerNumber && 
                    piece->gameTag() == Pawn) {
                    return true;
                }
            }
        }
    }
    
    // No check found
    return false;
}

bool Chess::doesMoveResolveCheck(Bit& bit, BitHolder& src, BitHolder& dst) 
{   
    Bit* originalDstPiece = nullptr;

    //if check is being resolved with a capture
    if(dst.bit() != nullptr)
    {
        originalDstPiece = dst.bit();
    }

    //hold on to the bit while it is temporarily removed from its holders 
    Bit* movingPiece = &bit;

    //set holders bits to nullptr so the piece isnt in multiple places at once
    src.clearBit();
    dst.clearBit();

    //set piece in the destination
    dst.setBit(movingPiece) ;

    //if the piece is a king, it must have its location temporarily updated to properly handle check
    if(bit.gameTag() == King)
    {
        (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(&dst) : set_black_king_square(&dst) ;
    }

    //is the king still in check after the move is made?
    bool stillInCheck = isKingInCheck(getCurrentPlayer()->playerNumber());

    //undo the move and set everything back to its orignal place
    dst.clearBit();
    dst.setBit(originalDstPiece);
    src.setBit(movingPiece);

    //put king square back
    if(bit.gameTag() == King)
    {
        (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(&src) : set_black_king_square(&src) ;
    }

    return !stillInCheck;
}

//TODO: fix kings being able to move and still be in check
//the rest of check resolution works as intended, kings work
//almost correct but can still be moved into a check spot on occasion 
bool Chess::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst, bool winner_function)
{

    if(bit.getOwner()->playerNumber() == getCurrentPlayer()->playerNumber())
    {
        //bool check = isKingInCheck(getCurrentPlayer()->playerNumber()); //if the king is in check, the check must be resolved

        if(!dst.empty() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()) //under no circumstance should a piece be placed in an occupied square owned by the same player
        {
            return false;
        }

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


                if(destination_row == 8) //queen promotion
                {
                    
                    if( (ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                    {
                        Bit *promoted_queen = PieceForPlayer(bit.getOwner()->playerNumber(), Queen); //create a queen
                        bit.~Bit(); //destroy the pawn

                        promoted_queen->setPosition(dst.getPosition());
                        promoted_queen->setGameTag(Queen);
                        dst.setBit(promoted_queen);
                    }

                }

                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }

                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                {
                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
                }
                return true;
            }

            //white pawn can move forward 2 in the starting row
            if(starting_row == 2 && bit.getOwner()->playerNumber() == 0)
            {
                if(destination_row == starting_row + 2 && destination_column == starting_column)
                {
                    if(!dst.empty() ||!_grid[destination_row - 2][destination_column-1].empty()) //cant move to an occupied spot or jump a piece to move forward 2
                    {
                        return false;
                    }

                    if(!doesMoveResolveCheck(bit, src, dst))
                    {
                        return false;
                    }

                    if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                    {
                        set_passant(&bit); //pawn is now passantable
                        if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                        {
                            src.clearBit();
                            dst.setBit(&bit);
                            bit.setPosition(dst.getPosition());
                        }
                        endTurn();
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

                if(destination_row == 1) //queen promotion
                {
                    if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                    {
                        Bit *promoted_queen = PieceForPlayer(bit.getOwner()->playerNumber(), Queen); //create a queen
                        bit.~Bit(); //destroy the pawn

                        promoted_queen->setPosition(dst.getPosition());
                        promoted_queen->setGameTag(Queen);
                        dst.setBit(promoted_queen);
                    }

                }
                
                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }

                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                {
                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
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
                    
                    if(!doesMoveResolveCheck(bit, src, dst))
                    {
                        return false;
                    }

                    if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                    {
                        set_passant(&bit); //pawn is now passantable
                        if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                        {
                            src.clearBit();
                            dst.setBit(&bit);
                            bit.setPosition(dst.getPosition());
                        }
                        endTurn();
                    }
                    return true;
                }
            }

            //pawn can capture
            if( ( starting_column + 1 == destination_column || starting_column - 1 == destination_column) && ( dst.bit() || passantable_pawn )) // capturing can happen either left of right
            {
                //white pawn
                if(starting_row + 1 == destination_row) //capture has to happen one row above
                {

                    if( (dst.bit() && dst.bit()->getOwner()->playerNumber() == 1 ) ) //if pawn is white and piece to capture is black
                    {
                        if(!doesMoveResolveCheck(bit, src, dst))
                        {
                            return false;
                        }

                        if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                        {
                            dst.destroyBit();
                            clear_passant(); 
                            if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                            {
                                src.clearBit();
                                dst.setBit(&bit);
                                bit.setPosition(dst.getPosition());
                            }
                            endTurn();

                            if(destination_row == 8) // queen promotion
                            {
                                Bit *promoted_queen = PieceForPlayer(bit.getOwner()->playerNumber(), Queen); //create a queen
                                bit.~Bit(); //destroy the pawn

                                promoted_queen->setPosition(dst.getPosition());
                                promoted_queen->setGameTag(Queen);
                                dst.setBit(promoted_queen);
                            }
                            
                        }
                        return true;
                    }

                    if(_grid[starting_row - 1][destination_column - 1].bit() == passant_pawn() && bit.getOwner()->playerNumber() == 0) // en passant
                    {
                        if(!doesMoveResolveCheck(bit, src, dst))
                        {
                            return false;
                        }

                        if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                        {
                            _grid[starting_row - 1][destination_column - 1].destroyBit();
                            clear_passant();
                            if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                            {
                                src.clearBit();
                                dst.setBit(&bit);
                                bit.setPosition(dst.getPosition());
                            }
                            endTurn();
                        }
                        return true;
                    }

                    return false;
                }

                //black pawn
                if(starting_row - 1 == destination_row) //capture has to happen one row below
                {
                    if(dst.bit() && dst.bit()->getOwner()->playerNumber() == 0)
                    {
                        if(!doesMoveResolveCheck(bit, src, dst))
                        {
                            return false;
                        }

                        if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                        {
                            dst.destroyBit();
                            clear_passant();
                            if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                            {
                                src.clearBit();
                                dst.setBit(&bit);
                                bit.setPosition(dst.getPosition());
                            }
                            endTurn();

                            if(destination_row == 1) // queen promotion
                            {
                                Bit *promoted_queen = PieceForPlayer(bit.getOwner()->playerNumber(), Queen); //create a queen
                                bit.~Bit(); //destroy the pawn

                                promoted_queen->setPosition(dst.getPosition());
                                promoted_queen->setGameTag(Queen);
                                dst.setBit(promoted_queen);
                            }
                        }
                        return true;
                    }

                    if(_grid[starting_row - 1][destination_column - 1].bit() == passant_pawn() && bit.getOwner()->playerNumber() == 1) // en passaant
                    {
                        if(!doesMoveResolveCheck(bit, src, dst))
                        {
                            return false;
                        }

                        if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                        {
                            _grid[starting_row - 1][destination_column - 1].destroyBit();
                            clear_passant();
                            if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                            {
                                src.clearBit();
                                dst.setBit(&bit);
                                bit.setPosition(dst.getPosition());
                            }
                            endTurn();
                        }
                        return true;
                    }
                    return false;
                }
                return false;
            }
            return false;
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
                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }

                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                {
                    if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
                    {
                        dst.destroyBit();
                    }
                    bit.movedFromStart();

                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
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

                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }

                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                {
                    if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
                    {
                    dst.destroyBit();
                    } 

                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
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
                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }
                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                {
                    if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
                    {
                        dst.destroyBit();
                    }

                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
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
                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }

                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                {
                    if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
                    {
                        dst.destroyBit();
                    }
                    bit.movedFromStart();
                    getCurrentPlayer()->playerNumber() ? set_white_king_square(&dst) : set_black_king_square(&dst) ;
                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
                }
                return true;
            
            }

            //castling 
            if(starting_row == destination_row && column_difference == 2) //valid castling squares for king
            {
                if(!bit.hasMovedFromStart()) //cannot castle if king has moved
                {
                    if(destination_column - starting_column > 0) //castling to right
                    {
                        if(!_grid[starting_row - 1][7].bit()->hasMovedFromStart()) //if the rook to castle to hasnt moved
                        {
                            if(!doesMoveResolveCheck(bit, src, dst))
                            {
                                return false;
                            }

                            if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                            {
                                Bit *rook_to_castle = new Bit(*_grid[starting_row - 1][7].bit()); //create a new rook to castle with

                                _grid[starting_row - 1][7].destroyBit(); //remove the old rook

                                rook_to_castle->setPosition(_grid[starting_row - 1][5].getPosition());
                                _grid[starting_row - 1][5].setBit(rook_to_castle);

                                rook_to_castle->movedFromStart();
                                bit.movedFromStart();
                                getCurrentPlayer()->playerNumber() ? set_white_king_square(&dst) : set_black_king_square(&dst) ;
                                clear_passant();
                                if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                                {
                                    src.clearBit();
                                    dst.setBit(&bit);
                                    bit.setPosition(dst.getPosition());
                                }
                                endTurn();
                            }
                            return true;
                        }
                    }

                    if(destination_column - starting_column < 0) //castling to left
                    {
                        if(!_grid[starting_row - 1][0].bit()->hasMovedFromStart()) //if the rook to castle to hasnt moved
                        {
                            if(!doesMoveResolveCheck(bit, src, dst))
                            {
                                return false;
                            }

                            if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                            { 
                                Bit *rook_to_castle = new Bit(*_grid[starting_row - 1][0].bit()); //create a new rook to castle with

                                _grid[starting_row - 1][0].destroyBit(); //remove the old rook

                                rook_to_castle->setPosition(_grid[starting_row - 1][3].getPosition());
                                _grid[starting_row - 1][3].setBit(rook_to_castle);

                                rook_to_castle->movedFromStart();
                                bit.movedFromStart();
                                getCurrentPlayer()->playerNumber() ? set_white_king_square(&dst) : set_black_king_square(&dst) ;
                                clear_passant();
                                if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                                {
                                    src.clearBit();
                                    dst.setBit(&bit);
                                    bit.setPosition(dst.getPosition());
                                }
                                endTurn();
                            }
                            return true;
                        }
                    }
                }
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

                if(!doesMoveResolveCheck(bit, src, dst))
                {
                    return false;
                }

                if((ImGui::IsMouseReleased(0) || getCurrentPlayer()->playerNumber() == getAIPlayer()) && !winner_function)
                { 
                    if(dst.bit() != nullptr && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()) //capture piece if there is one
                    {
                        dst.destroyBit();
                    }

                    clear_passant();
                    if(getAIPlayer() == getCurrentPlayer()->playerNumber())
                    {
                        src.clearBit();
                        dst.setBit(&bit);
                        bit.setPosition(dst.getPosition());
                    }
                    endTurn();
                }
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) 
{
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
    int currentPlayer = getCurrentPlayer()->playerNumber();
    int opposingPlayer = (currentPlayer == 0) ? 1 : 0;

    bool isInCheck = isKingInCheck(currentPlayer);
    bool legalMoveExists = false;

    // Iterate through board to find all the current player's pieces
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            BitHolder* src = nullptr;

            // Find pieces owned by the current player
            if(!_grid[row][col].empty() && _grid[row][col].bit()->getOwner()->playerNumber() == currentPlayer)
            {
                src = &_grid[row][col];
            }

            if(src != nullptr) 
            {
                // Look for a viable move for this piece
                for(int dstRow = 0; dstRow < 8; dstRow++)
                {
                    for(int dstCol = 0; dstCol < 8; dstCol++)
                    {
                        BitHolder* dst = &_grid[dstRow][dstCol];

                        // Check if the move is legal (prevents own king from being in check)
                        if(canBitMoveFromTo(*src->bit(), *src, *dst, true))
                        {
                            legalMoveExists = true;
                            break;
                        }
                    }

                    if(legalMoveExists)
                    {
                        break;
                    }
                }

                if(legalMoveExists)
                {
                    break;
                }
            }
        }

        if(legalMoveExists)
        {
            break;
        }
    } 

    // Checkmate: king is in check and no legal moves exist
    if(isInCheck && !legalMoveExists)
    {
        Player* winner = (opposingPlayer == 0) ? White_King_Square->bit()->getOwner() : Black_King_Square->bit()->getOwner();
        ClassGame::EndOfGame(winner);
        return winner;
    }
    // Stalemate: king is NOT in check, but no legal moves exist
    else if(!isInCheck && !legalMoveExists)
    {
        ClassGame::StaleMate();
        return nullptr;
    }

    return nullptr; // No winner yet
}

bool Chess::checkForDraw()
{
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
    if(checkForWinner())
    {
        return;
    }

    //arbitrary low number to be overwritten later
    int bestScore = std::numeric_limits<int>::min(); 
    int depth = 3;

    //move the ai will ultimately make
    BitHolder* bestMoveFrom = nullptr;
    BitHolder* bestMoveTo = nullptr;
    Bit* bestPiece = nullptr;

    //loop through board and find all possible legal moves
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            BitHolder* src = nullptr;
            if(_grid[row][col].bit() && _grid[row][col].bit()->getOwner()->playerNumber() == AI_PLAYER) //find ai pieces
            {
                src = &_grid[row][col];
            

                //find where on the board the AI could move the piece
                for(int dstRow = 0; dstRow < 8; dstRow++)
                {
                    for(int dstCol = 0; dstCol < 8; dstCol++)
                    {
                        BitHolder* dst = &_grid[dstRow][dstCol];
                        
                        if(canBitMoveFromTo(*src->bit(), *src, *dst, true) && doesMoveResolveCheck(*src->bit(), *src, *dst)) //legal move is found
                        {
                            legalMoves.push_back(dst); //add move to possible move list
                            legalMoveStartingPositions.push_back(src); //add position of the move 
                        }
                    }
                }
            }

        }
    }

    //start negamax
    for(int i = 0; i < legalMoves.size(); i++ )
    {
        BitHolder* moveFrom = legalMoveStartingPositions[i];
        BitHolder* moveTo = legalMoves[i];

        //if the move involves a capture store the piece that is being captured
        Bit* originalDstPiece = nullptr;

        if(!moveTo->empty())
        {
            originalDstPiece = moveTo->bit();
        }

        //hold on to the bit while it is temporarily removed from its holders 
        Bit* movingPiece = moveFrom->bit();

        //set holders bits to nullptr so the piece isnt in multiple places at once
        moveFrom->clearBit();
        moveTo->clearBit();

        //set piece in the destination
        moveTo->setBit(movingPiece) ;

        //if the piece is a king, it must have its location temporarily updated to properly handle check
        if(movingPiece->gameTag() == King)
        {
            (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(moveTo) : set_black_king_square(moveTo) ;
        }

        //call negamax on the proposed move
        int score = -negamax(depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false) ;
        //undo the move and set everything back to its orignal place
        moveTo->clearBit();
        moveTo->setBit(originalDstPiece);
        moveFrom->setBit(movingPiece);
        movingPiece->setPosition(moveFrom->getPosition());
        //put king square back
        if(moveTo->gameTag() == King)
        {
            (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(moveFrom) : set_black_king_square(moveFrom) ;
        }

        //update score if move is better than before
        if(score > bestScore)
        {
            bestScore = score;
            bestMoveFrom = moveFrom;
            bestMoveTo = moveTo;
            bestPiece = moveFrom->bit();
        }
    }

    //make the best move available
    canBitMoveFromTo(*bestMoveFrom->bit(), *bestMoveFrom, *bestMoveTo, false);

    //reset legal move vectors
    legalMoves.clear();
    legalMoveStartingPositions.clear();
    return;

}

int Chess::negamax(int depth, int alpha, int beta, bool isMaximizingPlayer)
{
    if(depth == 0 || checkForWinner())
    {
        return evaluateBoard(stateString());
    }

    int bestScore = std::numeric_limits<int>::min();
    int bestMoveIndex = -1;

    //evaluate all the legal moves
    for(int i = 0; i < legalMoves.size(); i++)
    {
        BitHolder* moveFrom = legalMoveStartingPositions[i];
        BitHolder* moveTo = legalMoves[i];

        Bit* originalDstPiece = moveTo->empty() ? nullptr : moveTo->bit();
        Bit* movingPiece = moveFrom->bit();

        // Bonus for capturing pieces
        int moveToRow = 10 - moveTo->getPosition().y;
        int moveToCol = moveTo->getPosition().x;

        int captureBonus = originalDstPiece ? evaluateScores[stateString()[ (moveToRow - 1) * 8 + (moveToCol - 1)]] : 0;

        // Penalty for moving a piece that can be captured
        int captureRisk = evaluateCaptureRisk(movingPiece, moveFrom, moveTo);

        moveFrom->clearBit();
        moveTo->clearBit();
        moveTo->setBit(movingPiece);
        // Update king square if necessary
        if(moveFrom == ((AI_PLAYER == 1) ? Black_King_Square : White_King_Square))
        {
            (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(moveTo) : set_black_king_square(moveTo);
        }

        int evaluation = -negamax(depth - 1, -beta, -alpha, !isMaximizingPlayer);
        
        // Incorporate capture bonus and capture risk into evaluation
        evaluation += (captureBonus * 50) - (captureRisk * 100);

        // Penalize moving the same piece repeatedly
        if (i > 0 && legalMoveStartingPositions[i-1]->bit() == movingPiece) {
            evaluation -= 50;  // Discourage repetitive moves
        }

        // Restore board state
        moveTo->clearBit();
        if(originalDstPiece)
        {
            moveTo->setBit(originalDstPiece);
        }
        
        moveFrom->setBit(movingPiece);
    
        if(moveTo->gameTag() == King)
        {
            (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(moveFrom) : set_black_king_square(moveFrom);
        }

        // Track best move
        if (evaluation > bestScore) {
            bestScore = evaluation;
            bestMoveIndex = i;
        }

        alpha = std::max(alpha, evaluation);
        if(alpha >= beta)
            break;
    }

    return bestScore;
}

int Chess::evaluateCaptureRisk(Bit* movingPiece, BitHolder* start, BitHolder* destination) 
{
    if (!movingPiece || !destination)
    {
        return 0;
    }

    int captureRisk = 0;
    int playerNumber = movingPiece->getOwner()->playerNumber();
    int opponentPlayer = (playerNumber == 0) ? 1 : 0;

    // Save the current piece at the destination to simulate the move
    Bit* originalDstPiece = nullptr;
    if(destination->bit())
    {
        originalDstPiece = destination->bit();
    }
    start->clearBit();
    destination->clearBit();
    destination->setBit(movingPiece);

    // Update king square if necessary
    if(start == ((AI_PLAYER == 1) ? Black_King_Square : White_King_Square))
    {
        (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(destination) : set_black_king_square(destination);
    }
    
    // Simulate all opponent moves to see if the destination is under attack
    for (int row = 0; row < 8; row++) 
    {
        for (int col = 0; col < 8; col++) 
        {
            BitHolder* src = &_grid[row][col];
            if (!src->empty() && src->bit()->getOwner()->playerNumber() == opponentPlayer) 
            {
                // Check if the opponent's piece can move to the destination
                if (canBitMoveFromTo(*src->bit(), *src, *destination, true)) 
                {
                    // Add the value of the piece at risk to the penalty
                    captureRisk += evaluateScores[movingPiece->gameTag()];
                }
            }
        }
    }

    // Restore the original state
    destination->clearBit();
    if(originalDstPiece)
    {
        destination->setBit(originalDstPiece);
        originalDstPiece->setPosition(destination->getPosition());
    }
    start->setBit(movingPiece);
    movingPiece->setPosition(start->getPosition());
    if(movingPiece->gameTag() == King)
    {
        (getCurrentPlayer()->playerNumber() == 0) ? set_white_king_square(start) : set_black_king_square(start);
    }

    return captureRisk;
}


int Chess::evaluateBoard(std::string state) {
    int score = 0;
    for (int i=0; i<64; i++) {
        score += evaluateScores[state[i]];
    }
    for (int i=0; i<64; i++) {
        char piece = state[i];
        int j = 63 - i;
        switch (piece) {
            case 'N': // Knight
                score += knightTable[j];
                break;
            case 'n':
                score -= knightTable[63 - j];
                break;
            case 'P': // Knight
                score += pawnTable[j];
                break;
            case 'p':
                score -= pawnTable[63 - j];
                break;
            case 'K': // Knight
                score += kingTable[j];
                break;
            case 'k':
                score -= kingTable[63 - j];
                break;
            case 'R': // Knight
                score += rookTable[j];
                break;
            case 'r':
                score -= rookTable[63 - j];
                break;
            case 'Q': // Knight
                score += queenTable[j];
                break;
            case 'q':
                score -= queenTable[63-j];
                break;
        }
    }
    return score;
} 

