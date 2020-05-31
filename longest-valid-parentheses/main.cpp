#include <iostream>
#include <string>
#include <stack>

using namespace std;

// Given a string containing just the characters '(' and ')',
// find the length of the longest valid (well-formed) parentheses substring.
size_t longestValidParentheses(string s) {
    stack<size_t> counts;

    size_t length = 0;
    size_t max_length = 0;
    for (size_t i = 0, n = s.length(); i < n; i++) {
        auto c = s[i];
        if (c == '(') {
            counts.push(++length);
            length = 0;
        } else if (c == ')') {
            if (counts.size()) {
                auto top_length = counts.top();
                counts.pop();
                if (top_length % 2 == 1) {
                    length = length + top_length + 1;
                    if (length > max_length) {
                        max_length = length;
                    }
                }
            } else {
                length = 0;
            }
        } else {
            return 0;
        }
    }

    return max_length;
}

int main() {
    string input_string;
    cout << "Enter a string: ";
    getline(cin, input_string);
    cout << endl << "For string '" << input_string << "'" << endl;
    cout << "the longest valid parentheses is: " << longestValidParentheses(input_string) << endl;
    return 0;
}
