#include <stddef.h>
#include <assert.h>

#include "simd.h"

// K_STRIDE_F32 = 16:
// SSE2 	2 loops (-O2 -march=core2)
// AVX 		4 ops	(-O2 -march=sandybridge)
// AVX2 	2 ops	(-O2 -march=skylake)
// AVX512F 	1 op 	(-O2 -march=icelake-server)
#define K_STRIDE_F32 (K_STRIDE / sizeof(f32))

static int strides_cleanly(const size_t size) {
	return size % K_STRIDE == 0;
}

// A[i] += B[i]
static void elementwise_inplace(f32* restrict a, f32* restrict b, const size_t size, f32 fn(f32 a, f32 b)) {
	assert(strides_cleanly(size));

	size_t elements = size / sizeof(f32);

	for (size_t i = 0; i < elements; i += K_STRIDE_F32) {
		for (size_t j = 0; j < K_STRIDE_F32; j++) {
			a[i + j] = fn(a[i + j], b[i + j]);
		}
	}
}

static f32 add(f32 a, f32 b) { return a + b; }
static f32 sub(f32 a, f32 b) { return a - b; }
static f32 mul(f32 a, f32 b) { return a * b; }
static f32 div(f32 a, f32 b) { return a / b; }

#define flat __attribute__((flatten))

flat void f32_add_f32(f32* a, f32* b, const size_t size) { elementwise_inplace(a, b, size, add); }
flat void f32_sub_f32(f32* a, f32* b, const size_t size) { elementwise_inplace(a, b, size, sub); }
flat void f32_mul_f32(f32* a, f32* b, const size_t size) { elementwise_inplace(a, b, size, mul); }
flat void f32_div_f32(f32* a, f32* b, const size_t size) { elementwise_inplace(a, b, size, div); }

// acc = fn(acc, A[i])
static f32 reduce(f32* restrict x, const size_t size, f32 fn(f32 a, f32 b), f32 acc_init) {
	assert(strides_cleanly(size));

	size_t elements = size / sizeof(f32);
	f32 accs[K_STRIDE_F32];
	f32 acc = acc_init;

	for (size_t i = 0; i < K_STRIDE_F32; i++)
		accs[i] = acc_init;

	for (size_t i = 0; i < elements; i += K_STRIDE_F32) {
		for (size_t j = 0; j < K_STRIDE_F32; j++) {
			accs[j] = fn(x[i + j], accs[j]);
		}
	}

	for (size_t i = 0; i < K_STRIDE_F32; i++)
		acc = fn(accs[i], acc);

	return acc;
}

static f32 min(f32 a, f32 b) { return a < b ? a : b; }
static f32 max(f32 a, f32 b) { return a > b ? a : b; }
static f32 sum(f32 a, f32 b) { return a + b; }

flat f32 f32_min(f32* x, const size_t size) { return reduce(x, size, min, x[0]); }
flat f32 f32_max(f32* x, const size_t size) { return reduce(x, size, max, x[0]); }
flat f32 f32_sum(f32* x, const size_t size) { return reduce(x, size, sum, 0); }

// acc += A[i] * B[i]
f32 dot_f32_f32(f32* restrict a, f32* restrict b, const size_t size) {
	assert(strides_cleanly(size));

	size_t elements = size / sizeof(f32);
	f32 accs[K_STRIDE_F32] = {0.0};
	f32 acc = 0.0;

	for (size_t i = 0; i < elements; i += K_STRIDE_F32) {
		for (size_t j = 0; j < K_STRIDE_F32; j++) {
			accs[j] += a[i + j] * b[i + j];
		}
	}

	for (size_t j = 0; j < K_STRIDE_F32; j++)
		acc += accs[j];

	return acc;
}
