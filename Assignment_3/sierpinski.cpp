/* sierpinski.cpp
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-10-19
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/* calculateMidpoint takes two GPoints in as arguments
 * and returns a GPoint that is the midpoint between
 * them.
 */
GPoint calculateMidpoint(GPoint one, GPoint two) {
    GPoint result;

    result.x = (one.x + two.x)/2;
    result.y = (one.y + two.y)/2;

    return result;
}

/*
 * drawSierpinskiTriangle recursively draws triangles to form
 * a sierpinski triangle fractal. It returns the amount of triangles
 * drawn based on the passed parameter, order.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order == 0) {
	fillBlackTriangle(window, one, two, three);
	return 1;
    }

    int triangle1 = drawSierpinskiTriangle(window, one, calculateMidpoint(one, two), calculateMidpoint(one, three), order - 1);

    int triangle2 = drawSierpinskiTriangle(window, calculateMidpoint(one, two), two, calculateMidpoint(two, three), order - 1);

    int triangle3 = drawSierpinskiTriangle(window, calculateMidpoint(one, three), calculateMidpoint(two, three), three, order - 1);

    return triangle1 + triangle2 + triangle3;
}

/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all your testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

