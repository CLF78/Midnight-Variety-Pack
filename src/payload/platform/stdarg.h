#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char gpr;
	char fpr;
	char reserved[2];
	char* input_arg_area;
	char* reg_save_area;
} va_list;

#define va_start(ap,fmt) ((void)(fmt),__builtin_va_info(&(ap)))
#define va_end(ap) ((void)0)

#ifdef __cplusplus
}
#endif
