#include <iostream>
#include <vector>
#include <string>
#include <papi.h>

// #include <atomic>
// #include <cstdlib>
// #include <malloc.h>
// #include <new>

#include "AC_automaton.h"
#include "charInputFormatter.h"

// For measuring RAM
// std::atomic<size_t> g_current_bytes{0};
// std::atomic<size_t> g_peak_bytes{0};

// For time
#include <chrono>
using namespace std::chrono;

// std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::_V2::steady_clock::duration> std::chrono::_V2::steady_clock::time_point
// #define CLK std::chrono::time_point<std::chrono::steady_clock>

using namespace std;

int main()
{
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        std::cerr << "PAPI init failed\n";
        return 1;
    }
    int event_set = PAPI_NULL;
    if (PAPI_create_eventset(&event_set) != PAPI_OK ||
        PAPI_add_event(event_set, PAPI_TOT_INS) != PAPI_OK) {
        std::cerr << "Failed to configure PAPI event set\n";
        return 1;
    }

    // Calibrate: measure the instruction overhead of start/stop itself
    long long warmup = 0, overhead = 0;
    PAPI_start(event_set);
    PAPI_stop(event_set, &warmup); // Resolves PLT/GOT symbols & warms I-cache

    PAPI_start(event_set);
    PAPI_stop(event_set, &overhead); // Measures true steady-state call overhead

    size_t N;
    cin >> N;
    
    // cout << "Start Input\n";
    vector<string> input(N);
    for(size_t i=0; i<N; i++){
        cin >> input[i];
    }
    // cout << "End Input\n";

    // cout << "Start Encode\n";
    // CLK encodeStart = std::chrono::steady_clock::now();
    charInputFormatter encoder(input);
    vector<vector<int>> dictionary = encoder.getDictionary();
    // CLK encodeEnd = std::chrono::steady_clock::now();
    // cout << "End Encode\n";


    // Stage 1: Automaton construction
    AC_Automaton* AC = nullptr;
    // CLK ACStart, ACEnd;
    long long initTime=0, gotoTime=0, failTime=0;
    try {
        // ACStart = std::chrono::steady_clock::now();
        // ACEnd = std::chrono::steady_clock::now();
        PAPI_start(event_set);
        AC = new AC_Automaton(128, dictionary);
        PAPI_stop(event_set, &initTime);
        // initTime = (initTime > overhead) ? (initTime - overhead) : 0;

        PAPI_start(event_set);
        AC->constructGotoPaths();
        PAPI_stop(event_set, &gotoTime);
        // gotoTime = (gotoTime > overhead) ? (gotoTime - overhead) : 0;

        PAPI_start(event_set);
        AC->constructFailurePaths();
        PAPI_stop(event_set, &failTime);
        // failTime = (failTime > overhead) ? (failTime - overhead) : 0;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    }

    cout << "Init Count: " << initTime << endl;
    cout << "goto Count: " << gotoTime << endl;
    cout << "fail Count: " << failTime << endl;

    // std::chrono::microseconds microDiff = std::chrono::duration_cast<std::chrono::microseconds>(encodeEnd - encodeStart);
    // std::chrono::milliseconds milliDiff = std::chrono::duration_cast<std::chrono::milliseconds>(encodeEnd - encodeStart);
    // cout << "Encode: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;

    // microDiff = duration_cast<microseconds>(ACEnd - ACStart);
    // milliDiff = duration_cast<milliseconds>(ACEnd - ACStart);
    // cout << "Automaton Construction: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
    // cout << "Node Count: " << AC -> findNodeCount() << endl;

    int op;
    long long patternTime;

    if (!(cin >> op)) { goto The_End; }
    // Discard the single '\n' left after 'op' ONCE before entering the loop
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while(op > 0)
    {
        op--;
        // cout << "Enter [0:Exit | 1:Input]: ";
        // if (!(cin >> op)) { break; }
        // if(op<0 || op>1){ continue; }
        // if(op == 0){ break; }

        string full_line_input;
        if (!std::getline(std::cin, full_line_input)) { break; } // Reads spaces, tabs, punctuation, etc.

        // Strip trailing '\r' in case input.txt uses Windows CRLF line endings
        if (!full_line_input.empty() && full_line_input.back() == '\r') {
            full_line_input.pop_back();
        }
        vector<int> acIn = encoder.formatInput2(full_line_input);
        vector<vector<int>> actual;

        patternTime = 0;
        try {
            // ACStart = std::chrono::steady_clock::now();
            // ACEnd = std::chrono::steady_clock::now();
            PAPI_start(event_set);
            actual = AC->matchPattern(acIn);
            PAPI_stop(event_set, &patternTime);
            // patternTime = (patternTime > overhead)? (patternTime - overhead) : 0;
        }
        catch (const exception& e) {
            cerr << "Pattern matching failed: "
                << e.what() << '\n';
            delete AC;
            return 1;
        }
        
        // microDiff = duration_cast<microseconds>(ACEnd - ACStart);
        // milliDiff = duration_cast<milliseconds>(ACEnd - ACStart);
        cout << "Pattern Len: " << acIn.size() << "   ";
        // cout << "Pattern Time: " << milliDiff.count() << " ms  |  " << microDiff.count() << " us" << endl;
        cout << "Pattern Ins: " << patternTime << endl;

        cout << "Word: " << full_line_input << endl;
        cout << "Keywords: ";
        for(int i=0; i<actual.size(); i++){
            // cout<<i<<" : ";
            for(int k : actual[i]){ cout <<'<'<<i<<" , "<<encoder.findKeyWord(k)<< "> "; }
            // cout<< endl;
        }
        cout << "\n--------------------------------\n\n";
    }
    
The_End:
    cout << "All Done..." << endl;

    PAPI_cleanup_eventset(event_set);
    PAPI_destroy_eventset(&event_set);
    PAPI_shutdown();
    return 0;
}