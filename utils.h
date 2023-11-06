//
// Created by plusleft on 11/6/2023.
//

#include <string>

using namespace std;

static vector<string> split(string str, string pattern) {
    string::size_type pos;
    vector<string> result;
    str += pattern;
    int size = str.size();
    for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}