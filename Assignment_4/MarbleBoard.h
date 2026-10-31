/*
 * File: MarbleBoard.h
 * -------------------
 * CS106B Marble Solitaire
 * A MarbleBoard stores a Marble Solitaire board and knows the rules of the
 * game. You will write findPossibleMoves, makeMove, and undoMove in
 * MarbleBoard.cpp.
 *
 * You may add private helper functions, but do not change the public ones.
 */
#pragma once
#include <iostream>
#include <string>
#include "grid.h"
#include "gridlocation.h"
#include "vector.h"
#include "marbletypes.h"   // for Marble, Move, BoardEncoding

class MarbleBoard {
public:
    /*
     * Creates the starting board: a marble in every hole except the center.
     */
    MarbleBoard();

    /*
     * Creates a board from a grid, for example:
     *     MarbleBoard board(loadBoard("marbles/boards/cross.txt"));
     */
    MarbleBoard(const Grid<Marble>& grid);

    /*
     * Returns all legal moves, in any order. A legal move jumps a marble
     * over the marble next to it (up, down, left, or right) into an empty
     * hole on the other side.
     */
    Vector<Move> findPossibleMoves() const;

    /*
     * Makes the move: the marble at start jumps to end, and the marble it
     * jumps over is removed. You may assume the move is legal.
     * Remember to update _numMarbles.
     */
    void makeMove(Move move);

    /*
     * Undoes the move, which you can assume was the last move made and was
     * valid: the marble at end goes back to start, and the jumped-over
     * marble is put back. Remember to update _numMarbles.
     */
    void undoMove(Move move);

    /* The functions below are already written for you. */

    /* Returns the number of marbles on the board. */
    int numMarbles() const;

    /* Returns true if exactly one marble is left. */
    bool isSolved() const;

    /* Returns what is at the given square: empty, occupied, or invalid. */
    Marble at(GridLocation loc) const;

    /*
     * Returns a small value that stands for this board. Boards with marbles
     * in the same places have equal encodings. Use these to remember boards
     * you have already explored.
     */
    BoardEncoding encode() const;

    /* Returns a copy of the board as a grid. */
    Grid<Marble> toGrid() const;

    /* Returns a picture of the board, handy for debugging: cout << board; */
    std::string toString() const;

    /* Let boards be compared with == and != and printed with <<. */
    friend bool operator ==(const MarbleBoard& lhs, const MarbleBoard& rhs) { return lhs._grid == rhs._grid; }
    friend bool operator !=(const MarbleBoard& lhs, const MarbleBoard& rhs) { return !(lhs == rhs); }
    friend std::ostream& operator <<(std::ostream& out, const MarbleBoard& board) { return out << board.toString(); }

private:
    Grid<Marble> _grid;   // the 7x7 grid; the four 2x2 corners are MARBLE_INVALID
    int _numMarbles;      // how many squares are MARBLE_OCCUPIED
};
