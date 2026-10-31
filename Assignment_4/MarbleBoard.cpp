/*
 * File: MarbleBoard.cpp
 * ---------------------
 *
 *  MarbleBoard.cpp defines methods for the MarbleBoard class.
 *
 *  We wrote several functions to modify the board and look for valid moves
 *  so that we could implement a recursive solver.
 *
 *  Cooper Ericksen & Odelia Kislyuk
 *  2026-10-30
 */
#include "MarbleBoard.h"
#include "set.h"
#include "SimpleTest.h"
using namespace std;

/*
 * ::findPossibleMoves loops through the marbleGrid and 
 * checks to see if each spot in the grid is a marble.
 * If it is a marble, it looks and adds any valid
 * jumps in the 4 possible directions from that marble.
 */
Vector<Move> MarbleBoard::findPossibleMoves() const {
    Vector<Move> moves;

    Grid<Marble> marbleGrid = toGrid();

    for(int row = 0; row < marbleGrid.numRows(); row++) {
        for(int col = 0; col < marbleGrid.numCols(); col++) {
            Marble currentMarble = marbleGrid[row][col];

            // If invalid or empty, skip and keep going
            if (currentMarble == MARBLE_INVALID || currentMarble == MARBLE_EMPTY) {
                continue;
            }

            // check above
            if (row > 1 && marbleGrid[row-1][col] == MARBLE_OCCUPIED && marbleGrid[row-2][col] == MARBLE_EMPTY) {
                Move newMove;
                newMove.start = {row, col};
                newMove.end = {row-2, col};

                moves += newMove;
            }
            // check below
            if (row < marbleGrid.numRows() - 2 && marbleGrid[row+1][col] == MARBLE_OCCUPIED && marbleGrid[row+2][col] == MARBLE_EMPTY) {
                Move newMove;
                newMove.start = {row, col};
                newMove.end = {row+2, col};

                moves += newMove;
            }
            // check left
                if (col > 1 && marbleGrid[row][col-1] == MARBLE_OCCUPIED && marbleGrid[row][col-2] == MARBLE_EMPTY) {
                Move newMove;
                newMove.start = {row, col};
                newMove.end = {row, col-2};

                moves += newMove;
            }
            // check right
            if (col < marbleGrid.numCols() - 2 && marbleGrid[row][col+1] == MARBLE_OCCUPIED && marbleGrid[row][col+2] == MARBLE_EMPTY) {
                Move newMove;
                newMove.start = {row, col};
                newMove.end = {row, col+2};

                moves += newMove;
            }
        }
    }

    return moves;
}

/*
 * ::makeMove takes in a move and updates the ending location
 * to be a marble, the middle location to be empty,
 * and the starting location to be empty.
 * It also updates the count of _numMarbles as one marble was
 * deleted by the jump.
 */
void MarbleBoard::makeMove(Move move) {
    // change ending location to occupied
    _grid[move.end] = MARBLE_OCCUPIED;

    // remove jumped marble
    int rowDifference = (move.start.row + move.end.row) / 2;
    int colDifference = (move.start.col + move.end.col) / 2;
    _grid[{rowDifference, colDifference}] = MARBLE_EMPTY;
    
    // change starting location to empty
    _grid[move.start] = MARBLE_EMPTY;

    // subtract 1 from marble count
    _numMarbles--;
}

/*
 * ::undoMove takes in a move just like ::makeMove and updates
 * the ending location to be empty, the middle location to 
 * be a marble, and the starting location to be marble.
 * It also updates the count of _numMarbles as one marble was
 * reinstated by the undo.
 */
void MarbleBoard::undoMove(Move move) {
    // change ending location to empty
    _grid[move.end] = MARBLE_EMPTY;
    
    // remove jumped marble
    int rowDifference = (move.start.row + move.end.row) / 2;
    int colDifference = (move.start.col + move.end.col) / 2;
    _grid[{rowDifference, colDifference}] = MARBLE_OCCUPIED;
    
    // change starting location to occupied
    _grid[move.start] = MARBLE_OCCUPIED;

    // add 1 to marble count
    _numMarbles++;
}









/* The functions below have already been written for you! */

MarbleBoard::MarbleBoard() : MarbleBoard(fullBoard()) {}   // default constructor creates the starting board

MarbleBoard::MarbleBoard(const Grid<Marble>& grid) {
    _grid = grid;
    _numMarbles = countMarbles(grid);
}

int MarbleBoard::numMarbles() const {
    return _numMarbles;
}

bool MarbleBoard::isSolved() const {
    return _numMarbles == 1;
}

Marble MarbleBoard::at(GridLocation loc) const {
    return _grid[loc];
}

BoardEncoding MarbleBoard::encode() const {
    return compressBoard(_grid);
}

Grid<Marble> MarbleBoard::toGrid() const {
    return _grid;
}

string MarbleBoard::toString() const {
    return boardToString(_grid);
}


/* * * * * * Test Cases * * * * * */

/*
 * Helper for the tests below: returns the moves as a Set, so that the tests
 * do not depend on the order in which findPossibleMoves returns its moves.
 */
static Set<Move> toSet(const Vector<Move>& moves) {
    Set<Move> result;
    for (const Move& move : moves) {
        result.add(move);
    }
    return result;
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: a lone marble has no moves") {
    MarbleBoard board(loadBoard("marbles/boards/solved.txt"));
    EXPECT_EQUAL(board.findPossibleMoves().size(), 0);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: two marbles side by side can jump either way") {
    MarbleBoard board(boardFromStrings({
        "XX---XX",
        "XX---XX",
        "-------",
        "-OO----",
        "-------",
        "XX---XX",
        "XX---XX"
    }));
    Vector<Move> moves = board.findPossibleMoves();
    Set<Move> expected = { { {3, 1}, {3, 3} },     // left marble jumps right
                           { {3, 2}, {3, 0} } };   // right marble jumps left
    EXPECT_EQUAL(moves.size(), 2);
    EXPECT_EQUAL(toSet(moves), expected);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: no jumping over empty holes, diagonally, off the board, or into corners") {
    // marbles with empty holes between them
    MarbleBoard spaced(boardFromStrings({
        "XXO-OXX",
        "XX---XX",
        "O-O-O-O",
        "-------",
        "O-O-O-O",
        "XX---XX",
        "XXO-OXX"
    }));
    EXPECT_EQUAL(spaced.findPossibleMoves().size(), 0);

    // marbles touching only diagonally
    MarbleBoard diagonals(loadBoard("marbles/boards/no-diagonals.txt"));
    EXPECT_EQUAL(diagonals.findPossibleMoves().size(), 0);

    // the only jumps in this line of marbles would land off the board or in a corner
    MarbleBoard edge(loadBoard("marbles/boards/off-the-edge.txt"));
    EXPECT_EQUAL(edge.findPossibleMoves().size(), 0);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: the full board has exactly four moves, all into the center") {
    MarbleBoard board;
    Vector<Move> moves = board.findPossibleMoves();
    Set<Move> expected = { { {1, 3}, {3, 3} }, { {5, 3}, {3, 3} },
                           { {3, 1}, {3, 3} }, { {3, 5}, {3, 3} } };
    EXPECT_EQUAL(moves.size(), 4);
    EXPECT_EQUAL(toSet(moves), expected);
}

PROVIDED_TEST("MarbleBoard::makeMove and undoMove: jump down into the center, then undo") {
    MarbleBoard board;
    Move move = { {1, 3}, {3, 3} };
    board.makeMove(move);
    EXPECT_EQUAL(board.at({1, 3}), MARBLE_EMPTY);       // jumping marble left its start
    EXPECT_EQUAL(board.at({2, 3}), MARBLE_EMPTY);       // jumped-over marble is gone
    EXPECT_EQUAL(board.at({3, 3}), MARBLE_OCCUPIED);    // jumping marble landed
    EXPECT_EQUAL(board.numMarbles(), 31);

    board.undoMove(move);
    EXPECT_EQUAL(board, MarbleBoard());
    EXPECT_EQUAL(board.numMarbles(), 32);
}

PROVIDED_TEST("MarbleBoard::makeMove and undoMove: jump to the left, then undo") {
    MarbleBoard board(loadBoard("marbles/boards/one-jump.txt"));
    MarbleBoard original = board;
    Move move = { {3, 2}, {3, 0} };
    board.makeMove(move);
    EXPECT_EQUAL(board, MarbleBoard(boardFromStrings({
        "XX---XX",
        "XX---XX",
        "-------",
        "O------",
        "-------",
        "XX---XX",
        "XX---XX"
    })));
    EXPECT(board.isSolved());
    board.undoMove(move);
    EXPECT_EQUAL(board, original);
    EXPECT_EQUAL(board.numMarbles(), 2);
}

STUDENT_TEST("MarbleBoard::makeMove and undoMove") {
    MarbleBoard board(loadBoard("marbles/boards/pyramid.txt"));
    MarbleBoard original = board;
    Move move = { {3, 4}, {3, 6} };
    board.makeMove(move);
    Move move2 = { {4, 6}, {2, 6} };
    board.makeMove(move2);
    EXPECT_EQUAL(board, MarbleBoard(boardFromStrings({
        "XX---XX",
        "XX-O-XX",
        "--OOO-O",
        "-OOO---",
        "OOOOOO-",
        "XX---XX",
        "XX---XX"
    })));
    board.undoMove(move2);
    board.undoMove(move);
    EXPECT_EQUAL(board, original);
    EXPECT_EQUAL(board.numMarbles(), 16);
}

STUDENT_TEST("MarbleBoard::findPossibleMoves: one-jump.txt") {
    MarbleBoard board(loadBoard("marbles/boards/one-jump.txt"));
    EXPECT_EQUAL(board.findPossibleMoves().size(), 2);
}

STUDENT_TEST("MarbleBoard::findPossibleMoves: pyramid.txt") {
    MarbleBoard board(loadBoard("marbles/boards/pyramid.txt"));
    EXPECT_EQUAL(board.findPossibleMoves().size(), 12);
}
