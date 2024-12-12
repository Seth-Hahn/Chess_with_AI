#pragma once
#include "Game.h"
#include "ChessSquare.h"

const int pieceSize = 64;

enum ChessPiece {
    NoPiece = 0,
    Pawn = 1,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

//
// the main game class
//
class Chess : public Game
{
public:
    Chess();
    ~Chess();

    // set up the board
    void        setUpBoard() override;
    void        FENtoBoard(std::string FEN_string); 

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder& holder) override;
    bool        canBitMoveFrom(Bit& bit, BitHolder& src) override;
    bool        canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst, bool winner_function) override;
    void        bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    void        stopGame() override;
    BitHolder& getHolderAt(const int x, const int y) override { return _grid[y][x]; }

	void        updateAI() override;
    bool        gameHasAI() override { return true; };

    // en passant functions
    void       set_passant(Bit* bit) {passantable_pawn = bit;};
    Bit*       passant_pawn() {return passantable_pawn;} ;
    void       clear_passant() {passantable_pawn = nullptr; };

    // draw functions
    void       set_half_moves(int num) {half_moves = num;} ;
    int        number_of_half_moves() {return half_moves;} ;

    //Check functions
    bool       isKingInCheck(int playerNumber);
    bool       doesMoveResolveCheck(Bit& bit, BitHolder& src, BitHolder& dst); //return the bit that resloves check if true, nullptr if false

    void       set_white_king_square(BitHolder* square) {White_King_Square = square; } ;
    void       set_black_king_square(BitHolder* square) {Black_King_Square = square; } ;

    BitHolder* White_King_Position() {return White_King_Square;} ;
    BitHolder* Black_King_Position() {return Black_King_Square; } ;
private:
    Bit *       PieceForPlayer(const int playerNumber, ChessPiece piece);
    const char  bitToPieceNotation(int row, int column) const;

    ChessSquare      _grid[8][8];

    //used for checking en passant
    Bit* passantable_pawn = nullptr;

    //track king spot for check situations
    BitHolder* White_King_Square;
    BitHolder* Black_King_Square;
    //used for draws
    int half_moves;
};

