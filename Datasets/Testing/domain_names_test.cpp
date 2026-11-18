#include <iostream>
#include <vector>
#include <string>

#include "AC_automaton.h"
#include "charInputFormatter.h"

// For time
#include <chrono>
using namespace std::chrono;

// std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::_V2::steady_clock::duration> std::chrono::_V2::steady_clock::time_point
#define CLK std::chrono::time_point<std::chrono::steady_clock>

using namespace std;

int main()
{
    size_t N;
    cin >> N;

    
    cout << "Start Input\n";
    vector<string> input(N);
    for(size_t i=0; i<N; i++){
        cin >> input[i];
    }
    cout << "End Input\n";

    cout << "Start Encode\n";
    CLK encodeStart = std::chrono::steady_clock::now();
    charInputFormatter encoder(input);
    vector<vector<int>> dictionary = encoder.getDictionary();
    CLK encodeEnd = std::chrono::steady_clock::now();
    cout << "End Encode\n";

    // Stage 1: Automaton construction
    AC_Automaton* AC = nullptr;
    CLK ACStart, ACEnd;
    try {
        cout << "Start AC\n";
        ACStart = std::chrono::steady_clock::now();
        AC = new AC_Automaton(128, dictionary);
        ACEnd = std::chrono::steady_clock::now();
        cout << "End AC\n";
        // cout << "Automaton construction successful!\n";
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    }

    std::chrono::microseconds microDiff = std::chrono::duration_cast<std::chrono::microseconds>(encodeEnd - encodeStart);
    std::chrono::milliseconds milliDiff = std::chrono::duration_cast<std::chrono::milliseconds>(encodeEnd - encodeStart);
    cout << "Encode: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;

    microDiff = duration_cast<microseconds>(ACEnd - ACEnd);
    milliDiff = duration_cast<milliseconds>(ACEnd - ACStart);
    cout << "Automaton Construction: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
    cout << "Node Count: " << AC -> findNodeCount() << endl;

    int op;
    while(1)
    {
        cout << "Enter [0:Exit | 1:Input]: ";
        cin >> op;
        if(op<0 || op>1){ continue; }
        if(op == 0){ break; }

        string full_line_input;
        std::getline(std::cin.ignore(), full_line_input); // Reads spaces, tabs, punctuation, etc.

        vector<int> acIn = encoder.formatInput(full_line_input);
        vector<vector<int>> actual;
        try {
            ACStart = std::chrono::steady_clock::now();
            actual = AC->matchPattern(acIn);
            ACEnd = std::chrono::steady_clock::now();
        }
        catch (const exception& e) {
            cerr << "Pattern matching failed: "
                << e.what() << '\n';
            delete AC;
            return 1;
        }
        
        microDiff = duration_cast<microseconds>(ACEnd - ACEnd);
        milliDiff = duration_cast<milliseconds>(ACEnd - ACStart);
        cout << "Pattern Len: " << acIn.size();
        cout << "Pattern Time: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;

        cout << "Keywords:\n";
        for(int i=0; i<actual.size(); i++){
            cout<<i<<" : ";
            for(int k : actual[i]){ cout << encoder.findKeyWord(k) << ", "; }
            cout<< endl;
        }
        cout << "\n--------------------------------\n\n";
    }
    
    cout << "All Done..." << endl;
    return 0;
}