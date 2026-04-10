#include "lu_type.h"
#include <stddef.h>
#include <stdlib.h>

void* lu_malloc(size_t size);
void* lu_calloc(size_t num, size_t size);
void lu_free(void* ptr);

void* lu_malloc(size_t size)
{
    return malloc(size);
}

void* lu_calloc(size_t num, size_t size)
{
    return calloc(num, size);
}

void lu_free(void* ptr)
{
    free(ptr);
}
