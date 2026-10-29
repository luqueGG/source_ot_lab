#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <ranges>


bool es_primo(int n) 
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; i * i <= n; i += 2) 
    {
        if (n % i == 0) return false;
    }
    return true;
}

int main() 
{
    constexpr std::size_t N = 100;

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(1, 5000);

    std::vector<int> numeros(N);
    std::ranges::generate(numeros, [&] { return dist(gen); });

    auto cantidad = std::count_if(numeros.begin(), numeros.end(),
        [](int n) { return n < 1000 && es_primo(n); });

    std::cout << "Vector: ";
    for (int n : numeros) std::cout << n << ' ';
    std::cout << "\n\nPrimos menores que 1000: " << cantidad << '\n';
    return 0;
}
