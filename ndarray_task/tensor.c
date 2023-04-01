#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "simd.h"
#include "tensor_t.h"

static void default_stride(tensor_t* tensor) {
	size_t stride = 1;

	for (int i = tensor->dimensions - 1; i >= 0; i--) {
		tensor->shape[i].stride = stride;
		stride *= tensor->shape[i].size;
	}
}

tensor_t* alloc_tensor(size_t shape_dimensions, size_t shape[]) {
	tensor_t* tensor = malloc(sizeof(tensor_t) + sizeof(size_t)*2*shape_dimensions);
	tensor->dimensions = shape_dimensions;
	size_t elements = 1;

	for (size_t i = 0; i < shape_dimensions; i++) {
		assert(shape[i] > 0);
		elements *= shape[i];
		tensor->shape[i].size = shape[i];
	}

	tensor->elements = elements;
	default_stride(tensor);

	return tensor;
}

// ============================================================================
// f32
// ============================================================================

tensor_t* zeros_init_f32_tensor(tensor_t* tensor) {
	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.f32[i] = 0.0;

	return tensor;
}

tensor_t* ones_init_f32_tensor(tensor_t* tensor) {
	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.f32[i] = 1.0;

	return tensor;
}

tensor_t* rand_init_f32_tensor(tensor_t* tensor) {
	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.f32[i] = (double)rand() / (double)RAND_MAX;

	return tensor;
}

tensor_t* array_init_f32_tensor(tensor_t* tensor, size_t array_size, f32 array[]) {
	assert(tensor->elements == array_size);

	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.f32[i] = array[i];

	return tensor;
}

// ============================================================================
// i32
// ============================================================================

tensor_t* zeros_init_i32_tensor(tensor_t* tensor) {
	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.i32[i] = 0;

	return tensor;
}

tensor_t* ones_init_i32_tensor(tensor_t* tensor) {
	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.i32[i] = 1;

	return tensor;
}

tensor_t* rand_init_i32_tensor(tensor_t* tensor) {
	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.i32[i] = rand();

	return tensor;
}

tensor_t* array_init_i32_tensor(tensor_t* tensor, size_t array_size, i32 array[]) {
	assert(tensor->elements == array_size);

	for (size_t i = 0, elements = tensor->elements; i < elements; i++)
		tensor->storage.i32[i] = array[i];

	return tensor;
}

void free_tensor(tensor_t* tensor) {
	free(tensor->storage.memory);
	free(tensor);
}
