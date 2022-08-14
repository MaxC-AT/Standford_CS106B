/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include <unordered_set>
#include <vector>
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    /* TODO: Implement this function. */
    if (str.length() <= 3) return 0;
    return str.length() - 3;
}

int getScore(string str, Lexicon& lex, unordered_set<string>& visitedWord) {
    /* TODO: Implement this function. */
    if (visitedWord.find(str) != visitedWord.end()) return 0;
    if (!lex.contains(str)) return 0;

    visitedWord.insert(str);
    return points(str);
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
 vector<vector<int>> getNeighbors(int row, int col, Grid<char>& board) {
    vector<vector<int>> neighbors;
    vector<vector<int>> dirs {{0,1}, {1,0}, {0,-1}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for (vector<int>& dir: dirs) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        if (newRow >= 0 && newRow < board.numRows() && newCol >= 0 && newCol < board.numCols() && board[newRow][newCol] != '#') {
            neighbors.push_back({newRow, newCol});
        }
    }
    return neighbors;
}

void scoreBoardHelp(int row, int col, int& score, string currString, Grid<char>& board, Lexicon& lex, unordered_set<string>& visitedWord) {
    if (!currString.empty() && !lex.containsPrefix(currString)) return;

    score += getScore(currString, lex, visitedWord);
    for (vector<int> neighbor: getNeighbors(row, col, board)) {
        int nextRow = neighbor[0];
        int nextCol = neighbor[1];
        char nextChar = board[nextRow][nextCol];
        board[nextRow][nextCol] = '#';
        scoreBoardHelp(nextRow, nextCol, score, currString + nextChar, board, lex, visitedWord);
        board[nextRow][nextCol] = nextChar;
    }
    return;
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    int rows = board.numRows();
    int cols = board.numCols();
    unordered_set<string> visitedWord;

    int score = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
             scoreBoardHelp(r, c, score, "", board, lex, visitedWord);
        }
    }
    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'R','E','P','W'},
                        {'A','S','N','S'},
                        {'E','O','R','U'},
                        {'T','O','H','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 282);
}

STUDENT_TEST("Test scoreBoard, big full board, large number of words") {
    Grid<char> board = {{'A','I','X','A','S'},
                        {'G','T','T','C','M'},
                        {'S','W','Y','R','O'},
                        {'W','O','E','T','E'},
                        {'S','C','D','S','N'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 429);
}
