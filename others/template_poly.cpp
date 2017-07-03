#include <iostream>

template <typename T, typename R> auto map(T array[], long numel, R (*f)(T)) {
	auto result = new R[numel];
	for (long i = 0; i < numel; i++) {
		result[i] = f(array[i]);
	}

	return result;
}

template <typename T> auto poly_incr(T x) { return x + 1; }

int main() {
	long numel = 5;
	long array[] = {1, 2, 3, 4, 5};

	auto res = map(array, numel, poly_incr<decltype(poly_incr(array[0]))>);
	for (int a = 0; a < numel; a++) {
		std::cout << "res[" << a << "]: " << res[a] << std::endl;
	}
}
