/* search.cpp
 * Odelia Kislyuk & Cooper Ericksen
 * 2026-10-12
 */

#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* cleanToken takes one parameter 's' of type string
 *
 * The function sanitizes the input string by removing
 * symbols and making all letters lowercase leaving only
 * lowercase letters and numbers.
 */
string cleanToken(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i]) || isdigit(s[i])) {
            result += tolower(s[i]);
        }
    }
    return result;
}

/* gatherTokens takes a string of text and splits it into 
 * words using " " as the delimiter.
 *
 * It cleans each word using the cleanToken function and 
 * adds cleanedToken to the Set, tokens.
 */
Set<string> gatherTokens(string text) {
    Set<string> tokens;

    Vector<string> uncleanTokens = stringSplit(text, " ");
    
    for (int i = 0; i < uncleanTokens.size(); i++) {
	string cleanedToken = cleanToken(uncleanTokens[i]); 
	if (cleanedToken != "") {
	    tokens += cleanedToken;
	}
    }

    return tokens;
}

/* buildIndex takes a path to a dbfile and a reference to Map to fill
 * with our index.
 *
 * The function first creates a forwardIndex associating URLs to words.
 *
 * Next, it reverses this forwardIndex to create the intended index associating
 * words to URLs.
 *
 * It returns the number of URLs processed.
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index) {

    Map<string, Set<string>> forwardIndex;

    // reading dbfile
    ifstream in;
    Vector<string> fileText;

    if (openFile(in, dbfile)) {
	fileText = readLines(in);
    }

    // creating forwardIndex
    string currentURL;
    for (int i = 0; i < fileText.size(); i++) {
	if (i % 2 == 0) {
	    currentURL = fileText[i];
	} else {
	    forwardIndex[currentURL] = gatherTokens(fileText[i]);
	}
    }

    // reversing forwardIndex
    for (string key : forwardIndex) {
	int setSize = forwardIndex[key].size();
	for (int i = 0; i < setSize; i++) {
	    string token = forwardIndex[key].last();

	    index[token] += key;

	    forwardIndex[key].remove(token);
	}
    }

    return forwardIndex.size();
}

/* findQueryMatches gathers a list of URLs where the query string terms appear
 * and returns a set containing all of those URLs.
 *
 * Supports Set intersect and Set difference using + and - operators in front 
 * of search terms. Otherwise it defaults to Set unionWith
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;

    Vector<string> splitQuery = stringSplit(query, " ");

    for (int i = 0; i < splitQuery.size(); i++) {
	string searchTerm = splitQuery[i];

	if (stringContains(searchTerm, "+")) {
	    searchTerm = cleanToken(searchTerm);
	    result.intersect(index[searchTerm]);
	} else if (stringContains(searchTerm, "-")) {
	    searchTerm = cleanToken(searchTerm);
	    result.difference(index[searchTerm]);
	} else {
	    searchTerm = cleanToken(searchTerm);
	    result.unionWith(index[searchTerm]);
	}

    }

    return result;
}

/* searchEngine takes one argument, the path to the dbfile
 *
 * It creates a loop for the user to search through the reverse
 * index created for the provided dbfile using a query capable
 * of Set operations like intersect, difference, and unionWith.
 *
 * It will print pages that match with the query provided by the user.
 */ 
void searchEngine(string dbfile) {
    Map<string, Set<string>> index;

    cout << "\nStand by while building index..." << endl;

    int pagesProcessed = buildIndex(dbfile, index);

    Set<string> uniqueTerms;

    for (string key: index) {
	uniqueTerms += key;
    }

    int uniqueTermCount = uniqueTerms.size(); 

    cout << "Indexed " << pagesProcessed << " pages containing " << uniqueTermCount << " unique terms." << endl;

    string input;

    while (true) {
	input = getLine("\nEnter query sentence (RETURN/ENTER to quit): ");
	
	if (input == "RETURN" || input == "") {
	    return;
	}

	Set<string> matches = findQueryMatches(index, input);

	int matchesFound = matches.size();

	cout << "Found " << matchesFound << " matching pages" << endl;
	cout << matches << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings of letters and digits") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS106B"), "cs106b");
}

PROVIDED_TEST("cleanToken on strings containing punctuation") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("they're"), "theyre");
}

PROVIDED_TEST("cleanToken on string of only punctuation") {
    EXPECT_EQUAL(cleanToken("#$^@@.;"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 12 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 12);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

STUDENT_TEST("cleanToken on empty string") {
    EXPECT_EQUAL(cleanToken(""), "");
}

STUDENT_TEST("cleanToken on numbers") {
    EXPECT_EQUAL(cleanToken("12345"), "12345");
}

STUDENT_TEST("cleanToken on random") {
    EXPECT_EQUAL(cleanToken("TEAsao175thas#!@$eaesouh"), "teasao175thaseaesouh");
}

STUDENT_TEST("gatherTokens with lots of symbols") {
    Set<string> tokens = gatherTokens("$$ HELLO #// World! CS106 B !$");
    EXPECT_EQUAL(tokens.size(), 4);
    EXPECT(tokens.contains("hello"));
    EXPECT(!tokens.contains("HELLO"));
}

STUDENT_TEST("gatherTokens with double spaces between words") {
    Set<string> tokens = gatherTokens("Hello  World  This  Has  Double  Spaces  !");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("has"));
    EXPECT(!tokens.contains("!"));
}

STUDENT_TEST("gatherTokens with longer sentence") {
    Set<string> tokens = gatherTokens("Poecilia vandepolli is a poeciliid fish native to Aruba, Bonaire, and Curacao. It is one of the most common fish in its range, inhabiting fresh, brackish, salt, and hypersaline waters.");
    EXPECT_EQUAL(tokens.size(), 27); // 27 Unique words in sentence
    EXPECT(tokens.contains("curacao"));
}

STUDENT_TEST("buildIndex from website.txt, 36 pages") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 36);
    EXPECT(index.containsKey("gradescope"));
}

STUDENT_TEST("buildIndex benchmark") {
    Map<string, Set<string>> index;
    TIME_OPERATION(4, buildIndex("res/tiny.txt", index));
    TIME_OPERATION(36, buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries case-insensitive") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);

    Set<string> result = findQueryMatches(index, "RED -fish");
    EXPECT_EQUAL(result.size(), 1);
}

STUDENT_TEST("findQueryMatches from tiny.txt, longer compound query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);

    Set<string> result = findQueryMatches(index, "red +fish blue -10");
    EXPECT_EQUAL(result.size(), 1);
}

STUDENT_TEST("findQueryMatches from website.txt, searching for maze page") {
    Map<string, Set<string>> index;
    buildIndex("res/website.txt", index);

    Set<string> result = findQueryMatches(index, "solvemazedfs framework -illuminating minotaur gridlocation +labyrinths");
    EXPECT_EQUAL(result.first(), "http://cs106b.stanford.edu/assignments/2-adt/maze.html");
}
