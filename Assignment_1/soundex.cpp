/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */

int CODE_LENGTH = 4;

string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}


char getSoundexcode(char c) {
    char code;
    if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'H' || c == 'W' || c == 'Y') code = '0';
    else if (c == 'B' || c == 'F' || c == 'P' || c == 'V') code = '1';
    else if (c == 'C' || c == 'G' || c == 'J' || c == 'K' || c == 'Q' || c == 'S' || c == 'X' || c == 'Z') code = '2';
    else if (c == 'D' || c == 'T') code = '3';
    else if (c == 'L') code = '4';
    else if (c == 'M' || c == 'N') code = '5';
    else code = '6';
    return code;
}

string coalesceAdjcentDuplicate(string soundex) {
    char prev = '#';
    string result;
    for (char& c: soundex) {
        if (c == prev) continue;
        result += c;
        prev = c;
    }
    return result;
}

void replaceFirstDigit(string& soundex, const char* firstLetter) {
    soundex[0] = toupper(*firstLetter);
}

string removeZeros(string soundex) {
    string result;
    for (int i = 0; i < soundex.size(); i++) {
        if (soundex[i] != '0') result += soundex[i];
    }
    return result;
}

string padOrTruncateSoundexCode(string soundex) {
    string result;
    if (soundex.size() > CODE_LENGTH) {
      for (int i = 0; i < CODE_LENGTH; i++) result += soundex[i];
    } else {
        result = soundex;
        int padTimes = CODE_LENGTH - soundex.size();
        for (int i = 0; i < padTimes; i++) result += '0';
    }
    return result;
}

string convertToSoundexCode(string letters) {
    string result;
    for (char& c: letters) {
        result += getSoundexcode(toupper(c));
    }
    result = coalesceAdjcentDuplicate(result);
    replaceFirstDigit(result, &letters[0]);
    result = removeZeros(result);
    result = padOrTruncateSoundexCode(result);
    return result;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    string letters = removeNonLetters(s);
    return convertToSoundexCode(letters);
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void convertNames2Soundexes(Vector<string>& databaseSoundexes, Vector<string>& databaseNames) {
    for (string& name : databaseNames) {
        databaseSoundexes.add(soundex(name));
    }
}

void findMatchedSoundexes(string refcode, Vector<string>& databaseNames, Vector<string>& databaseSoundexes, Vector<string>& matched) {
    for (int i = 0; i < databaseSoundexes.size(); i++) {
        string code = databaseSoundexes[i];
        if (code == refcode) matched.add(databaseNames[i]);
    }
}


void printMatchedNames(Vector<string>& matchedNames) {
    cout << "Matches from database: " << matchedNames << endl;
}


void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    Vector<string> databaseSoundexes;
    convertNames2Soundexes(databaseSoundexes, databaseNames);

    /* TODO: Fill in the remainder of this function. */
    string surname;
    string soundcode;
    while (true) {
        surname = getLine("Enter a surname (RETURN to quit): ");
        if (surname == "") break;
        soundcode = soundex(surname);

        Vector<string> matchedNames;
        findMatchedSoundexes(soundcode, databaseNames, databaseSoundexes, matchedNames);

        matchedNames.sort();
        printMatchedNames(matchedNames);
    }
    return;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
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

// TODO: add your test cases here
STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s1 = " O'Hara";
    string result1 = removeNonLetters(s1);
    EXPECT_EQUAL(result1, "OHara");

    string s2 = "    ";
    string result2 = removeNonLetters(s2);
    EXPECT_EQUAL(result2, "");
}

STUDENT_TEST("Test coalesceAdjcentDuplicate") {
    string s1 = "2";
    string result1 = coalesceAdjcentDuplicate(s1);
    EXPECT_EQUAL(result1, "2");

    string s2 = "22";
    string result2 = coalesceAdjcentDuplicate(s2);
    EXPECT_EQUAL(result2, "2");

    string s3 = "220";
    string result3 = coalesceAdjcentDuplicate(s3);
    EXPECT_EQUAL(result3, "20");

    string s4 = "200";
    string result4 = coalesceAdjcentDuplicate(s4);
    EXPECT_EQUAL(result4, "20");

    string s5 = "2002";
    string result5 = coalesceAdjcentDuplicate(s5);
    EXPECT_EQUAL(result5, "202");
}

STUDENT_TEST("Test replaceFirstDigit") {
    string s1 = "2";
    replaceFirstDigit(s1, "A");
    EXPECT_EQUAL(s1, "A");

    string s2 = "22";
    replaceFirstDigit(s2, "A");
    EXPECT_EQUAL(s2, "A2");
}

STUDENT_TEST("Test removeZeros") {
    string s1 = "2";
    string result1;
    result1 = removeZeros(s1);
    EXPECT_EQUAL(result1, "2");

    string s2 = "0";
    string result2;
    result2 = removeZeros(s2);
    EXPECT_EQUAL(result2, "");

    string s3 = "020020";
    string result3;
    result3 = removeZeros(s3);
    EXPECT_EQUAL(result3, "22");
}

STUDENT_TEST("Test findMatchedSoundexes") {
    string soundcode = "A234";
    Vector<string> databaseNames {"ACDL", "BCFD", "AGTL", "CMRX"};
    Vector<string> databaseSoundexes {"A234", "B213", "A234", "C562"};
    Vector<string> expected {"ACDL", "AGTL"};
    Vector<string> matchedSoundex;
    findMatchedSoundexes(soundcode, databaseNames, databaseSoundexes, matchedSoundex);
    EXPECT_EQUAL(matchedSoundex, expected);
}
