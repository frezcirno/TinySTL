#include <ctime>
#include <iostream>

#include "Queue.hpp"
#include "Stack.hpp"
#include "Vector.hpp"

#include <stack>
#include <vector>

using namespace std;

template <typename T, class C1, class C2>
void benchmark(T start, T end) {
    clock_t start_t, end_t;
    start_t = clock();
    C1 v;
    for (T i = start; i < end; i++) {
        v.insert(v.begin(), i);
    }
    end_t = clock();
    cout << "C1 " << end_t - start_t << endl;

    start_t = clock();
    C2 V;
    for (T i = start; i < end; i++) {
        V.insert(V.begin(), i);
    }
    end_t = clock();
    cout << "C2 " << end_t - start_t << endl;
}

template <typename T, class C1, class C2>
void benchmark(T value) {
    clock_t start_t, end_t;
    start_t = clock();
    C1 v;
    for (int i = 0; i < 10000; i++) {
        v.insert(v.begin(), value);
    }
    end_t = clock();
    cout << "C1 " << end_t - start_t << endl;

    start_t = clock();
    C2 V;
    for (int i = 0; i < 10000; i++) {
        V.insert(V.begin(), value);
    }
    end_t = clock();
    cout << "C2 " << end_t - start_t << endl;
}

int main() {
    benchmark<double, vector<double>, tinySTL::Vector<double>>(10000, 20000);
    benchmark<int, vector<int>, tinySTL::Vector<int>>(10000, 20000);
    benchmark<char, vector<char>, tinySTL::Vector<char>>(1, 127);
    benchmark<string, vector<string>, tinySTL::Vector<string>>("abcabcabcabc");

    return 0;
}
