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
    size_t N, totalSize=0;
    cin >> N;
    
    // cout << "Start Input\n";
    vector<string> input(N);
    for(size_t i=0; i<N; i++){
        cin >> input[i];
        totalSize += input[i].length();
    }
    // cout << "End Input\n";

    // cout << "Start Encode\n";
    CLK encodeStart = std::chrono::steady_clock::now();
    charInputFormatter encoder(input);
    vector<vector<int>> dictionary = encoder.getDictionary();
    CLK encodeEnd = std::chrono::steady_clock::now();
    // cout << "End Encode\n";

    // Stage 1: Automaton construction
    AC_Automaton* AC = nullptr;
    CLK ACStart, ACEnd, gotoStart, gotoEnd, failStart, failEnd;
    try {
        // cout << "Start AC\n";
        ACStart = std::chrono::steady_clock::now();
        AC = new AC_Automaton(128, dictionary);
        ACEnd = std::chrono::steady_clock::now();
        // cout << "End AC\n";
        // cout << "Automaton construction successful!\n";


        gotoStart = std::chrono::steady_clock::now();
        AC->constructGotoPaths();
        gotoEnd = std::chrono::steady_clock::now();

        failStart = std::chrono::steady_clock::now();;
        AC->constructFailurePaths();
        failEnd = std::chrono::steady_clock::now();;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    }

    std::chrono::nanoseconds microDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(encodeEnd - encodeStart);
    // std::chrono::milliseconds milliDiff = std::chrono::duration_cast<std::chrono::milliseconds>(encodeEnd - encodeStart);
    // cout << "Encode: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
    cout << microDiff.count() << endl;

    microDiff = duration_cast<nanoseconds>(ACEnd - ACStart);
    // milliDiff = duration_cast<milliseconds>(ACEnd - ACStart);
    // cout << "Automaton Construction: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
    cout << microDiff.count() << endl;
    // cout << "Node Count: " << AC -> findNodeCount() << endl;
    
    microDiff = duration_cast<nanoseconds>(gotoEnd - gotoStart);
    // milliDiff = duration_cast<milliseconds>(gotoEnd - gotoStart);
    // cout << "goto Construction: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
    cout << microDiff.count() << endl;
    
    microDiff = duration_cast<nanoseconds>(failEnd - failStart);
    // milliDiff = duration_cast<milliseconds>(failEnd - failStart);
    // cout << "Failure Construction: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
    cout << microDiff.count() << endl;
    cout << N << ' ' << totalSize << ' ' << AC -> findNodeCount() << endl;

    int op;

    if (!(cin >> op)) { goto The_End; }
    // Discard the single '\n' left after 'op' ONCE before entering the loop
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while(op > 0)
    {
        op--;
        string full_line_input;
        if (!std::getline(std::cin, full_line_input)) { break; } // Reads spaces, tabs, punctuation, etc.

        // Strip trailing '\r' in case input.txt uses Windows CRLF line endings
        if (!full_line_input.empty() && full_line_input.back() == '\r') {
            full_line_input.pop_back();
        }

        vector<int> acIn = encoder.formatInput2(full_line_input);
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
        
        microDiff = duration_cast<nanoseconds>(ACEnd - ACStart);
        // milliDiff = duration_cast<milliseconds>(ACEnd - ACStart);
        // cout << "Pattern Len: " << acIn.size();
        // cout << "Pattern Time: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
        
        // cout << "Word: " << full_line_input << endl;
        // cout << "Keywords: ";
        long long keyCount = 0;
        for(int i=0; i<actual.size(); i++){
            // cout<<i<<" : ";
            for(int k : actual[i]){ 
                // cout <<'<'<<i<<" , "<<encoder.findKeyWord(k)<< "> "; 
                keyCount++;
            }
            // cout<< endl;
        }
        cout << full_line_input.length() <<' '<< microDiff.count() <<' '<< keyCount << endl;
        // cout << "\n--------------------------------\n\n";
    }

The_End:
    // cout << "All Done..." << endl;
    return 0;
}