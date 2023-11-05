//
// Created by plusleft on 11/5/2023.
//

#include <iostream>
#include <string>
#include "Reader.h"

using namespace std;

Reader::Reader(std::string filename) {
    this->filename = filename;
    // read the file and put in content
    file.open(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            content += line + "\n";
        }
    } else {
        cout << "Error opening file " << filename << endl;
    }
}

void Reader::close() {
    if (file.is_open()) {
        file.close();
    }
}

string Reader::get_content() {
    return content;
}

string Reader::get_filename() {
    return filename;
}

Reader::~Reader() {
    close();
}