//
// Created by zulus on 13.05.17.
//

#ifndef LAB8_TEXTPROCESSOR_H
#define LAB8_TEXTPROCESSOR_H

#include <string>

class TextProcessor {
private:
    TextProcessor();

public:
    static std::string read(const std::string &filename);

    static size_t fileSize(const std::string &filename);

    static void process(const std::string &text, long &max, long &count);
};


#endif //LAB8_TEXTPROCESSOR_H
