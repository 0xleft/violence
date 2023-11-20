//
// Created by plusleft on 11/18/2023.
//

#include "InlineCHandler.h"
#include <string>

using namespace std;

InlineCHandler::InlineCHandler() {
    this->functions = vector<string>();
}

void InlineCHandler::add_function(string function) {
    this->functions.push_back(function);
}

void InlineCHandler::compile(string filename) {
    // put all in the same file
    string content = "";
    for (int i = 0; i < this->functions.size(); i++) {
        content += this->functions[i] + "\n";
    }

    string command = filename + "_inline_c_violence.c";

    // write to file
    FILE *file = fopen(command.c_str(), "w");
    fprintf(file, "%s", content.c_str());
    fclose(file);

    // create dir violence
    system("mkdir -p violence");

    // compile
    command = "gcc -shared -fPIC -o violence/"+ filename +"_inline_c_violence.dll "+ filename +"_inline_c_violence.c";
    // if failed to compile
    if (system(command.c_str()) != 0) {
        printf("Error: Failed to compile inline C code for %s\n", filename.c_str());
    }

    command = filename + "_inline_c_violence.c";
    remove(command.c_str());
}

void InlineCHandler::clean() {
    system("rm -rf violence/*");
}

InlineCHandler::~InlineCHandler() {
}