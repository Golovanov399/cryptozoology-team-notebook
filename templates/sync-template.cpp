#include <bits/stdc++.h>

// Arguments: $FileName$ $FileDir$/main.cpp
// Working directory: $ProjectFileDir$
// ! Synchronize files after execution
// ! Open console for tool output

using namespace std;

int main(int argc, char** argv) {
    freopen(argv[2], "w", stdout);
    string s;
    while (getline(cin, s)) {
        if (s.length() >= 2 && s.substr(0, 2) == "#i")
            s = "idiot " + string(argv[1]) + "\"";
            
        cout << s << "\n";
    }
    
    return 0;
}
