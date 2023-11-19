#pragma once

//
// Created by plusleft on 11/18/2023.
//

#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class InlineCHandler {
private:
    vector<string> functions;

public:
    string run(string filename, string function_name, vector<string> args) {
        // load the library
        string filename_relative = "violence/" + filename + "_inline_c_violence.dll";
        void *handle = dlopen(filename_relative.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Error loading library: " << dlerror() << std::endl;
            return "";
        }

        // get the function
        void* func = dlsym(handle, function_name.c_str());
        if (!func) {
            std::cerr << "Error getting function: " << dlerror() << std::endl;
            dlclose(handle);
            return "";
        }

        // it is a char* return type no args function
        typedef char* (*func_t)();
        func_t func_ptr = (func_t)func;
        char* output = func_ptr();
        string output_string = string(output);
        dlclose(handle);

        return output_string;
    }

    InlineCHandler();
    void add_function(string function);
    void compile(string filename);
    static void clean();
    ~InlineCHandler();
};

