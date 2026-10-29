#include <iostream>
using namespace std;[cite: 1]

template <class T = char, int N = 8>[cite: 1]
class Comun1 {[cite: 1]
    T bloque[N];[cite: 1]
public:[cite: 1]
    void set(int num, T tval);[cite: 1]
    T get(int num);[cite: 1]
};[cite: 1]

template <class T, int N>[cite: 1]
void Comun1<T, N>::set(int num, T tval) {[cite: 1]
    bloque[num] = tval;[cite: 1]
}[cite: 1]

template <class T, int N>[cite: 1]
T Comun1<T, N>::get(int num) {[cite: 1]
    return bloque[num];[cite: 1]
}[cite: 1]

int main() {[cite: 1]
    Comun1 <int, 5> objInt;[cite: 1]
    Comun1 <double, 5> objFloat;[cite: 1]
    Comun1 < > obj;[cite: 1]
    objInt.set(0, 10);[cite: 1]
    objFloat.set(2, 3.1);[cite: 1]

    std::cout << objInt.get(0) << std::endl;[cite: 1]
    std::cout << objFloat.get(2) << std::endl;[cite: 1]
    std::cout << obj.get(4) << std::endl;[cite: 1]

    return 0;[cite: 1]
}[cite: 1]
