/* ballots.cpp
 * Odelia Kislyuk & Cooper Ericksen
 * 2026-10-18
 */
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/* Function Prototype */
int countGoodOrderings(int a, int b, int lead);

/* countAllOrderings takes two integer arguments: a & b
 *
 * The function uses recursive calls to return the number 
 * of possible arrangements of A's and B's as if they were
 * ballots in a vote.
 */
int countAllOrderings(int a, int b) {
    if (a == 0 || b == 0) {
	return 1;
    }

    return countAllOrderings(a - 1, b) + countAllOrderings(a, b - 1);
}

/* countGoodOrderings takes two integer arguments: a & b
 *
 * The function uses recursive calls to return the number
 * of good arrangements of A's and B's as if they were
 * ballots in a vote.
 */
int countGoodOrderings(int a, int b) {
    int lead = a - b;
    return countGoodOrderings(a, b, lead);
}

int countGoodOrderings(int a, int b, int lead) {
    if (lead <= 0) {
	return 0;
    }

    // if only A's left or only B's left and lead is bigger than B's left
    if ((a == 0 && lead > b) || b == 0) {
	return 1;
    }

    return countGoodOrderings(a - 1, b, lead - 1) + countGoodOrderings(a, b - 1, lead + 1);
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("countAllOrderings, two A one B") {
   EXPECT_EQUAL(countAllOrderings(2, 1), 3);
}

PROVIDED_TEST("countGoodOrderings, two A one B") {
   EXPECT_EQUAL(countGoodOrderings(2, 1), 1);
}

STUDENT_TEST("countAllOrderings, three votes for one side; zero for other side") {
    EXPECT_EQUAL(countAllOrderings(3, 0), 1);
    EXPECT_EQUAL(countAllOrderings(0, 3), 1);
}

STUDENT_TEST("countAllOrderings, four A one B") {
   EXPECT_EQUAL(countAllOrderings(4, 1), 5);
}

// 5! / (2! * 3!)
STUDENT_TEST("countAllOrderings, two A three B") {
   EXPECT_EQUAL(countAllOrderings(2, 3), 10);
}

// 25! / (10! * 15!)
STUDENT_TEST("countAllOrderings, ten A fifteen B") {
   EXPECT_EQUAL(countAllOrderings(10, 15), 3268760);
}

STUDENT_TEST("countGoodOrderings, three votes for one side; zero for other side") {
    EXPECT_EQUAL(countGoodOrderings(3, 0), 1);
    EXPECT_EQUAL(countGoodOrderings(0, 3), 0);
}

STUDENT_TEST("countGoodOrderings, four A one B") {
   EXPECT_EQUAL(countGoodOrderings(4, 1), 3);
}

STUDENT_TEST("countGoodOrderings, two A three B") {
   EXPECT_EQUAL(countGoodOrderings(2, 3), 0);
}

STUDENT_TEST("countGoodOrderings, three A two B") {
   EXPECT_EQUAL(countGoodOrderings(3, 2), 2);
}

STUDENT_TEST("Bertrand's Formula Test") {
    for (int a = 1; a < 15; a++) {
	for (int b = 1; b < 15; b++) {
	    double berty = (a - b) / (a + b);
	    double funcs = (countGoodOrderings(a, b) / countAllOrderings(a, b));
	    EXPECT_EQUAL(berty, funcs);
	}
    }
}
