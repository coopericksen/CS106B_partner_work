/*
 * soundex.cpp
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-10-04
 * The program converts names into soundex codes of length 4.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* 
 * The string was initialized with the first character of the string.
 * If that character was not a letter, then a letter or special character
 * could contaminate our result string.
 *
 * We declared result and then sanitized all of the letters in the string
 * by starting the loop at 0 instead of 1.
 */

/* lettersOnly takes one argument 's' which is a string that we want to sanitize.
 * It loops over and removes non-letters and returns the result.
 */
string lettersOnly(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/*
 * Helper Functions
 */

/*
 * toDigits function takes one argument 's' of type string
 * It will convert the string into a numeric code based
 * on the table using the constants defined in the function.
*/
string toDigits(string s) {
    // CONSTANTS
    string digit0 = "AEIOUHWY";
    string digit1 = "BFPV";
    string digit2 = "CGJKQSXZ";
    string digit3 = "DT";
    string digit4 = "L";
    string digit5 = "MN";
    string digit6 = "R";

    string code;

    for (int i = 0; i < s.length(); i++) {
	if (stringContains(digit0, toupper(s[i]))) {
	    code += "0";
	}
	if (stringContains(digit1, toupper(s[i]))) {
	    code += "1";
	}
	if (stringContains(digit2, toupper(s[i]))) {
	    code += "2";
	}
	if (stringContains(digit3, toupper(s[i]))) {
	    code += "3";
	}
	if (stringContains(digit4, toupper(s[i]))) {
	    code += "4";
	}
	if (stringContains(digit5, toupper(s[i]))) {
	    code += "5";
	}
	if (stringContains(digit6, toupper(s[i]))) {
	    code += "6";
	}
    }

    return code;
}

/*
 * removeDuplicates function takes one argument 's' of type string
 * Joins duplicate adjacent numbers from the code together
*/
string removeDuplicates(string s) {
    string combinedCode;
    
    for (int i = 0; i < s.length(); i++) {
	if (i == s.length()-1) {
	    combinedCode += s[i];
	    break;
	}
	if (s[i] != s[i+1]) {
	    combinedCode += s[i];
	}
    }

    return combinedCode;
}

/*
 * removeZeros function takes one argument 's' of type string
 * Removes zeros from the combined together code
*/
string removeZeros(string s) {
    string zeroFree;

    for (int i = 0; i < s.length(); i++) {
	if (s[i] != '0') {
	    zeroFree += s[i];
	}
    }

    return zeroFree;
}

/*
 * setLengthFour function takes one argument 's' of type string
 * Makes the string shorter if it is longer than 4 characters by truncating values
 * Makes the string longer if it is shorter than 4 characters by adding zeros
 * Does nothing if the string is already 4 characters long
*/
string setLengthFour(string s) {
    if (s.length() > 4) {
	string result;
	for (int i = 0; i < 4; i++) {
	    result += s[i];
	}
	return result;
    }

    while (s.length() != 4) {
	s += "0";
    }

    return s;
}


/* soundex function takes one argument 's' of type string that will be converted
 * into a four-digit code based on its syllables.
 */
string soundex(string s) {
    // Gets only letters
    string sanitized = lettersOnly(s);

    // Creates long form code
    string code = toDigits(sanitized);

    // Condense adjacent numbers
    string combinedCode = removeDuplicates(code);

    // Replace first index with first letter from string
    combinedCode[0] = toupper(sanitized[0]);

    // Strip zeros
    string zeroFree = removeZeros(combinedCode);

    // Enforce length 4
    string result = setLengthFour(zeroFree);

    return result;
}

/* soundexSearch takes one argument 'filepath' of type string.
 * It loads names from a file at the provide path and allows
 * the user to search through the list and see names
 * with matching soundex codes to the one they inputted.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    while (true) {

    string query = getLine("\nEnter a surname (RETURN to quit): ");

    if (query == "RETURN") {
	cout << "All done!" << endl;
	return;
    }

    query = soundex(query);

    cout << "Soundex code is " << query << endl;

    Vector<string> result;

    for (int i = 0; i < allNames.size(); i++) {
	string soundexed = soundex(allNames[i]);
	if (soundexed == query) {
	    result.add(allNames[i]);
	}
    }

    result.sort();

    cout << "Matches from database: " << result << endl;

    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

STUDENT_TEST("Testing lettersOnly with empty string") {
    string s = "";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Testing lettersOnly with numbers only") {
    string s = "67";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Testing lettersOnly with name that starts with number") {
    string s = "6Odelia";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "Odelia");
}

STUDENT_TEST("Tests for toDigits") {
    EXPECT_EQUAL(toDigits("Cooper"), "200106");
    EXPECT_EQUAL(toDigits("Odelia"), "030400");
}

STUDENT_TEST("Tests for removeDuplicates") {
    EXPECT_EQUAL(removeDuplicates("200106"), "20106");
    EXPECT_EQUAL(removeDuplicates("030400"), "03040");
}

STUDENT_TEST("Tests for removeZeros") {
    EXPECT_EQUAL(removeZeros("C0106"), "C16");
    EXPECT_EQUAL(removeZeros("O3040"), "O34");
}

STUDENT_TEST("Tests for setLengthFour") {
    EXPECT_EQUAL(setLengthFour("C16"), "C160");
    EXPECT_EQUAL(setLengthFour("O34"), "O340");
    EXPECT_EQUAL(setLengthFour("C1618234888"), "C161");
    EXPECT_EQUAL(setLengthFour("J"), "J000");
    EXPECT_EQUAL(setLengthFour("E752"), "E752");
}
