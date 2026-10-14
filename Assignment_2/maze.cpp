/* maze.cpp
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-10-10
 */

#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* generateValidMoves takes the Grid and current location and checks
 * for valid moves around the current location (N, E, S, W)
 *
 * It returns a set of grid locations that are valid moves
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    // Furthest possible locations to check
    // If location goes outside of these bounds,
    // then its not a valid move.
    int topBound = 0;
    int leftBound = 0;
    int bottomBound = maze.numRows()-1;
    int rightBound = maze.numCols()-1;

    GridLocation location;

    // above
    location.row = cur.row - 1;
    location.col = cur.col;
    if (location.row >= topBound && maze[location]) {
	neighbors += location;
    }
    // below
    location.row = cur.row + 1;
    location.col = cur.col;
    if (location.row <= bottomBound && maze[location]) {
	neighbors += location;
    }
    // left
    location.row = cur.row;
    location.col = cur.col - 1;
    if (location.col >= leftBound && maze[location]) {
	neighbors += location;
    }
    // right
    location.row = cur.row;
    location.col = cur.col + 1;
    if (location.col <= rightBound && maze[location]) {
	neighbors += location;
    }

    return neighbors;
}

/* validatePath accepts the maze and path as parameters.
 * The function checks that the proposed solution to the maze
 * does not violate one of the following:
 *
 * - The path is not empty
 * - The path starts at the entry
 * - The path ends at the exit
 * - Each location is a valid move from the previous
 * - Path must not contain a loop
 */
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty()) {
        error("Path is empty!");
    }

    // Starts at entry (top-left)
    GridLocation start = { 0, 0 };
    if (path[0] != start) {
	error("Path does not start at entry point!");
    }

    // Starts at exit (bottom-right)
    GridLocation exit = { maze.numRows()-1, maze.numCols()-1 };
    if (path[path.size()-1] != exit) {
	error("Path does not end at exit point!");
    }

    // Checking moves are valid
    for (int i = 1; i < path.size()-1; i++) {
	if (!(generateValidMoves(maze, path[i]).contains(path[i-1]))) {
	    error("Path contains invalid move!");
	}
    }

    Set<GridLocation> visited;
    for (int i = 0; i < path.size()-1; i++) {
	if (visited.contains(path[i])) {
	    error("Path has already been to this location");
	}

	visited += path[i];
    }
}

/* solveMazeBFS takes in a maze and returns a path that solves the maze
 * It uses breadth first search with a queue to find the solution.
 */
Vector<GridLocation> solveMazeBFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Queue<Vector<GridLocation>> allPaths;

    GridLocation startLocation = { 0, 0 };
    GridLocation exitLocation = { maze.numRows()-1, maze.numCols()-1 };

    drawMaze(maze);

    path += startLocation;
    allPaths.enqueue(path);

    cout << "Beginning search..." << endl;

    Set<GridLocation> visited;

    while (path[path.size()-1] != exitLocation) {

        path = allPaths.dequeue();

        highlightPath(path, "#ff0000");

        Set<GridLocation> movesAvailable = generateValidMoves(maze, path[path.size()-1]);

        while (movesAvailable.size() > 0) {
	    Vector<GridLocation> newPath = path;

	    GridLocation newMove = movesAvailable.last();
	    movesAvailable.remove(newMove);

	    if (!(visited.contains(newMove))) {
		visited.add(newMove);
		newPath += newMove;

		allPaths.enqueue(newPath);
	    }
        }
    }

    return path;
}

/* solveMazeDFS takes in a maze and returns a path that solves the maze
 * It uses depth first search with a stack to find the solution.
 */
Vector<GridLocation> solveMazeDFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Stack<Vector<GridLocation>> allPaths;

    GridLocation startLocation = { 0, 0 };
    GridLocation exitLocation = { maze.numRows()-1, maze.numCols()-1 };

    drawMaze(maze);

    path += startLocation;
    allPaths.push(path);

    Set<GridLocation> visited;

    while (path[path.size()-1] != exitLocation) {

        path = allPaths.pop();

        highlightPath(path, "#ff0000");

        Set<GridLocation> movesAvailable = generateValidMoves(maze, path[path.size()-1]);

        while (movesAvailable.size() > 0) {
	    Vector<GridLocation> newPath = path;

	    GridLocation newMove = movesAvailable.last();
	    movesAvailable.remove(newMove);

	    if (!(visited.contains(newMove))) {
		visited.add(newMove);
		newPath += newMove;

		allPaths.push(newPath);
	    }
        }
    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines = stringSplit(readEntire(in), '\n');

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}
PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0, 0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> notBeginAtEntry = { {1, 1}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}

PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("generateValidMoves, random mix") {
    Grid<bool> maze = {{true, true, false},
                       {false, true, true},
                       {true, false, false}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves, with no valid moves") {
    Grid<bool> maze = {{false, false, false},
                       {false, true, false},
                       {false, false, false}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves, larger maze") {
    Grid<bool> maze = {{true, true, false, true},
                       {false, false, true, true},
                       {false, true, true, true},
                       {true, false, false, true}};
    GridLocation center = {2, 2};
    Set<GridLocation> expected = {{2, 1}, {1, 2}, {2, 3}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("validatePath on looping solution") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, true},
                       {true, true, true}};
    Vector<GridLocation> soln = { {0, 0}, {1, 0}, {2, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2} };
    Vector<GridLocation> soln2 = { {0, 0}, {1, 0}, {1, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 2} };

    EXPECT_ERROR(validatePath(maze, soln));
    EXPECT_ERROR(validatePath(maze, soln2));
}

STUDENT_TEST("validatePath on valid solution") {
    Grid<bool> maze = {{true, false, false},
                       {true, true, true},
                       {false, false, true}};
    Vector<GridLocation> soln = { {0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on empty path") {
    Grid<bool> maze = {{true, false, false},
                       {true, true, true},
                       {false, false, true}};
    Vector<GridLocation> soln = {};

    EXPECT_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
