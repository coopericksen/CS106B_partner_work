/* Labyrinth.cpp
 * 2026-11-12
 * Cooper Ericksen & Odelia Kislyuk
 */

#include "Labyrinth.h"
#include "iostream"
using namespace std;

/* checkItem is a helper function that takes the current cell and checks
 * which item (if any) it holds. If it does have an item it uses the
 * reference to our booleans to mark the item as collected.
 */
void checkItem(MazeCell* current, bool& collectedSpellbook, bool& collectedPotion, bool& collectedWand) {
    if (current->whatsHere == Item::SPELLBOOK) {
        collectedSpellbook = true;
    }
    if (current->whatsHere == Item::POTION) {
        collectedPotion = true;
    }
    if (current->whatsHere == Item::WAND) {
        collectedWand = true;
    }
}

/* isPathToFreedom takes a starting cell and collection of moves and 
 * checks to see whether the path is valid to travel and collects
 * all three magic items, returning a boolean based on the path's success.
 */
bool isPathToFreedom(MazeCell* start, const string& moves) {
    bool collectedSpellbook = false;
    bool collectedPotion = false;
    bool collectedWand = false;

    MazeCell* current = start;

    for (char move: moves) {

        checkItem(current, collectedSpellbook, collectedPotion, collectedWand);

        MazeCell* nextMove;

        if (move == 'N') {
            nextMove = current->north;
        }
        if (move == 'E') {
            nextMove = current->east;
        }
        if (move == 'S') {
            nextMove = current->south;
        }
        if (move == 'W') {
            nextMove = current->west;
        }

        if (nextMove == nullptr) return false; // Requested move is invalid

        current = nextMove;
    }

    checkItem(current, collectedSpellbook, collectedPotion, collectedWand); // Check ending cell for item

    return collectedSpellbook && collectedPotion && collectedWand; // Success if found all three
}


/* * * * * * Test Cases Below This Point * * * * * */
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"

/* * * * * Provided Tests Below This Point * * * * */

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we write 'delete maze;'
     * rather than what's shown above?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}

/* Printer for items. */
ostream& operator<< (ostream& out, Item item) {
    if (item == Item::NOTHING) {
        return out << "Item::NOTHING";
    } else if (item == Item::WAND) {
        return out << "Item::WAND";
    } else if (item == Item::POTION) {
        return out << "Item::POTION";
    } else if (item == Item::SPELLBOOK) {
        return out << "Item::SPELLBOOK";
    } else {
        return out << "<unknown item type>";
    }
}
