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

    // function to call the incline C function with args...
    template <typename... Args>
    std::string callFunction(void* func, Args&&... args) {
        typedef char* (*func_t)(Args...);
        func_t func_ptr = (func_t)func;
        char* output = func_ptr(args...);
        string output_string = string(output);
        return output_string;
    }

    // helper function to transform vector<string> to Args...
    std::string callFunctionWithVector(void* func, const vector<string>& args) {
        std::vector<std::string> transformedArgs;
        transformedArgs.reserve(args.size());
        for (const auto& arg : args) {
            transformedArgs.emplace_back(arg);
        }

        return callFunction(func, transformedArgs);
    }

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

        string output_string = callFunctionWithVector(func, args);

        dlclose(handle);

        return output_string;
    }

    InlineCHandler();
    void add_function(string function);
    void compile(string filename);
    static void clean();
    ~InlineCHandler();
};

