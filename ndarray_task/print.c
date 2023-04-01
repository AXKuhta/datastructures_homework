#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "simd.h"
#include "tensor_t.h"
#include "print.h"

#define H_ELEMENT_MAX 6

static void f32_print_fn(const tensor_t* tensor, size_t index) {
	printf("%.4f", tensor->storage.f32[index]);
}

static void i32_print_fn(const tensor_t* tensor, size_t index) {
	printf("%d", tensor->storage.i32[index]);
}

static void print_generic(const tensor_t* tensor, void print_fn(const tensor_t* tensor, size_t index)) {
	const size_t dimensions = tensor->dimensions;

	printf("tensor(Shape(");

	for (size_t i = 0; i < dimensions; i++)
		printf("%zu%s", tensor->shape[i].size, dimensions - i > 1 ? ", " : "");

	printf("), \"%s\")\n", "f32");

	size_t* counters_reload = malloc(dimensions*sizeof(size_t));
	size_t* counters = malloc(dimensions*sizeof(size_t));
	size_t elements = 1;

	for (int i = dimensions - 1; i >= 0; i--) {
		elements *= tensor->shape[i].size;
		counters_reload[i] = elements;
		counters[i] = 0;
	}

	char* level_enter = malloc(dimensions + 1);
	char* level_leave = malloc(dimensions + 1);
	char* level_pad = malloc(dimensions + 1);

	memset(level_enter, '[', dimensions);
	memset(level_leave, ']', dimensions);
	memset(level_pad, ' ', dimensions);

	level_enter[dimensions] = 0;
	level_leave[dimensions] = 0;
	level_pad[dimensions] = 0;

	size_t h_elements = 0;

	for (size_t i = 0; i < elements; i++) {
		size_t braces_open = 0;
		size_t braces_close = 0;

		for (size_t j = 0; j < dimensions; j++) {
			braces_open += counters[j] == 0;
			braces_close += 1+counters[j] == counters_reload[j];
			counters[j] = 1+counters[j] == counters_reload[j] ? 0 : counters[j] + 1;
		}

		h_elements++;

		if (h_elements >= H_ELEMENT_MAX) {
			if (h_elements == H_ELEMENT_MAX)
				printf("..., ");

			if (braces_close == 0)
				continue;
		}

		const char* pad = 1 == h_elements ? level_pad + braces_open : "";
		const char* prefix = level_enter + dimensions - braces_open;
		const char* separator = elements - i > 1 ? ", " : "";
		const char* postfix = level_leave + dimensions - braces_close;

		printf("%s%s", pad, prefix);
		print_fn(tensor, i);
		printf("%s%s", postfix, separator);

		if (braces_close > 0) {
			h_elements = 0;
			printf("\n");
		}
	}

	free(level_enter);
	free(level_leave);
	free(level_pad);
	free(counters);
	free(counters_reload);
}

void print_tensor(const tensor_t* tensor) {
	print_generic(tensor, f32_print_fn);
}
