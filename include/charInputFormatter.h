#ifndef FORMAT_CHAR_INT
#define FORMAT_CHAR_INT

#include <vector>
#include <string>
#include <cstddef>

class charInputFormatter {
private:
    std::size_t N;
    std::vector<std::string> in;
    std::vector<std::vector<int>> dictionary;

    bool insertIntoDict(const std::string &str, std::size_t ind);
public:
    charInputFormatter(const std::vector<std::string> &inputs);
    const std::vector<std::vector<int>> getDictionary(void) const;
    std::vector<int> formatInput(const std::string &str);
    std::vector<int> formatInput2(const std::string &str);
    bool findKeyword(std::size_t keyID, std::string &str);
    std::string findKeyWord(std::size_t keyID);
};

#endif