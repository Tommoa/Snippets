#include <iostream>

template <typename T, typename R> auto map (T array[], long numel, R(*f)(T)) {
    auto result = new R[numel];
    std::cout << "Size of input array: " << numel << std::endl;
    for (long i = 0; i < numel; i++) {
        result[i] = f(array[i]);
        std::cout << result[i] << std::endl;
    }

    return result;
}

template <typename T> auto poly_incr(T x) {
    return x+1;
}

int main() {
    long numel = 5;
    long array[] = { 1, 2, 3, 4, 5 };

    auto res = map<long, typeof(poly_incr<typeof(array[0])>(array[0]))>(array, numel, poly_incr);
    for (int a = 0; a < numel; a++) {
        std::cout << "res[" << a << "]: " << res[a] << std::endl;
    }
}
