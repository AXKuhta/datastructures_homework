#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "simd.h"
#include "tensor_t.h"
#include "ops.h"

// Beware of pointer lt/gt comparison undefined behavior 
static int no_overlap(tensor_t* a, tensor_t* b) {
	return a->storage.memory != b->storage.memory;
}

static int same_size(tensor_t* a, tensor_t* b) {
	return a->storage_size == b->storage_size;
}

static void set_padding(tensor_t* x, f32 value) {
	for (size_t i = x->elements, t = x->storage_size / sizeof(f32); i < t; i++)
		x->storage.f32[i] = value;
}

static void elem_pad(tensor_t* x) { set_padding(x, x->storage.f32[0]); }
static void zero_pad(tensor_t* x) { set_padding(x, 0); }

static void tensor_f32_x_f32(tensor_t* a, tensor_t* b, void fn(f32* a, f32* b, const size_t size)) {
	assert(no_overlap(a, b));
	assert(same_size(a, b));

	fn(a->storage.f32, b->storage.f32, a->storage_size);
}

void tensor_f32_add_f32(tensor_t* a, tensor_t* b) { tensor_f32_x_f32(a, b, f32_add_f32); }
void tensor_f32_sub_f32(tensor_t* a, tensor_t* b) { tensor_f32_x_f32(a, b, f32_sub_f32); }
void tensor_f32_mul_f32(tensor_t* a, tensor_t* b) { tensor_f32_x_f32(a, b, f32_mul_f32); }
void tensor_f32_div_f32(tensor_t* a, tensor_t* b) { tensor_f32_x_f32(a, b, f32_div_f32); }

f32 tensor_min_f32(tensor_t* x) { elem_pad(x); return f32_min(x->storage.f32, x->storage_size); }
f32 tensor_max_f32(tensor_t* x) { elem_pad(x); return f32_max(x->storage.f32, x->storage_size); }
f32 tensor_sum_f32(tensor_t* x) { zero_pad(x); return f32_sum(x->storage.f32, x->storage_size); }

f32 tensor_mean_f32(tensor_t* x) {
	return tensor_sum_f32(x) / (f32)x->elements;
}

f32 tensor_dot_f32(tensor_t* a, tensor_t* b) {
	assert(a->dimensions == 1 && b->dimensions == 1);
	assert(no_overlap(a, b));
	assert(same_size(a, b));

	zero_pad(a);
	zero_pad(b);

	return dot_f32_f32(a->storage.f32, b->storage.f32, a->storage_size);
}
