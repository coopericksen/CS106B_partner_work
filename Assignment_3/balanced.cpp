/* balanced.cpp
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-10-19
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "SimpleTest.h"

using namespace std;

/*
 * operatorsFrom takes a single string argument.
 * It returns a string containing only operators
 * from the symbols variable: ()[]{}.
 */
string operatorsFrom(string str) {
    string symbols = "()[]{}";
    string result;

    for (int i = 0; i < str.length(); i++) {
	if (stringContains(symbols, str[i])) {
	    result += str[i];
	}
    }

    return result;
}

/*
 * operatorsAreMatched takes on string argument
 * and checks to see if the string contains valid
 * operator matches for parentheses, brackets, and braces
 */
bool operatorsAreMatched(string ops) {
    if (ops == "") {
	return true;
    }

    if (stringContains(ops, "()")) {
	int index = stringIndexOf(ops, "()");
	return operatorsAreMatched(ops.substr(0, index) + ops.substr(index + 2));
    }
    if (stringContains(ops, "[]")) {
	int index = stringIndexOf(ops, "[]");
	return operatorsAreMatched(ops.substr(0, index) + ops.substr(index + 2));
    }
    if (stringContains(ops, "{}")) {
	int index = stringIndexOf(ops, "{}");
	return operatorsAreMatched(ops.substr(0, index) + ops.substr(index + 2));
    }

    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsFrom with line of code") {
    EXPECT_EQUAL(operatorsFrom("for (int i = 0; i < str.length(); i++) {}"), "(()){}");
}

STUDENT_TEST("operatorsFrom with no symbols") {
    EXPECT_EQUAL(operatorsFrom("int number = 4;"), "");
    EXPECT_EQUAL(operatorsFrom(""), "");
}

STUDENT_TEST("operatorsFrom with symbol jumble") {
    EXPECT_EQUAL(operatorsFrom("{{{}{()(}[][]}"), "{{{}{()(}[][]}");
    EXPECT_EQUAL(operatorsFrom("{{{c++}{()(}[hehe][]}"), "{{{}{()(}[][]}");
}

STUDENT_TEST("operatorsAreMatched with matching strings") {
    EXPECT(operatorsAreMatched("{}()[]"));
    EXPECT(operatorsAreMatched("{()[]}"));
    EXPECT(operatorsAreMatched("{([])}"));
    EXPECT(operatorsAreMatched(""));
}

STUDENT_TEST("operatorsAreMatched with misplaced symbols") {
    EXPECT(!operatorsAreMatched("}){([]"));
    EXPECT(!operatorsAreMatched("]{[()}"));
    EXPECT(!operatorsAreMatched("{([]})"));
}

STUDENT_TEST("operatorsAreMatched with missing symbols") {
    EXPECT(!operatorsAreMatched("{(["));
    EXPECT(!operatorsAreMatched("{}(()[]"));
    EXPECT(!operatorsAreMatched("[()]}"));
}
