/** @file
    @brief Text & file processor
*/

#ifndef LAB8_TEXTPROCESSOR_H
#define LAB8_TEXTPROCESSOR_H

#include <string>

/**
 @class TextProcessor
 @brief TextProcessor type, for process string data
 */
class TextProcessor {
private:
    /**
     @brief private TextProcessor constructor
     */
    TextProcessor();

public:
    /**
     @brief read text in file and return it
     @param filename file's path
     @return read text from file (return empty string, if cannot open file)
     */
    static std::string read(const std::string &filename);

    /**
    @brief return size of file in bytes
    @param filename file's path
    @return size of file in bytes (or 0, if cannot open file)
     */
    static size_t fileSize(const std::string &filename);

    /**
     @brief process text data and calculate number of integers in text and its max value
     @param text text to processing
     @param max reference to max value of integers in text
     @param count reference to count of integers in text
     */
    static void process(const std::string &text, long &max, long &count);
};


#endif //LAB8_TEXTPROCESSOR_H
