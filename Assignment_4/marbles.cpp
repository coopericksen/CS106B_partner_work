/*
 * File: marbles.cpp
 * -----------------
 *
 *  marbles.cpp uses a recursive algorithm
 *  to solve the marble solitaire game for 
 *  any board provided to it. It uses helper methods
 *  defined in the MarbleBoard.cpp file to simulate 
 *  boards as it searches.
 *
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-10-30
 */
#include <iostream>
#include "backtracking.h"
#include "hashset.h"
#include "set.h"
#include "SimpleTest.h"
using namespace std;

bool solvePuzzle(MarbleBoard& board, Vector<Move>& moveHistory, Set<BoardEncoding>& boardsSeen);

/*
 * solvePuzzle takes a board and move history and returns
 * a boolean based on whether it was able to find a valid
 * solution to the board. It recursively checks all options
 * and using backtracking to search through valid moves.
 * It also implements memoization to avoid redundant passes.
 */
bool solvePuzzle(MarbleBoard& board, Vector<Move>& moveHistory) {
    Set<BoardEncoding> boardsSeen;
    return solvePuzzle(board, moveHistory, boardsSeen);
}

bool solvePuzzle(MarbleBoard& board, Vector<Move>& moveHistory, Set<BoardEncoding>& boardsSeen) {
    // base case
    if (board.isSolved()) {
        return true;
    }

    // skip already seen boards
    BoardEncoding currentBoard = board.encode();
    if (boardsSeen.contains(currentBoard)) {
        return false;
    }
    boardsSeen += currentBoard;

    Vector<Move> possibleMoves = board.findPossibleMoves();

    for (Move& move: possibleMoves) {
        // choose
        board.makeMove(move);
        moveHistory.add(move);

        // explore
        if (solvePuzzle(board, moveHistory, boardsSeen)) {
            return true;
        }

        // unchoose
        board.undoMove(move);
        moveHistory.remove(moveHistory.size()-1);
    }

    return false;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("solvePuzzle: one marble in the center is already solved") {
    MarbleBoard board(loadBoard("marbles/boards/solved.txt"));
    Vector<Move> moves;
    EXPECT(solvePuzzle(board, moves));
    EXPECT_EQUAL(moves.size(), 0);
}

PROVIDED_TEST("solvePuzzle: two marbles are solved with one jump") {
    Grid<Marble> original = loadBoard("marbles/boards/one-jump.txt");
    MarbleBoard board(original);
    Vector<Move> moves;
    EXPECT(solvePuzzle(board, moves));
    EXPECT_EQUAL(moves.size(), 1);
    EXPECT_EQUAL(checkSolution(original, moves), "");
}


PROVIDED_TEST("solvePuzzle: an unsolvable board returns false and is left unchanged") {
    MarbleBoard board(loadBoard("marbles/boards/five-plus.txt"));
    MarbleBoard original = board;
    Vector<Move> moves;
    EXPECT(!solvePuzzle(board, moves));
    EXPECT_EQUAL(board, original);
    EXPECT_EQUAL(board.numMarbles(), original.numMarbles());
}

PROVIDED_TEST("solvePuzzle: solvable mid-size boards") {
    for (string name : { "cross", "seven-step", "plus", "double-cross"}) {
        Grid<Marble> original = loadBoard("marbles/boards/" + name + ".txt");
        MarbleBoard board(original);
        Vector<Move> moves;
        EXPECT(solvePuzzle(board, moves));
        EXPECT_EQUAL(checkSolution(original, moves), "");
    }
}

PROVIDED_TEST("solvePuzzle: mid-size unsolvable board") {
    MarbleBoard board(loadBoard("marbles/boards/tangle.txt"));
    Vector<Move> moves;
    EXPECT(!solvePuzzle(board, moves));
}

PROVIDED_TEST("solvePuzzle: the full 32-marble board") {
    MarbleBoard board;
    Vector<Move> moves;
    bool solved = false;
    TIME_OPERATION(32, solved = solvePuzzle(board, moves));
    EXPECT(solved);
    EXPECT_EQUAL(checkSolution(fullBoard(), moves), "");
}

PROVIDED_TEST("solvePuzzle: unsolvable 15-marble board (needs the explored-boards optimization)") {
    // A solver that does not skip boards it has already explored will take
    // a very, very long time on this test.
    MarbleBoard board(loadBoard("marbles/boards/letter-l.txt"));
    Vector<Move> moves;
    bool solved = true;
    TIME_OPERATION(15, solved = solvePuzzle(board, moves));
    EXPECT(!solved);
}
