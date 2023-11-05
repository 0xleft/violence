//
// Created by plusleft on 11/5/2023.
//

#include <string>
#include <fstream>

using namespace std;

class Reader {
private:
    string filename;
    string content;
    ifstream file;

public:
    Reader(string filename);
    void close();
    string get_content();
    string get_filename();
    ~Reader();
};