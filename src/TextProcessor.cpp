//
// Created by zulus on 13.05.17.
//

#include "TextProcessor.h"
#include <fstream>
#include <iostream>

using namespace std;


size_t TextProcessor::fileSize(const std::string &filename) {
    fstream myfile(filename, ios::binary);
    if (myfile.is_open()) {
        auto begin = myfile.tellg();
        myfile.seekg(0, ios::end);
        auto end = myfile.tellg();
        myfile.close();
        return end - begin;
    } else {
        return 0;
    }
}

std::string TextProcessor::read(const std::string &filename) {
    ifstream t(filename);
    string str = "";
    if (t.is_open()) {
        t.seekg(0, std::ios::end);
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
        t.close();
    }
    return str;
}

void TextProcessor::process(const std::string &text, long &max, long &count) {
    count = 0;
    max = INT32_MIN;
    auto ctext = text.c_str();
    for (size_t i = 0; i < text.length(); i++) {
        if (isdigit(text[i]) || (text[i] == '-' && isdigit(text[i + 1]))) {
            auto tmp = atol(ctext + i);
            if (tmp > max) {
                max = tmp;
            }
            count++;
            i += to_string(tmp).length();
        }
    }
}

TextProcessor::TextProcessor() {}
