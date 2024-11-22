# Chess AI Implementation Project

![Chess Board](https://raw.githubusercontent.com/zaphodgjd/class-chess-123/main/chess/w_king.png)

## 🎯 Project Overview
This repository contains a skeleton implementation of a Chess AI engine written in C++. The project is designed to teach fundamental concepts of game AI, including board representation, move generation, and basic game tree search algorithms.

### 🎓 Educational Purpose
This project serves as a teaching tool for computer science students to understand:
- Game state representation
- Object-oriented design in C++
- Basic AI concepts in game playing
- Bitboard operations and chess piece movement
- FEN (Forsyth–Edwards Notation) for chess position representation

## 🔧 Technical Architecture

### Key Components
1. **Chess Class**: Core game logic implementation
   - Board state management
   - Move validation
   - Game state evaluation
   - AI player implementation

2. **Piece Representation**
   - Unique identifiers for each piece type
   - Sprite loading and rendering
   - Movement pattern definitions

3. **Board Management**
   - 8x8 grid representation
   - Piece positioning
   - Move history tracking
   - FEN notation support

## 🚀 Getting Started

### Prerequisites
- C++ compiler with C++11 support or higher
- Image loading library for piece sprites
- CMake 3.10 or higher

### Building the Project
```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests
```bash
./chess_tests
```

## 📝 Implementation Details

### Current Features
- Basic board setup and initialization
- Piece movement validation framework
- FEN notation parsing and generation
- Sprite loading for chess pieces
- Player turn management

### Planned Features
- [ ] AI move generation
- [ ] Position evaluation
- [ ] Opening book integration
- [ ] Advanced search algorithms
- [ ] Game state persistence

## 🔍 Code Examples

### Piece Movement Validation
```cpp
bool Chess::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst) {
    // TODO: Implement piece-specific movement rules
    return false;
}
```

### FEN Notation Generation
```cpp
const char Chess::bitToPieceNotation(int row, int column) const {
    if (row < 0 || row >= 8 || column < 0 || column >= 8) {
        return '0';
    }
    // Implementation details for FEN notation
}
```

## 📚 Class Assignment Structure

### Phase 1: Board Setup
- Implement piece placement
- Setup initial board state
- Validate board representation

### Phase 2: Move Generation
- Implement basic piece movements
- Add move validation
- Implement special moves (castling, en passant)

### Phase 3: AI Implementation
- Develop position evaluation
- Implement minimax algorithm
- Add alpha-beta pruning
- Basic opening book

## 🤝 Contributing
Students are encouraged to:
1. Fork the repository
2. Create a feature branch
3. Implement assigned components
4. Submit their fork for review

## 🔒 Code Style and Standards
- Use consistent indentation (4 spaces)
- Follow C++ naming conventions
- Document all public methods
- Include unit tests for new features

## 📄 License
This project is licensed under the MIT License.

## 👥 Contributors
- [Your Name] - Initial work
- [Student Names] - Implementation and testing

## 🙏 Acknowledgments
- Chess piece sprites from [Wikipedia](https://en.wikipedia.org/wiki/Chess_piece)
- Original game engine framework by [ocornut](https://github.com/ocornut/imgui)

---
*This README is part of an educational project and is intended to serve as an example of good documentation practices.*
---

-----------------------------------
My Work So Far
-----------------------------------

### Game.cpp

# findDropTarget
in the findDropTarget function, on line 172, I reversed the order of the "and" conditional so that CanBitMoveFromTo
was checked before canDropBitAtPoint. This helped to streamline the capture logic that was implemented into the
CanBitMoveFromTo function. 

### Game.h

# setCurrentTurnNo
added a function: "setCurrentTurnNo" to be used by the FENtoBoard function since
part of the fen string includes how many turns have been made in the game so far

-----------------------------------

### Bit.h

# _movedFromStart
added a bool "_movedfromstart" to help handle castling to the bit class. 

# movedfromStart
added a function: "movedfromstart()" to be called when a rook or king has moved from its initial position. This function sets the bool to true
# hasmovedfromstart
added a function: "hasMovedFromStart()" which simply returns the bool. This is used for castling handling 
# notMoved
added a function: "notMoved()" which is used by the FENtoBoard function to handle
castling logic. This function simply sets the flag that indicates whether a piece
has moved from its starting position to false, so that castling can be set up on 
the right pieces.
-----------------------------------

### Chess.h 

added the FENtoBoard function which replaces the old implementation for setting up the board (which is still described below)

added an int variable "half moves" and functions taht set and return this variable as the number of half moves left is included in the FEN string. This can be used 
later on to determine draw states

### Chess.cpp

## FENtoBoard
This function takes the string passed throught the function and uses it as a stream
of data, extracting the part of the string that acts as the board representation.

row and column variables are set up to handle indexing on the chess board, and then the fen string is iterated through character by character. If the character is a '/', then the current row is finished, and the column needs to be reset to 0 to start at the first position in the next row. If the character is a digit, that indicates x number of empty spaces, thus the character (- '0' for proper handling)
is added to column to skip the empty spaces.

If the character is not a digit or a '/', then it must be a piece to place on the board, thus a new piece is created, and the owner of the piece is checked using the isupper() function (uppercase for white, lowercase for black) and stored as a bool. A switch statement then checks each of the possible cases (p,r,n,b,q,k) and creates the correct piece according to the character. Since the owner of the piece is already stored in the bool isWhite, the tolower() function is called on the character to minimize the number of switch cases, and the isWhite bool determines the color of the piece. By default, rooks and kings have the movedfromstart() flag called to indicate that castling is unavailable, so that castling can be set up on individual rooks later on.

after the switch statement, the piece is placed on the board as long as the piece isnt set to nullptr. Of note, due to the FEN string starting from black side and working down the board to the white side, the row variable needs to be subtracted from 7 to flip the board and put pieces in there correct positions.

after the board representation is complete, the rest of the fen string can be checked. The first check that occurs is whether or not the active player is equal to "w" or "b", if neither are set, then the FEN string must have just been the board representation and thus it is unncecessary to do the rest of the setup. 

If it is a complete FEN string, the first thing that happens is that the current player is set. Since a turn system hasnt been implemented yet, this code has been left empty for now.

Next, castling availability is handled by a for loop and switch statement similar to how the board representation is handled. This simply switches through the 4 possible cases, and sets the corressponding rook and king's movedfromstart flags to false so that they are able to castle (this is why they were set to true during board set up). The En Passant target is set by getting the column and row index as indicated from the FEN string. With how En Passant is implemented in my code, the pawn that is passable needs to be stored, not the position that a capturing pawn would move to if it were to capture via en passant. Thus, since a passantable pawn could only be in the 3rd or 6th rows (white pawn and black pawn respectively), the set_passant function is called to the correct row of the grid depending on pawn color, and the column index is taken directly from the fen string

The rest of the function simply sets the number of half moves with the function set_half_moves(), and the current turn number is set to full_move_number + 1 since the last number of the fen string indicates how many turns have passed, so the current turn is that + 1.
## setUpBoard

New implementation: call the FENtoBoard function as such:

    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"); 

this will handle the entirety of board set up.

OLD IMPLEMENTATION: In this function, I created logic to set up the pieces to the initial game state of a standard chess game. 
I started with a for loop that runs twice - once for each player. The main for loop is used to handle things such
as properly setting piece colors, and determining the proper row for a piece based on its color. 

A second for loop, inside of the player loop, is used to initialize pawns for both players. This loop runs 8 times 
per player to give them the right amount of pawns. 

The last loop of this function, also inside the player loop, initializes the rest of the pieces for players. This loop
also runs twice, and helps ensure that rooks, bishops, knights, queens, and kings get placed in their proper positions
around the board. Because there are 2 knights, bishops, and rooks, the loop runs twice for each piece, and the queen 
and king only get handled once per player.

The function ends by calling StartGame().

-------------------------------------

## canBitMoveFromTo
This function starts by pulling the columns, and rows of the starting and destination positions. The column index numbers have to be divided by 64
to properly index from 1-8, while the row indexes additionally need to be subtracted from 10 to get the proper position on the board.

This function then uses if statements to check which piece is being played, with the logic for each piece's movement being held within their 
respective statements.

# pawn
The pawn logic is more complex than the other pieces, as the valid movement options changes depending on if the pawns are white or black. Because pawns can only move 
forward (relative to the player), white pawns can only move towards higher rows, while black pawns can only move towards lower rows. Pawns are also able to 
move forward 2 spaces if they are in their initial position, so logic was implemented to ensure that that would be a valid move, with further checks to make sure
a pawn moving forward two spaces cannot jump over any pieces. Capturing also occurs differently for pawns, so there are checks to make sure captures only happen in columns
directly right or left (and one row forward) of the capturing pawn.

# pawn promotion
Promoting a pawn to a queen is a simple check to be added to the existing pawn logic. Simply check if the row that the pawn is moving to is the 8th row (for white pawns) or the 1st row (for black pawns). If this is the case, then destroy the pawn once it moves and create a queen in its place.

# rook
Rooks can only move horizontally or vertically, so the Rook logic checks to make sure that the moving rook ends up in the same column OR row that it was in initially. 
multiple if statements are used that handle upwards, downwards, left, and right movement logic. Rooks cannot jump over other pieces, so each movement check looks between the 
squares of the destination and the starting position and holds an index value of pieces that are in the way. If the destination is past the blocking piece (based on the index value), it cannot jump over that piece. Additional logic had to be implemented to ensure that the rook itself was not considered to be in its own way.

# bishop
The bishop takes the absolute value of the destination column - starting column, adn the destination row - starting row. Valid diagonal moves exist where the absolute values 
of these are equal (that is, abs(columns) == abs(rows)). To handle diagonals in each direction, the question mark operator is used to handle direction based on where the 
destination is relative to the start. A while loop then checks the squares along the diagonal path to see if any pieces are in the way (and thus cannot be moved past). If 
a blocking piece is found, its index is stored to be checked against later. An if statement checks if the blocking pieces were set, and if they were, further logic ensures
that the bishop cannot move past a piece that is in the way

# knight
The knight uses similar logic to the bishop, using the absolute value of row and column differences to check for valid moves. In this case, either the row difference needs to be 2 and the column difference needs to be 1 OR the row difference needs to be 1 and the column difference needs to be 2. This ensures proper L shaped movement. Since knights can
jump over pieces, no logic needed to be implemented that checks if a piece is blocking the path. 

# queen 
The queen is simply a combination of the Rook and Bishop, and as such combines their logic to validate its own moves.

# king
The king uses absolute values of row and column differences as well. Since the king can only move one space around itself in any direction. The only check that needs to be done
is that both the column and row difference is less than or equal to one, thus it is only moving one square from its starting point. 

# castling
Castling can only happen when the column difference is 2 since the king has 
to move 2 spaces to complete this move.  The king cannot castle if it has moved from the start, so an if statement check if that is the case.

Once it is determined that the king hasnt moved, the next check is whether it is a queen side castle or king side castle. This is a simple check to see if (destination column - starting column) is positive or negative. positive is a king side castle, negative is queen side. As long as the mouse is released (aka the move is accepted), then a new rook is created from the old one, the old one is destroyed, and the new rook is placed in its proper post-castling position. 

# capturing
NEW IMPLEMENTATION: The old implementation was buggy and did not allow you to change your mind, if a piece was hovered over that was capturable than it would be captured instantly. A simple change helped solve this, and all that needed to happen was before confirming a move, check if the mouse had been released using the ImGui function ismousereleased(). This solved the previous bugs with capturing and allows the player to change their mind

OLD IMPLEMENTATION: Capturing logic is incredibly simple at this point, though a few more tweaks are planned for it to make it more complex. Currently, if a capturable piece is hovered over by the capturing piece, the piece to capture is instantly destroyed and the spot becomes available for the capturing piece to occupy.

# En Passant
Since En Passant can only happen if a pawn has moved forward 2 spaces from the starting position, the set_passant flag is called on any pawn that moves forward twice. This stores the location of a passantable pawn in the chess class, so it can be checked agaisnt later. 
    
Since pawns are the only piece that can capture via en passant, a simple update needed to be made to the pawn capturing logic. Now, when a pawn tries to move to a square via en passant capture, the piece directly behind the square to move to is checked against the stored passant pawn. If they match, then the en passant is valid and the pawn can capture via en passant. 

Since en passant can only happen the move immediately after a pawn goes forward two spaces. A clear_passant function is called after any move is made so that the en passant cannot happen later on.

<img width="1269" alt="Screenshot 2024-11-13 at 2 02 41 PM" src="https://github.com/user-attachments/assets/a9811d89-07de-4289-86bf-a510fb2be0a8">

<img width="1255" alt="Screenshot 2024-11-14 at 2 53 26 PM" src="https://github.com/user-attachments/assets/8405c303-7266-41ee-99ec-43a8adaba623">


