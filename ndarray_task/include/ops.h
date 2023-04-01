
void tensor_f32_add_f32(tensor_t* a, tensor_t* b);
void tensor_f32_sub_f32(tensor_t* a, tensor_t* b);
void tensor_f32_mul_f32(tensor_t* a, tensor_t* b);
void tensor_f32_div_f32(tensor_t* a, tensor_t* b);

f32 tensor_min_f32(tensor_t* x);
f32 tensor_max_f32(tensor_t* x);
f32 tensor_sum_f32(tensor_t* x);

f32 tensor_mean_f32(tensor_t* x);
f32 tensor_dot_f32(tensor_t* a, tensor_t* b);
