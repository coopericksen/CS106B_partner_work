/*
 * redistrict.cpp
 *
 * This file implements recursive backtracking to create
 * a gerrymandering algorithm. It will return a boolean
 * based on whether it was able to subdivide the district
 * into n equal sized subdistricts.
 *
 * Odelia Kisyluk & Cooper Ericksen
 * 2026-10-30
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "grid.h"
#include "gridgraphics.h"
#include "SimpleTest.h"
using namespace std;

int subgridSum(Grid<int>& g, GridLocationRange range) {
    int sum = 0;

    for (GridLocation loc: range) {
        sum += g[loc];
    }

    return sum;
}

void divideHorizontal(GridLocationRange range, int indexOfBottomMostRowInTopPart, GridLocationRange& topPart, GridLocationRange& bottomPart) {
    topPart = GridLocationRange(range.startRow(), range.startCol(), indexOfBottomMostRowInTopPart, range.endCol());
    bottomPart = GridLocationRange(indexOfBottomMostRowInTopPart + 1, range.startCol(), range.endRow(), range.endCol());
}

void divideVertical(GridLocationRange range, int indexOfRightMostColInLeftSide, GridLocationRange& leftSide, GridLocationRange& rightSide) {
    leftSide = GridLocationRange(range.startRow(), range.startCol(), range.endRow(), indexOfRightMostColInLeftSide);
    rightSide = GridLocationRange(range.startRow(), indexOfRightMostColInLeftSide + 1, range.endRow(), range.endCol());
}


/*
 * canRedistrict takes in a population grid and a number of districts to divide into. 
 * It returns true if the destrict can be divided into that many districts.
 *
 * It uses helper functions subgridSum, divideHorizontal, and divideVertical to
 * count the sum of the currentrange, create a horizontal cut, and create a vectical cut
 * respectively.
 *
 */
bool canRedistrict(Grid<int>& populationGrid, int target, GridLocationRange range) {
    int currentSum = subgridSum(populationGrid, range);

    // base case
    if (currentSum == target) {
        return true;
    }

    // failure case
    if (currentSum % target != 0) {
        return false;
    }

    // try horizontal cuts
    for (int i = range.startRow(); i < range.endRow(); i++) {
        GridLocationRange topHalf;
        GridLocationRange bottomHalf;
        divideHorizontal(range, i, topHalf, bottomHalf);

        drawHorizSplit(populationGrid, range, i - range.startRow() + 1);

        if (canRedistrict(populationGrid, target, topHalf) && canRedistrict(populationGrid, target, bottomHalf)) {
            return true;
        }
    }

     // try vertical cuts
    for (int i = range.startCol(); i < range.endCol(); i++) {
        GridLocationRange leftSide;
        GridLocationRange rightSide;
        divideVertical(range, i, leftSide, rightSide);

        drawVertSplit(populationGrid, range, i - range.startCol() + 1);

        if (canRedistrict(populationGrid, target, leftSide) && canRedistrict(populationGrid, target, rightSide)) {
            return true;
        }
    }

    // nothing worked
    return false;

}
bool canRedistrict(Grid<int>& populationGrid, int nDistricts) {
    GridLocationRange range = { {0, 0}, {populationGrid.numRows() - 1, populationGrid.numCols() - 1} };
    int target = subgridSum(populationGrid, range) / nDistricts;

    return canRedistrict(populationGrid, target, range);

}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("subgridSum sums values in grid range") {
    // This is an example test of helper function subgridSum
    Grid<int> grid = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange firstColumn = GridLocationRange(0, 0, 3, 0);
    GridLocationRange bottomCorner = GridLocationRange(3, 2, 3, 2);

    EXPECT_EQUAL(subgridSum(grid, firstColumn), 22);
    EXPECT_EQUAL(subgridSum(grid, bottomCorner), 10);
    EXPECT_EQUAL(subgridSum(grid, all), 75);
}

/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */


PROVIDED_TEST("subgridSum sums values in grid range") {
    // This is an example test of helper function subgridSum
    Grid<int> grid = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange firstColumn = GridLocationRange(0, 0, 3, 0);
    GridLocationRange bottomCorner = GridLocationRange(3, 2, 3, 2);

    EXPECT_EQUAL(subgridSum(grid, firstColumn), 22);
    EXPECT_EQUAL(subgridSum(grid, bottomCorner), 10);
    EXPECT_EQUAL(subgridSum(grid, all), 75);
}

PROVIDED_TEST("nevada example from writeup can be divided into 4 districts") {
    Grid<int> nevada = { {20, 31, 20},
                         { 5, 19, 30},
                         {25, 25, 25} };

    initForPopulationGrid(nevada); // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(nevada, 4));
    waitForCloseWindow();
}

PROVIDED_TEST("wyoming example from writeup cannot be divided into 2 districts") {
    Grid<int> wyoming = { {10, 15},
                          { 5, 10} };

    initForPopulationGrid(wyoming); // you can remove if you don't wish to animate this test case
    EXPECT(!canRedistrict(wyoming, 2));
    waitForCloseWindow();
}

PROVIDED_TEST("small example, 5 districts") {
    Grid<int> small = { {3, 7, 10},
                        {6, 3, 1},
                        {1, 10, 9} };
    initForPopulationGrid(small);  // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(small, 5));
    waitForCloseWindow();
}

PROVIDED_TEST("medium example, 12 districts") {
    Grid<int> medium = { {10, 1,  2, 5, 1, 4},
                          {1, 1,  8, 2, 1, 7},
                          {9, 8, 10, 3, 3, 4},
                          {5, 2,  2, 8, 7, 3},
                          {2, 1,  1, 7, 1, 1} };
    initForPopulationGrid(medium);  // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(medium, 12));
    waitForCloseWindow();
}

PROVIDED_TEST("large example can be divided into 25 districts") {
    Grid<int> large = { { 5,  2, 2,  1,  6, 4, 8, 2},
                        {10, 10, 2,  1,  3, 4, 2, 3},
                        { 1,  1, 4,  5,  1, 9, 1, 4},
                        { 9,  5, 5, 10, 10, 3, 3, 4},
                        { 3,  1, 1,  1,  2, 8, 6, 2},
                        { 2,  1, 1,  7,  3, 5, 1, 1},
                        { 1,  1, 2,  2,  7, 5, 1, 9},
                        { 4,  1, 2, 10,  1, 9, 5, 5} };
    initForPopulationGrid(large); // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(large, 25));
    waitForCloseWindow();
}
