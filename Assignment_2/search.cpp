// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string trimPunct(string s)
{
    int left = 0;
    int right = s.length()-1;
    while (ispunct(s[left])) left++;
    while (ispunct(s[right])) right--;
    string res;
    for (int i = left; i <= right; i++) res += s[i];
    return res;
}

string cleanToken(string s)
{
    string cleanedStr = "";
    int alphaCount = 0;
    s = trimPunct(s);
    for (char& chr:s) {
        if (isalpha(chr)) {
            chr = tolower(chr);
            alphaCount++;
        }
        cleanedStr += chr;
    }
    return (alphaCount > 0)? cleanedStr:"";
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> origTokens = stringSplit(text, " ");
    for (string& token:origTokens) {
        token = cleanToken(token);
        if (!token.empty()) tokens.add(cleanToken(token));
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int readDBFile(string dbfile, Vector<string>& lines) {
    ifstream in;
    string line;
    if (!openFile(in, dbfile)) {
        error("Cannot open file named " + dbfile);
    } else {
        while (getline(in, line)) lines.add(line);
    }
    return 0;
}


int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    Vector<string> lines;
    readDBFile(dbfile, lines);
    if (lines.isEmpty()) return 0;

    int page = 0;
    for (int i = 0; i < lines.size(); i += 2) {
        page++;
        Set<string> tokens = gatherTokens(lines[i+1]);
        for (string token: tokens) {
            if (!index.containsKey(token)) {
                Set<string> pages = {lines[i]};
                index.put(token, pages);
            } else {
                index[token].add(lines[i]);
            }
        }
    }
    return page;
}

void parseQuery(string query, Vector<string>& words, Vector<int>& actions) {
    int action;
    Vector<string> queries = stringSplit(query, " ");
    for (string& word:queries) {
        if (word[0] == '+') action = 1;
        else if (word[0] == '-') action = 2;
        else action = 0;

        word = cleanToken(word);
        if (word.length() == 0) continue;
        words.add(word);
        actions.add(action);
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> words;
    Vector<int> actions;
    parseQuery(query, words, actions);
    for (int i = 0; i < words.size(); i++){
        string word = words[i];
        int action = actions[i];
        if (!index.containsKey(word)) continue;
        if (i == 0) {
            result.unionWith(index[word]);
        } else {
            switch (action)
            {
                case 0:
                    result.unionWith(index[word]);
                    break;
                case 1:
                    result.intersect(index[word]);
                    break;
                case 2:
                    result.difference(index[word]);
            }
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    int pages = buildIndex(dbfile, index);
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << pages << " containing " << index.keys().size() << " unique terms" << endl;
    while (true) {
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if (query == "") break;
        Set<string> matches = findQueryMatches(index, query);
        cout << "Found " << matches.size() << " matching pages" << endl;
        cout << matches << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
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


// TODO: add your test cases here
STUDENT_TEST("cleanToken on strings with more than one punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("<<hello>>"), "hello");
    EXPECT_EQUAL(cleanToken("++he<>llo~~"), "he<>llo");
    EXPECT_EQUAL(cleanToken("~~woRLD"), "world");
    EXPECT_EQUAL(cleanToken("woRL!D>>>>"), "worl!d");
}

STUDENT_TEST("buildIndex tests") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(index["fish"], {"www.shoppinglist.com", "www.dr.seuss.net", "www.bigbadwolf.com"});
    EXPECT_EQUAL(index["blue"], {"www.rainbow.org", "www.dr.seuss.net"});
}

STUDENT_TEST("parseQuery tests") {
    string query = "red fish";
    Vector<string> words;
    Vector<int> actions;
    parseQuery(query, words, actions);
    EXPECT_EQUAL(words, {"red", "fish"});
    EXPECT_EQUAL(actions, {0, 0});

    query = "red -fish +blue";
    words.clear();
    actions.clear();
    parseQuery(query, words, actions);
    EXPECT_EQUAL(words, {"red", "fish", "blue"});
    EXPECT_EQUAL(actions, {0, 2, 1});

}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "Red fiSH");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedOrGreen = findQueryMatches(index, "red green");
    EXPECT_EQUAL(matchesRedOrGreen.size(), 2);
    Set<string> matchesRedOrFishAndBread = findQueryMatches(index, "red fish +bread");
    EXPECT_EQUAL(matchesRedOrFishAndBread.size(), 1);
    Set<string> matchesBlueOrOne = findQueryMatches(index, "blue one");
    EXPECT_EQUAL(matchesBlueOrOne.size(), 2);
    Set<string> matchesRedWithoutBlue= findQueryMatches(index, "red -blue");
    EXPECT_EQUAL(matchesRedWithoutBlue.size(), 0);
}
