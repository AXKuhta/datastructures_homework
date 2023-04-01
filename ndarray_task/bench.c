#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "api.h"

static uint64_t nanoseconds() {
	struct timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	return ts.tv_sec*1000000000ull + ts.tv_nsec;
}

static double bench_inplace_add(size_t trials) {
	tensor_t* a = rand_tensor(f32, 50277);
	tensor_t* b = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_f32_add_f32(a, b);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_inplace_sub(size_t trials) {
	tensor_t* a = rand_tensor(f32, 50277);
	tensor_t* b = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_f32_sub_f32(a, b);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_inplace_mul(size_t trials) {
	tensor_t* a = rand_tensor(f32, 50277);
	tensor_t* b = rand_tensor(f32, 50277);

	// https://stackoverflow.com/questions/53507874/intel-simd-why-is-inplace-multiplication-so-slow
	tensor_t* ones = ones_tensor(f32, 50277);
	tensor_f32_add_f32(b, ones);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_f32_mul_f32(a, b);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_inplace_div(size_t trials) {
	tensor_t* a = rand_tensor(f32, 50277);
	tensor_t* b = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_f32_div_f32(a, b);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_min(size_t trials) {
	tensor_t* x = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_min_f32(x);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_max(size_t trials) {
	tensor_t* x = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_max_f32(x);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_sum(size_t trials) {
	tensor_t* x = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_sum_f32(x);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_mean(size_t trials) {
	tensor_t* x = rand_tensor(f32, 50277);

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		tensor_mean_f32(x);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static double bench_dot(size_t trials) {
	tensor_t* a = rand_tensor(f32, 50277);
	tensor_t* b = rand_tensor(f32, 50277);

	f32 acc = 0.0;

	uint64_t start = nanoseconds();

	for (size_t i = 0; i < trials; i++)
		acc += tensor_dot_f32(a, b);

	uint64_t elapsed = nanoseconds() - start;

	return (double)elapsed / (double)trials;
}

static void run_bench(const char* identifier, double fn(size_t)) {
	printf("%s: %.0lf us/iter\n", identifier, fn(100000)/1000.0);
}

#define bench(X) run_bench(#X, X)

int main() {
	bench(bench_inplace_add);
	bench(bench_inplace_sub);
	bench(bench_inplace_mul);
	bench(bench_inplace_div);
	bench(bench_min);
	bench(bench_max);
	bench(bench_sum);
	bench(bench_mean);
	bench(bench_dot);
}
