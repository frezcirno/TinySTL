#pragma once

class swaper {
   private:
    int times;

   public:
    swaper() : times(0){};
    ~swaper(){};
    void operator()(int a, int b) {
        int c = a;
        a = b;
        b = c;
        times++;
    }
    void reset() { times = 0; }
    int times() { return times; }
};

class comparer {
   private:
    int times;

   public:
    comparer() : times(0) {}
    ~comparer() {}
    int operator()(int a, int b) {
        times++;
        return b - a;
    }
    void reset() { times = 0; }
    int times() { return times; }
};