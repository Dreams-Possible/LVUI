#ifndef LU_MEM_H
#define LU_MEM_H

#include "lu_type.h"

void* lu_malloc(size_t size);
void* lu_calloc(size_t num, size_t size);
void lu_free(void* ptr);

#endif
