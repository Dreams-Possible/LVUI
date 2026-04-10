#ifndef LU_SLL_H
#define LU_SLL_H

#include "lvui/plat/lu_type.h"

typedef struct lu_sll_node_t lu_sll_node_t;
typedef struct lu_sll_t lu_sll_t;

lu_sll_t* lu_sll_create();
void lu_sll_delete(lu_sll_t **sll);
void* lu_sll_append(lu_sll_t *sll, const void* data);
void* lu_sll_insert(lu_sll_t *sll, int32_t index, const void* data);
void lu_sll_replace(lu_sll_t *sll, int32_t index, const void* data);
void lu_sll_remove(lu_sll_t *sll, int32_t index);
void* lu_sll_get_data(const lu_sll_node_t* ll_node);
lu_sll_node_t* lu_sll_get_head(const lu_sll_t *sll);
lu_sll_node_t* lu_sll_get_tail(const lu_sll_t *sll);
lu_sll_node_t* lu_sll_get_next(const lu_sll_node_t* ll_node);
int32_t lu_sll_get_index(const lu_sll_t *sll, const lu_sll_node_t* ll_node);
int32_t lu_sll_get_len(const lu_sll_t *sll);
bool lu_sll_is_empty(const lu_sll_t *sll);

#endif
