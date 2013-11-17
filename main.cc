#include <iostream>
#include "Time.hh"

using namespace std;

int main() {
    int interval = 17;
    long long int current;
    long long int prevFrame = Time::msec();
    long long int diff;
    while (true) {
        current = Time::msec();
        diff = current - prevFrame;
        if (diff >= interval) {
            cout << current << endl;
            prevFrame = current - (diff - interval);
        }
    }
    return 0;
}
