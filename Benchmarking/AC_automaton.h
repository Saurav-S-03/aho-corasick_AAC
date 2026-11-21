#ifndef AC_AUTOMATON_H
#define AC_AUTOMATON_H

#include <vector>
#include <cstddef>

struct Node;

class AC_Automaton {
private:
    Node* TrieRoot;
    int alphaSize, kwSize;
    long long ID;

    std::vector<std::vector<int>> Dictionary;

    void constructGoto();
    void insertKeyword(const std::vector<int>& keyword, int keyID);
    void constructFailure();

    void unionResult(Node* out, Node* in);
    void findOuts(
        const std::vector<bool>& outs,
        std::size_t ind,
        std::vector<std::vector<int>>& Res
    );

    void destroyNode(Node* x);

public:
    AC_Automaton(
        int as,
        const std::vector<std::vector<int>>& dict
    );

    ~AC_Automaton();

    // Prevent accidental shallow copies
    AC_Automaton(const AC_Automaton&) = delete;
    AC_Automaton& operator=(const AC_Automaton&) = delete;
    AC_Automaton(AC_Automaton&&) = delete;
    AC_Automaton& operator=(AC_Automaton&&) = delete;

    std::vector<std::vector<int>> matchPattern(
        const std::vector<int>& in
    );

    long long findNodeCount(void) const;

    void constructGotoPaths(void);
    void constructFailurePaths(void);
};

#endif
