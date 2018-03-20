#include <bits/stdc++.h>
using namespace std;

#define all(a) a.begin(), a.end()
typedef long long li;
typedef long double ld;
void solve(__attribute__((unused)) bool);
void precalc();
clock_t start;

int main() {
#ifdef CRYPTO 
    freopen("/PATH/input.txt", "r", stdin);
#endif
    start = clock();
    int t = 1;
#ifndef CRYPTO
    cout.sync_with_stdio(0);
    cin.tie(0);
#endif
    precalc();
    cout.precision(20);
    cout << fixed;
//    cin >> t;
    int testNum = 1;
    while (t--) {
        //cout << "Case #" << testNum++ << ": ";
        solve(true);
    }
    cout.flush();
#ifdef CRYPTO1
    while (true) {
      solve(false);
  }
#endif

#ifdef CRYPTO
    cout.flush();
    auto end = clock();

    usleep(10000);
    print_stats(end - start);
    usleep(10000);
#endif

    return 0;
}

void precalc() {
}

template<typename T>
inline T nxt() {
    T cur;
    cin >> cur;
    return cur;
}

//#define int li
//const int mod = 1000000007;

void solve(__attribute__((unused)) bool read) {

}
