#include "lvui/plat/lu_type.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"

typedef struct lu_sll_node_t
{
    const void* data;
    struct lu_sll_node_t* next;
} lu_sll_node_t;

typedef struct lu_sll_t
{
    int32_t size;
    lu_sll_node_t *meta;
} lu_sll_t;

lu_sll_t* lu_sll_create();
void lu_sll_delete(lu_sll_t* *sll);
void* lu_sll_append(lu_sll_t* sll, const void* data);
void* lu_sll_insert(lu_sll_t* sll, int32_t index, const void* data);
void lu_sll_replace(lu_sll_t* sll, int32_t index, const void* data);
void lu_sll_remove(lu_sll_t* sll, int32_t index);
void* lu_sll_get_data(const lu_sll_node_t* ll_node);
lu_sll_node_t* lu_sll_get_head(const lu_sll_t* sll);
lu_sll_node_t* lu_sll_get_tail(const lu_sll_t* sll);
lu_sll_node_t* lu_sll_get_next(const lu_sll_node_t* ll_node);
int32_t lu_sll_get_index(const lu_sll_t* sll, const lu_sll_node_t* ll_node);
int32_t lu_sll_get_len(const lu_sll_t* sll);
bool lu_sll_is_empty(const lu_sll_t* sll);

lu_sll_t* lu_sll_create()
{
    lu_sll_t* sll=(lu_sll_t*)lu_calloc(1,sizeof(lu_sll_t));
    if(!sll)
    {
        return NULL;
    }
    sll->meta=NULL;
    sll->size=0;
    lu_sll_node_t* node=(lu_sll_node_t*)lu_calloc(1,sizeof(lu_sll_node_t));
    if(!node)
    {
        lu_free(sll);
        return NULL;
    }
    node->data=NULL;
    node->next=NULL;
    sll->meta=node;
    return sll;
}

void lu_sll_delete(lu_sll_t* *sll)
{
    if(!sll)
    {
        return;
    }
    if(!(*sll))
    {
        return;
    }
    lu_sll_node_t* cur_node=(*sll)->meta;
    while(cur_node->next)
    {
        lu_sll_node_t* next_node=cur_node->next->next;
        lu_sll_node_t* tg_node=next_node->next;
        if(tg_node->data)
        {
            lu_free((void*)tg_node->data);
        }
        lu_free(tg_node);
        cur_node=next_node;
    }
    lu_free((*sll)->meta);
    lu_free(*sll);
    *sll=NULL;
}

void* lu_sll_append(lu_sll_t* sll, const void* data)
{
    if(!sll)
    {
        return NULL;
    }
    lu_sll_node_t* new_node=(lu_sll_node_t*)lu_calloc(1,sizeof(lu_sll_node_t));
    if(!new_node)
    {
        return NULL;
    }
    new_node->data=data;
    new_node->next=NULL;
    lu_sll_node_t* cur_node=sll->meta;
    while(cur_node->next)
    {
        cur_node=cur_node->next;
    }
    cur_node->next=new_node;
    ++sll->size;
    return new_node;
}

void* lu_sll_insert(lu_sll_t* sll, int32_t index, const void* data)
{
    if(!sll)
    {
        return NULL;
    }
    if(index>=sll->size)
    {
        return NULL;
    }
    lu_sll_node_t* new_node=(lu_sll_node_t*)lu_calloc(1,sizeof(lu_sll_node_t));
    if(!new_node)
    {
        return NULL;
    }
    new_node->data=data;
    new_node->next=NULL;
    int32_t i=0;
    lu_sll_node_t* cur_node=sll->meta;
    while(i<index)
    {
        cur_node=cur_node->next;
        ++i;
    }
    new_node->next=cur_node->next;
    cur_node->next=new_node;
    ++sll->size;
    return new_node;
}

void lu_sll_replace(lu_sll_t* sll, int32_t index, const void* data)
{
    if(!sll)
    {
        return;
    }
    if(index>=sll->size)
    {
        return;
    }
    int32_t i=0;
    lu_sll_node_t* cur_node=sll->meta;
    while(i<index)
    {
        cur_node=cur_node->next;
        ++i;
    }
    if(cur_node->next->data)
    {
        lu_free((void*)cur_node->next->data);
    }
    cur_node->next->data=data;
}

void lu_sll_remove(lu_sll_t* sll, int32_t index)
{
    if(!sll)
    {
        return;
    }
    if(index>=sll->size)
    {
        return;
    }
    int32_t i=0;
    lu_sll_node_t* cur_node=sll->meta;
    while(i<index)
    {
        cur_node=cur_node->next;
        ++i;
    }
    lu_sll_node_t* tg_node=cur_node->next;
    cur_node->next=cur_node->next->next;
    if(tg_node->data)
    {
        lu_free((void*)tg_node->data);
    }
    lu_free(tg_node);
    --sll->size;
}

void* lu_sll_get_data(const lu_sll_node_t* ll_node)
{
    if(!ll_node)
    {
        return NULL;
    }
    return (void*)ll_node->data;
}

lu_sll_node_t* lu_sll_get_head(const lu_sll_t* sll)
{
    if(!sll)
    {
        return NULL;
    }
    return sll->meta->next;
}

lu_sll_node_t* lu_sll_get_tail(const lu_sll_t* sll)
{
    if(!sll)
    {
        return NULL;
    }
    lu_sll_node_t* cur_node=sll->meta;
    while(cur_node->next)
    {
        cur_node=cur_node->next;
    }
    return cur_node;
}

lu_sll_node_t* lu_sll_get_next(const lu_sll_node_t* ll_node)
{
    if(!ll_node)
    {
        return NULL;
    }
    return ll_node->next;
}

int32_t lu_sll_get_index(const lu_sll_t* sll, const lu_sll_node_t* ll_node)
{
    if(!sll)
    {
        return -1;
    }
    if(!ll_node)
    {
        return -1;
    }
    int32_t i=0;
    lu_sll_node_t* cur_node=sll->meta;
    while(cur_node->next!=ll_node)
    {
        if(!cur_node->next)
        {
            return -1;
        }
        cur_node=cur_node->next;
        ++i;
    }
    return i;
}

int32_t lu_sll_get_len(const lu_sll_t* sll)
{
    if(!sll)
    {
        return 0;
    }
    return sll->size;
}

bool lu_sll_is_empty(const lu_sll_t* sll)
{
    if(!sll)
    {
        return true;
    }
    return sll->size==0;
}
