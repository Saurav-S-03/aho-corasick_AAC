// #include "AC-automaton.h"

// #include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>

using namespace std;

struct Node{
    int StateID, outCount;
    vector<struct Node*> next_node;
    vector<bool> Outputs;
    struct Node* failure_node;

    Node(int id, int LetterSize, int KeywordSize) {
        StateID = id;
        next_node.assign(LetterSize, NULL);
        Outputs.assign(KeywordSize, false);
        failure_node = NULL;
        outCount = 0;
    }
    
};
typedef struct Node Node;

class AC_Automaton {
    Node* TrieRoot;
    int alphaSize, kwSize, ID;
    vector<vector<int>> Dictionary;

    inline void constructGoto(void) {
        for(int i=0; i<kwSize; i++){
            insertKeyword(Dictionary[i], i);
        }
    }
    
    inline void insertKeyword(const vector<int> &keyword, int keyID) {
        if (keyID < 0 || keyID >= kwSize) {
            throw std::out_of_range("Invalid keyword ID");
        }

        Node* newState = TrieRoot;
    
        for(int key : keyword){
            if (key < 0 || key >= alphaSize) {
                throw std::out_of_range("Invalid alphabet symbol");
            }

            if(newState->next_node[key] == NULL){
                newState->next_node[key] = new Node(ID++, alphaSize, kwSize);
            }
            newState = newState->next_node[key];
        }
    
        if(!newState->Outputs[keyID]){
            newState->Outputs[keyID] = true;
            newState->outCount++;
        }
    }
    
    inline void constructFailure(void) {
        queue<Node*> nodeQ;
    
        for(Node* next : TrieRoot->next_node){
            if(next != NULL){
                next->failure_node = TrieRoot;
                nodeQ.push(next);
            }
        }
    
        while(!nodeQ.empty()){
            Node* R = nodeQ.front(); nodeQ.pop();
    
            for(int a=0; a<alphaSize; a++)
            {
                if(R->next_node[a] == NULL){ continue; }
                Node* S = R->next_node[a];
                nodeQ.push(S);
    
                Node* st = R->failure_node;
                while(st!=TrieRoot && st->next_node[a] == NULL){
                    st = st->failure_node;
                }
                // S->failure_node = st->next_node[a];
    
                if(st->next_node[a] != NULL){ S->failure_node = st->next_node[a]; }
                else{ S->failure_node = TrieRoot; }
    
                if(S->failure_node != TrieRoot){
                    unionResult(S, S->failure_node);
                }
            }
        }
    }
    
    inline void unionResult(Node* out, Node* in) {
        out->outCount = 0;
        for(int i=0; i<kwSize; i++){
            out->Outputs[i] = out->Outputs[i] | in->Outputs[i];
            out->outCount += out->Outputs[i];
        }
    }

    inline void findOuts(const vector<bool> &outs, int ind, vector<vector<int>> &Res) {
        ind++;
        for(int i=0; i<kwSize; i++){
            if(outs[i]){
                int startInd = ind - Dictionary[i].size();
                if(startInd >= 0){
                    Res[startInd].push_back(i);
                }
            }
        }
    }

    void destroyNode(Node* x) {
        if(x == NULL){ return; }

        for(Node* next : x->next_node){
            if(next != NULL){
                destroyNode(next);
            }
        }

        delete(x);
    }

public:
    AC_Automaton(int as, const vector<vector<int>> dict) {
        if (as <= 0) {
            throw std::invalid_argument("Alphabet size must be positive");
        }

        this -> alphaSize = as;
        this -> kwSize = static_cast<int>(dict.size());
        this -> Dictionary.assign(dict.begin(), dict.end());
        this -> ID = 0;

        TrieRoot = new Node(ID++, alphaSize, kwSize);
        TrieRoot->failure_node = TrieRoot;

        this -> constructGoto();
        this -> constructFailure();
    }

    ~AC_Automaton() {
        this -> destroyNode(TrieRoot);
    }

    vector<vector<int>> matchPattern(const vector<int> &in) {
        int N = in.size();
        vector<vector<int>> Res(N, vector<int> ());

        Node* state = TrieRoot;
        for(int i=0; i<N; i++){
            int a = in[i];
            if (a<0 || a>=alphaSize) {
                throw std::out_of_range("Invalid input symbol");
            }

            while(state->next_node[a] == NULL){
                if(state == TrieRoot){ break; }
                state = state->failure_node;
            }

            if(state->next_node[a] != NULL){ state = state->next_node[a]; }
            
            if(state->outCount > 0){
                findOuts(state->Outputs, i, Res);
            }

        }

        return Res;
    }

};