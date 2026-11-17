#include <iostream>
#include <vector>
#include <string>

#include "AC_automaton.h"

using namespace std;

// Prints the result in a readable format
void printResult(const vector<vector<int>>& result) {
    for (size_t i = 0; i < result.size(); i++) {
        cout << "Index " << i << ": ";

        if (result[i].empty()) {
            cout << "-";
        }
        else {
            for (size_t j = 0; j < result[i].size(); j++) {
                cout << result[i][j];

                if (j + 1 < result[i].size()) {
                    cout << ", ";
                }
            }
        }

        cout << '\n';
    }
}

// Compares actual and expected outputs
bool compareResults(
    const vector<vector<int>>& actual,
    const vector<vector<int>>& expected
) {
    bool ret = actual == expected;

    cout << "\nExpected output:\n";
    printResult(expected);

    cout << "\nActual output:\n";
    printResult(actual);

    if (ret) {
        cout << "\nTEST PASSED!\n";
    }
    else{
        cout << "\nTEST FAILED!\n";
    }
    return ret;
}

int main() {
    int M, N;

    int alpha = 0;
    cout << "Enter Alphabet Size: ";
    cin >> alpha;

    // Dictionary
    cout << "Enter Dictionary Size: ";
    cin >> M;
    vector<vector<int>> dict(M, vector<int> ());

    for(int i=0; i<M; i++){
        printf("Enter %dth Keyword Size: ", i);
        cin >> N;

        dict[i].resize(N);
        printf("Enter %dth Keyword: ", i);
        for(int &x : dict[i]){ cin >> x; }
    }

    // Stage 1: Automaton construction
    AC_Automaton* AC = nullptr;

    try {
        AC = new AC_Automaton(alpha, dict);
        cout << "Automaton construction successful!\n";
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    }

    // Stage 2: Read input
    cout << "Enter Input Length: ";
    cin >> N;
    
    vector<int> input(N);
    cout << "Enter Input: ";
    for(int &x : input){ cin >> x; }


    // Expected output
    vector<vector<int>> expected(N);
    for(int i=0; i<N; i++){
        printf("Enter %dth Expected Out Size: ", i);
        cin >> M;

        expected[i].resize(M);
        printf("Enter %dth Exp. Out: ", i);
        for(int &x : expected[i]){ cin >> x; }
    }

    // Stage 3: Pattern matching
    vector<vector<int>> actual;

    try {
        actual = AC->matchPattern(input);
        cout << "Pattern matching successful!\n";
    }
    catch (const exception& e) {
        cerr << "Pattern matching failed: "
             << e.what() << '\n';
        delete AC;
        return 1;
    }

    // Compare results
    compareResults(actual, expected);
    return 0;
}
