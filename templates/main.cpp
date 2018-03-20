#undef NDEBUG
#define AIM
void print_stats(long long delta_time);
extern "C" int usleep(unsigned int usec);
#include "E.h"

#include <malloc.h>

void print_stats(long long delta_time) {
    cerr << "\n time: " << delta_time / 1.0 / CLOCKS_PER_SEC;

    extern char *__progname;
    system((string("size ") + __progname + " > /tmp/size.txt").c_str());
    ifstream in("/tmp/size.txt");
    string s;

    getline(in, s);
    int sz = 0;
    if (in >> sz >> sz >> sz >> sz) {
        cerr << ", static memory: " << sz / (double)(1024 * 1024) << " MB";
    }

    cerr << endl << endl;
}

//-D_GLIBCXX_DEBUG
