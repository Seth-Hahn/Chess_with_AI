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

-----------------------------------

### Chess.cpp

## setUpBoard
In this function, I created logic to set up the pieces to the initial game state of a standard chess game. 
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

# capturing 
Capturing logic is incredibly simple at this point, though a few more tweaks are planned for it to make it more complex. Currently, if a capturable piece is hovered over by the 
capturing piece, the piece to capture is instantly destroyed and the spot becomes available for the capturing piece to occupy.