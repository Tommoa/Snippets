#include <benchmark/benchmark.h>

static void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }

static void clobber() { asm volatile("" : : : "memory"); }

// NOTE: Almost all of these functions use 4 instructions and 1 register.
// 		If you do i++ instead of ++i, you use an additional register (and
// sometimes
// 		instruction) to store and return the result

// Clang: 	4 instructions, 1 register
// GCC: 	5 instructions, 1 register
int static_signed_int() {
	static int i = 0;
	return ++i;
}
static void bench_static_signed_int(benchmark::State& state) {
	int a;
	while (state.KeepRunning()) {
		a = static_signed_int();
		escape(&a);
	}
}
BENCHMARK(bench_static_signed_int);

// Clang: 	4 instructions, 1 register
// GCC: 	4 instructions, 1 register
unsigned int static_unsigned_int() {
	static unsigned int i = 0;
	return ++i;
}
static void bench_static_unsigned_int(benchmark::State& state) {
	int a;
	while (state.KeepRunning()) {
		a = static_unsigned_int();
		escape(&a);
	}
}
BENCHMARK(bench_static_unsigned_int);

// Clang: 	4 instructions, 1 register
// GCC: 	4 instructions, 1 register
long static_signed_long() {
	static long i = 0;
	return ++i;
}
static void bench_static_signed_long(benchmark::State& state) {
	int a;
	while (state.KeepRunning()) {
		a = static_unsigned_int();
		escape(&a);
	}
}
BENCHMARK(bench_static_signed_long);

// Clang: 	4 instructions, 1 register
// GCC: 	4 instructions, 1 register
unsigned long static_unsigned_long() {
	static unsigned long i = 0;
	return ++i;
}
static void bench_static_unsigned_long(benchmark::State& state) {
	int a;
	while (state.KeepRunning()) {
		a = static_unsigned_long();
		escape(&a);
	}
}
BENCHMARK(bench_static_unsigned_long);

BENCHMARK_MAIN();
