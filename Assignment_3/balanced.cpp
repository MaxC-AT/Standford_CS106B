/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include <unordered_set>
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isOperator(char c) {
    unordered_set<char> operators {'(', ')', '[', ']', '{', '}'};
    return (operators.find(c) == operators.end())? false:true;
}


string operatorsFrom(string str) {
    /* TODO: Implement this function. */
    string res = "";
    for (char& c:str) {
        if (isOperator(c)) res += c;
    }
    return res;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool matched(const char c1, const char c2) {
    if ((c1 == '(' && c2 == ')') ||
        (c1 == '[' && c2 == ']') ||
        (c1 == '{' && c2 == '}')) return true;
    return false;
}

bool operatorsAreMatched(string ops) {
    /* TODO: Implement this function. */
    if (ops.empty()) return true;
    char prev = '#';
    char curr = '#';
    for (int i = 0; i < ops.length(); i++) {
        curr = ops[i];
        if (matched(prev, curr)) {
            ops = ops.erase(i-1, 2);
            return operatorsAreMatched(ops);
        }
        prev = curr;
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

STUDENT_TEST("operatorsFrom test") {
    EXPECT_EQUAL(operatorsFrom("int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }"), "(){([])(())}");
}

STUDENT_TEST("operatorsAreMatched test") {
    EXPECT(operatorsAreMatched(""));
    EXPECT(operatorsAreMatched("{}[]()"));
    EXPECT(operatorsAreMatched("((()))"));

    EXPECT(!operatorsAreMatched("([{}]"));
    EXPECT(!operatorsAreMatched("()[]{)"));
    EXPECT(!operatorsAreMatched("((())"));
    EXPECT(!operatorsAreMatched(")))((("));
}
