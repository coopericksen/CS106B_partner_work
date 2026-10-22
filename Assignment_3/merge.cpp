/* merge.cpp
 * Odelia Kislyuk & Cooper Ericksen
 * 2026-10-20
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "SimpleTest.h"
using namespace std;

/*
 * binaryMerge takes two Queues of int, a and b
 * It combines these sorted Queues and returns them
 * as a new sorted Queue.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;

    int size = a.size() + b.size();

    int maxNum = INT_MIN; // for sorted validation
    for (int i = 0; i < size; i++) {
	int num;

	if (a.size() == 0) {
	    num = b.dequeue();
	} else if (b.size() == 0 || a.peek() <= b.peek()) {
	    num = a.dequeue();
	} else {
	    num = b.dequeue();
	}

	if (num < maxNum) {
	    error("Queue is unsorted.");
	} else {
	    maxNum = num;
	}
	
	result.enqueue(num);
    }

    return result;
}

/*
 * The function naiveMultiMerge assumes you have a correctly working
 * binaryMerge function, which this function calls to iteratively
 * merge a collection of sequences, starting off with an empty sequence
 * and repeatedly merging in an additional sequence until all are merged.
 * The code below is provided to you is implemented correctly and you
 * should not need to modify it.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * recMultiMerge takes one argument, a Vector of Queue<int>, all.
 * It uses a recursive approach to merge multiple sorted Queues 
 * into a single sorted Queue. 
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    if (all.size() == 0) {
	Queue<int> empty;
	return empty;
    }

    if (all.size() == 1) {
	return all[0];
    }

    if (all.size() == 2) {
	return binaryMerge(all[0], all[1]);
    }

    int half = all.size() / 2;

    Vector<Queue<int>> half1 = all.subList(0, half);
    Vector<Queue<int>> half2 = all.subList(half);

    return binaryMerge(recMultiMerge(half1), recMultiMerge(half2));
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

/* Student Tests */
STUDENT_TEST("binaryMerge with b longer than a") {
    Queue<int> a = {1, 3, 5, 7};
    Queue<int> b = {2, 4, 6, 8, 10};
    Queue<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 10};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
}

STUDENT_TEST("binaryMerge with a longer than b") {
    Queue<int> a = {1, 3, 5, 7, 9};
    Queue<int> b = {2, 4, 6};
    Queue<int> expected = {1, 2, 3, 4, 5, 6, 7, 9};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
}

STUDENT_TEST("binaryMerge with b empty") {
    Queue<int> a = {1, 1, 5, 5, 9};
    Queue<int> b = {};
    EXPECT_EQUAL(binaryMerge(a, b), a);
}

STUDENT_TEST("binaryMerge with a empty") {
    Queue<int> a = {};
    Queue<int> b = {3, 3, 5, 7, 7};
    EXPECT_EQUAL(binaryMerge(a, b), b);
}

STUDENT_TEST("binaryMerge with duplicates among a and b") {
    Queue<int> a = {1, 1, 1, 2, 9};
    Queue<int> b = {1, 1, 1, 4};
    Queue<int> expected = {1, 1, 1, 1, 1, 1, 2, 4, 9};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
}

STUDENT_TEST("binaryMerge with unsorted list") {
    Queue<int> a = {1, 1, 3, 2, 9};
    Queue<int> b = {1, 1, 1, 4};
    EXPECT_ERROR(binaryMerge(a, b));
}

STUDENT_TEST("binaryMerge with unsorted list #2") {
    Queue<int> a = {1, 2, 3, 4, 5};
    Queue<int> b = {3, 4, 5, 8, 3};
    EXPECT_ERROR(binaryMerge(a, b));
}

STUDENT_TEST("binaryMerge timing") {
    int n = 25000000;
    for (int i = 0; i < 4; i++) {
	Queue<int> a = createSequence(n);
	Queue<int> b = createSequence(n);
	TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));

	n *= 2;
    }
}

STUDENT_TEST("naiveMultiMerge with 0 queues") {
    Vector<Queue<int>> empty = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(empty), expected);
}

STUDENT_TEST("naiveMultiMerge with multiple empty queues") {
    Vector<Queue<int>> vec;
    for (int i = 0; i < 10; i++) {
	Queue<int> empty = {};
	vec += empty;
    }

    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(vec), expected);
}

STUDENT_TEST("naiveMultiMerge with normal queues") {
    Queue<int> a = {1, 2, 3};
    Queue<int> b = {2, 7, 8};
    Queue<int> c = {5, 6, 9};

    Vector<Queue<int>> vec = {a, b, c};

    Queue<int> expected = {1, 2, 2, 3, 5, 6, 7, 8, 9};
    EXPECT_EQUAL(naiveMultiMerge(vec), expected);
}

STUDENT_TEST("naiveMultiMerge timing (changing n)") {
    int n = 50000;
    int k = n/10;
    for (int i = 0; i < 5; i++) {
	Queue<int> input = createSequence(n);
	Vector<Queue<int>> all(k);
	distribute(input, all);
	TIME_OPERATION(input.size(), naiveMultiMerge(all));

	n *= 2;
    }
}

STUDENT_TEST("naiveMultiMerge timing (changing k)") {
    int n = 50000;
    int k = 1000;
    for (int i = 0; i < 5; i++) {
	Queue<int> input = createSequence(n);
	Vector<Queue<int>> all(k);
	distribute(input, all);
	TIME_OPERATION(k, naiveMultiMerge(all));

	k *= 2;
    }
}

STUDENT_TEST("Checking recMultiMerge with naiveMultiMerge") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);

    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("recMultiMerge timing (changing n)") {
    int n = 400000;
    int k = n/10;
    for (int i = 0; i < 5; i++) {
	Queue<int> input = createSequence(n);
	Vector<Queue<int>> all(k);
	distribute(input, all);
	TIME_OPERATION(input.size(), recMultiMerge(all));

	n *= 2;
    }
}

STUDENT_TEST("recMultiMerge timing (changing k)") {
    int n = 50000;
    int k = 100;
    for (int i = 0; i < 5; i++) {
	Queue<int> input = createSequence(n);
	Vector<Queue<int>> all(k);
	distribute(input, all);
	TIME_OPERATION(k, recMultiMerge(all));

	k *= 10;
    }
}
