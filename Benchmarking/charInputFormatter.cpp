#include "charInputFormatter.h"
// #include <vector>
// #include <string>

#include <utility>

using namespace std;

charInputFormatter :: charInputFormatter(const std::vector<std::string> &inputs) {
    this->in.assign(inputs.begin(), inputs.end());
    this->N = inputs.size();
    this->dictionary.resize(N);

    for(std::size_t i=0; i<N; i++){
        insertIntoDict(in[i], i);
    }
}

bool charInputFormatter :: insertIntoDict(const std::string &str, std::size_t ind) {
    if(ind<0 || ind>=N){ return false; }
    
    std::vector<int> entry(str.size());
    for(std::size_t i=0; i<str.size(); i++){
        entry[i] = (int)str[i];
    }
    this -> dictionary[ind] = std::move(entry);

    return true;
}

const std::vector<std::vector<int>> charInputFormatter :: getDictionary(void) const {
    return this->dictionary;
}

std::vector<int> charInputFormatter :: formatInput(const std::string &str) {
    vector<int> res(str.length());

    for(size_t i=0; i<str.length(); i++){
        res[i] = (int)str[i];
    }
    return res;
}

std::vector<int> charInputFormatter :: formatInput2(const std::string &str) {
    vector<int> res(str.length());

    for(size_t i=0; i<str.length(); i++){
        if(str[i]>='A' && str[i]<='Z'){
            res[i] = (int)('a' + str[i] - 'A');
        }
        else{ res[i] = (int)str[i]; }
    }
    return res;
}

bool charInputFormatter :: findKeyword(std::size_t keyID, std::string &str) {
    if(keyID<0 || keyID>=N){ return false; }
    str = this->in[keyID];
    return true;
}

std::string charInputFormatter :: findKeyWord(std::size_t keyID) {
    if(keyID<0 || keyID>=N){ return "\0"; }
    return this->in[keyID];
}