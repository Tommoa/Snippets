#include <iostream>
#include <vector>
#include <functional>

#define poly_map(v, f) map(v, f<decltype(v)::value_type>)

template <typename T, typename F, typename R = typename std::result_of<F&&(T&&)>::type> auto map(std::vector<T> arr, F&& f) -> decltype(std::vector<R>()) {
    std::vector<R> out(arr.size());
    for (long i = 0; i < arr.size(); ++i) {
            out[i] = f(arr[i]);
    }
    return out;
}

template <typename T> auto poly_incr(T x) -> decltype(x+1) { return x + 1; }
int long_incr(long x) { return ++x; }

template <typename T> void print_array(std::vector<T> arr) {
    std::cout << "{ ";
    for (auto t : arr)
        std::cout << t << " ";
    std::cout << "}" << std::endl;
}

struct Incr {
    long incr(long x) { return ++x; };
};

int main() {
    std::vector<long> array = {1, 2, 3, 4, 5};
    Incr i;
    print_array(array);

    print_array(map(array, std::bind(&Function::incr, &i, std::placeholders::_1)));

    print_array(poly_map(array, poly_incr));

    print_array(map(array, long_incr));
}
