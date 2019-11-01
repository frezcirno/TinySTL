#pragma once

class swaper {
   private:
    int m_times;

   public:
    swaper() : m_times(0){};
    ~swaper(){};
    void operator()(int& a, int& b) {
        int c = a;
        a = b;
        b = c;
        m_times++;
    }
    void reset() { m_times = 0; }
    int times() const { return m_times; }
};

class comparer {
   private:
    int m_times;

   public:
    comparer() : m_times(0) {}
    ~comparer() {}
    int operator()(int a, int b) {
        m_times++;
        return (a - b);
    }
    void reset() { m_times = 0; }
    int times() const { return m_times; }
};
