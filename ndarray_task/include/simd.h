
typedef float f32;
typedef int i32;

void f32_add_f32(f32* a, f32* b, const size_t size);
void f32_sub_f32(f32* a, f32* b, const size_t size);
void f32_mul_f32(f32* a, f32* b, const size_t size);
void f32_div_f32(f32* a, f32* b, const size_t size);

f32 f32_min(f32* x, const size_t size);
f32 f32_max(f32* x, const size_t size);
f32 f32_sum(f32* x, const size_t size);

f32 dot_f32_f32(f32* restrict a, f32* restrict b, const size_t size);

#define K_STRIDE 64
