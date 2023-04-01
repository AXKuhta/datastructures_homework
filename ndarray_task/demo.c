#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>

#include "api.h"

void tensor_initialization() {
	printf("===============================================\n");
	printf("Zeros/ones/random initialization\n");
	printf("===============================================\n");

	tensor_t* a = zeros_tensor(f32, 4, 4);
	tensor_t* b = ones_tensor(f32, 4, 4);
	tensor_t* c = rand_tensor(f32, 4, 4);

	printf("Zeros tensor:\n");
	print_tensor(a);

	printf("\nOnes tensor:\n");
	print_tensor(b);

	printf("\nRand tensor:\n");
	print_tensor(c);

	printf("\n\n");

	free_tensor(a);
	free_tensor(b);
	free_tensor(c);
}

void inplace_math() {
	printf("===============================================\n");
	printf("Inplace math\n");
	printf("===============================================\n");

	tensor_t* a = rand_tensor(f32, 4, 4);
	tensor_t* b = rand_tensor(f32, 4, 4);
	tensor_t* c = ones_tensor(f32, 4, 4);

	printf("Inplace add:\n");
	print_tensor(a);
	printf("+\n");
	print_tensor(c);
	tensor_f32_add_f32(a, c);
	printf("=\n");
	print_tensor(a);
	printf("\n");

	printf("Inplace sub:\n");
	print_tensor(a);
	printf("-\n");
	print_tensor(b);
	tensor_f32_sub_f32(a, b);
	printf("=\n");
	print_tensor(a);
	printf("\n");

	printf("Inplace mul:\n");
	print_tensor(a);
	printf("*\n");
	print_tensor(b);
	tensor_f32_mul_f32(a, b);
	printf("=\n");
	print_tensor(a);
	printf("\n");

	printf("Inplace div:\n");
	print_tensor(c);
	printf("/\n");
	print_tensor(a);
	tensor_f32_div_f32(c, a);
	printf("=\n");
	print_tensor(c);
	printf("\n");
}

void reduction_ops() {
	printf("===============================================\n");
	printf("Reduction operations: min/max/mean\n");
	printf("===============================================\n");
	
	tensor_t* a = rand_tensor(f32, 4, 4);
	print_tensor(a);

	printf("Min: %.4f\n", tensor_min_f32(a));
	printf("Max: %.4f\n", tensor_max_f32(a));
	printf("Mean: %.4f\n", tensor_mean_f32(a));
}

int main() {
	tensor_initialization();
	inplace_math();
	reduction_ops();
}
