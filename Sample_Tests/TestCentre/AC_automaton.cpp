#include "AC_automaton.h"

#include <queue>
#include <stdexcept>
#include <limits>

using namespace std;

struct Node{
    long long StateID; 
    int outCount;
    vector<struct Node*> next_node;
    vector<bool> Outputs;
    struct Node* failure_node;

    Node(long long id, int LetterSize, int KeywordSize) {
        StateID = id;
        next_node.assign(LetterSize, nullptr);
        Outputs.assign(KeywordSize, false);
        failure_node = nullptr;
        outCount = 0;
    }
};
typedef struct Node Node;

void AC_Automaton :: constructGoto(void) {
    for(int i=0; i<kwSize; i++){
        insertKeyword(Dictionary[i], i);
    }
}

void AC_Automaton :: insertKeyword(const vector<int> &keyword, int keyID) {
    if (keyID < 0 || keyID >= kwSize) {
        throw std::out_of_range("Invalid keyword ID");
    }

    if (keyword.empty()) {
        throw invalid_argument("Empty patterns are not supported");
    }

    Node* newState = TrieRoot;

    for(int key : keyword){
        if (key < 0 || key >= alphaSize) {
            throw std::out_of_range("Invalid alphabet symbol");
        }

        if(newState->next_node[key] == nullptr){
            newState->next_node[key] = new Node(ID++, alphaSize, kwSize);
        }
        newState = newState->next_node[key];
    }

    if(!newState->Outputs[keyID]){
        newState->Outputs[keyID] = true;
        newState->outCount++;
    }
}
    
void AC_Automaton :: constructFailure(void) {
    queue<Node*> nodeQ;

    for(Node* next : TrieRoot->next_node){
        if(next != nullptr){
            next->failure_node = TrieRoot;
            nodeQ.push(next);
        }
    }

    while(!nodeQ.empty()){
        Node* R = nodeQ.front(); nodeQ.pop();

        for(int a=0; a<alphaSize; a++)
        {
            if(R->next_node[a] == nullptr){ continue; }
            Node* S = R->next_node[a];
            nodeQ.push(S);

            Node* st = R->failure_node;
            while(st!=TrieRoot && st->next_node[a] == nullptr){
                st = st->failure_node;
            }
            // S->failure_node = st->next_node[a];

            if(st->next_node[a] != nullptr){ S->failure_node = st->next_node[a]; }
            else{ S->failure_node = TrieRoot; }

            if(S->failure_node != TrieRoot){
                unionResult(S, S->failure_node);
            }
        }
    }
}

void AC_Automaton :: unionResult(Node* out, Node* in) {
    out->outCount = 0;
    for(int i=0; i<kwSize; i++){
        out->Outputs[i] = out->Outputs[i] || in->Outputs[i];
        out->outCount += out->Outputs[i];
    }
}

void AC_Automaton :: findOuts(const vector<bool> &outs, size_t ind, vector<vector<int>> &Res) {
    size_t endInd = ind + 1;
    for(int i=0; i<kwSize; i++){
        if(outs[i]){
            if(Dictionary[i].size() <= endInd){
                size_t startInd = endInd - Dictionary[i].size();
                Res[startInd].push_back(i);
            }
        }
    }
}

void AC_Automaton :: destroyNode(Node* x) {
    if(x == nullptr){ return; }

    for(Node* next : x->next_node){
        if(next != nullptr){
            destroyNode(next);
        }
    }

    delete(x);
}

AC_Automaton :: AC_Automaton(int as, const vector<vector<int>>& dict) {
    if (as <= 0) {
        throw std::invalid_argument("Alphabet size must be positive");
    }

    if (dict.size() > static_cast<size_t>(numeric_limits<int>::max())) {
        throw length_error("Too many keywords");
    }

    this -> alphaSize = as;
    this -> kwSize = static_cast<int>(dict.size());
    this -> Dictionary.assign(dict.begin(), dict.end());
    this -> ID = 0;

    TrieRoot = new Node(ID++, alphaSize, kwSize);
    TrieRoot->failure_node = TrieRoot;

    try {
        this -> constructGoto();
        this -> constructFailure();
    }
    catch (...) {
        destroyNode(TrieRoot);
        TrieRoot = nullptr;
        throw;
    }
}

AC_Automaton :: ~AC_Automaton() {
    this -> destroyNode(TrieRoot);
}

vector<vector<int>> AC_Automaton :: matchPattern(const vector<int> &in) {
    size_t N = in.size();
    vector<vector<int>> Res(N, vector<int> ());

    Node* state = TrieRoot;
    for(size_t i=0; i<N; i++){
        int a = in[i];
        if (a<0 || a>=alphaSize) {
            throw std::out_of_range("Invalid input symbol");
        }

        while(state->next_node[a] == nullptr){
            if(state == TrieRoot){ break; }
            state = state->failure_node;
        }

        if(state->next_node[a] != nullptr){ state = state->next_node[a]; }
        
        if(state->outCount > 0){
            findOuts(state->Outputs, i, Res);
        }

    }

    return Res;
}
