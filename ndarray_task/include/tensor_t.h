
// ============================================================================
// SHAPING + INITIALIZERS
// ============================================================================

// Shape(4, 16) => 2, (size_t[]){4, 16}
#define Array(datatype, ...) (sizeof((datatype[]){__VA_ARGS__}) / sizeof(datatype)), ((datatype[]){__VA_ARGS__})
#define Shape(...) Array(size_t, __VA_ARGS__)
#define Index(...) Array(size_t, __VA_ARGS__)

#define Array_f32(...) Array(f32, __VA_ARGS__)
#define Array_i32(...) Array(i32, __VA_ARGS__)

// ============================================================================
// TENSOR STRUCTURE
// ============================================================================

typedef struct tensor_t {
	union {
		void* memory;
		f32* f32;
		i32* i32;
	} storage;

	size_t storage_size;
	size_t elements;
	size_t dimensions;

	struct {
		size_t size;
		size_t stride;
	} shape[];
} tensor_t;

// ============================================================================
// TENSOR CREATION
// ============================================================================

static size_t apply_padding(size_t size) {
	return size + (-size % K_STRIDE);
}

// Size annotations for compiler to warn you if you go out of bounds
// At least on static indices
static __attribute__((unused)) tensor_t* alloc_storage(size_t element_size, tensor_t* tensor) {
	size_t base_size = element_size * tensor->elements;
	size_t padded_size = apply_padding(base_size);

	tensor->storage_size = padded_size;
	tensor->storage.memory = malloc(padded_size);

	return tensor;
}

#define zeros_tensor(datatype, ...) zeros_init_##datatype##_tensor(alloc_storage(sizeof(datatype), alloc_tensor(Shape(__VA_ARGS__))))
#define ones_tensor(datatype, ...) ones_init_##datatype##_tensor(alloc_storage(sizeof(datatype), alloc_tensor(Shape(__VA_ARGS__))))
#define rand_tensor(datatype, ...) rand_init_##datatype##_tensor(alloc_storage(sizeof(datatype), alloc_tensor(Shape(__VA_ARGS__))))
#define _array_tensor(datatype, shape_size, shape, array_size, array) array_init_##datatype##_tensor(alloc_storage(sizeof(datatype), alloc_tensor(shape_size, shape)), array_size, array)
#define array_tensor(...) _array_tensor(__VA_ARGS__)

// ============================================================================
// PROTOTYPES
// ============================================================================

tensor_t* alloc_tensor(size_t shape_dimensions, size_t shape[]);

tensor_t* zeros_init_f32_tensor(tensor_t* tensor);
tensor_t* ones_init_f32_tensor(tensor_t* tensor);
tensor_t* rand_init_f32_tensor(tensor_t* tensor);
tensor_t* array_init_f32_tensor(tensor_t* tensor, size_t array_size, f32 array[]);

tensor_t* zeros_init_i32_tensor(tensor_t* tensor);
tensor_t* ones_init_i32_tensor(tensor_t* tensor);
tensor_t* rand_init_i32_tensor(tensor_t* tensor);
tensor_t* array_init_i32_tensor(tensor_t* tensor, size_t array_size, i32 array[]);

void free_tensor(tensor_t* tensor);

// ============================================================================
// INDEXING
// ============================================================================

static __attribute__((unused)) size_t linear_index(const tensor_t* tensor, size_t index_dimensions, size_t index[]) {
	assert(tensor->dimensions >= index_dimensions);

	size_t offset = tensor->dimensions - index_dimensions;
	size_t linear_index = 0;

	for (size_t i = 0; i < index_dimensions; i++) {
		linear_index += index[i] * tensor->shape[i + offset].stride;
	}

	return linear_index;
}

#define rw(datatype, x, ...) *(x->storage.datatype + linear_index(x, Index(__VA_ARGS__)))
