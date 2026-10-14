/*
 * perfect.cpp
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-10-04
 * A program to find perfect numbers.
 *
 * Reflection:
 *
 * We learned that my computer tends to get real loud when crunching numbers
 * while looking for the 10th perfect number, which we were unable to determine
 * on our own. We also thought that Euclid's formula was very interesting. It 
 * was cool to see a simple method finish the task much faster.
 */

#include "console.h"
#include <iostream>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* 
 * The smarterSum function takes one argument 'n' and calculates the
 * sum of proper divisors of 'n' excluding itself. To find divisors
 * a loop iterates over all the numbers from 1 to sqrt(n), testing
 * for a zero remainder from the division using the modulus operator %
 *
 * The smarterSum function also calculates the paired divisor when a
 * valid divisor is found and adds it to the total.
 */
long smarterSum(long n) {
    long total = 0;
    if (n < 0) {
	return 0;
    }
    for (long divisor = 1; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor;
	    if (divisor != sqrt(n)) {
		total += n / divisor;
	    }
        }
    }
    return total - n;
}

/* The isPerfectSmarter function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 *
 * This is a smarter implementation using our smarterSum function
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* The findPerfectsSmarter function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 *
 * This is a smarter implementation using our smarterSum function
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* findNthPerfectEuclid takes one argument 'n' that represents
 * the Nth perfect number we expect it to return. It will use Euclid's
 * method for finding perfect numbers until it has found n perfect numbers.
 * At that point it will then return the latest number it has found.
 *
 * Euclid's method is much faster than the brute force approaches we tried earlier!
 */
long findNthPerfectEuclid(long n) {
    long perfectsFound = 0;

    long k = 1;

    while (true) {
	long m = pow(2, k) - 1;

	// IF PRIME
	if (smarterSum(m) == 1) {
	    perfectsFound++;

	    // IF NUMBER WE ARE LOOKING FOR
	    if (perfectsFound == n) {
		return m * pow(2, k-1);
	    }
	}

	k++;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}

STUDENT_TEST("Time trial of findPerfects on input size 25000-200000") {
    int smallest = 25000, largest = 200000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}

STUDENT_TEST("Time trial of findPerfectsSmarter on input size 25000-200000") {
    int smallest = 25000, largest = 200000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfectsSmarter(size));
    }
}

STUDENT_TEST("For negative numbers") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-50));
    EXPECT(!isPerfect(-120));
    EXPECT(!isPerfect(-2500));
}

STUDENT_TEST("smarterSum equal to divisorSum") {
    EXPECT_EQUAL(smarterSum(2), divisorSum(2));
    EXPECT_EQUAL(smarterSum(6), divisorSum(6));
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
    EXPECT_EQUAL(smarterSum(-2), divisorSum(-2));
    EXPECT_EQUAL(smarterSum(-25), divisorSum(-25));
    EXPECT_EQUAL(smarterSum(1055), divisorSum(1055));
    EXPECT_EQUAL(smarterSum(99999), divisorSum(99999));
}

STUDENT_TEST("Testing findNthPerfectEuclid") {
    EXPECT_EQUAL(6, findNthPerfectEuclid(1));
    EXPECT_EQUAL(28, findNthPerfectEuclid(2));
    EXPECT_EQUAL(496, findNthPerfectEuclid(3));
    EXPECT_EQUAL(8128, findNthPerfectEuclid(4));
    EXPECT_EQUAL(33550336, findNthPerfectEuclid(5));
}
