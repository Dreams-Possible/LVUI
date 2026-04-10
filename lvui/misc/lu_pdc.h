#ifndef LU_PDC_H
#define LU_PDC_H

#include "lvui/plat/lu_type.h"

typedef struct lu_pdc_t lu_pdc_t;

lu_pdc_t* lu_pdc_create();
void lu_pdc_delete(lu_pdc_t**pdc);
void lu_pdc_set_target(lu_pdc_t* pdc, float target);
float lu_pdc_get_target(lu_pdc_t* pdc);
void lu_pdc_set_kp(lu_pdc_t* pdc, float kp);
float lu_pdc_get_kp(lu_pdc_t* pdc);
void lu_pdc_set_kd(lu_pdc_t* pdc, float kd);
float lu_pdc_get_kd(lu_pdc_t* pdc);
void lu_pdc_set_limit(lu_pdc_t* pdc, float min, float max);
void lu_pdc_get_limit(lu_pdc_t* pdc, float* min, float* max);
void lu_pdc_reset(lu_pdc_t* pdc);
float lu_pdc_comput(lu_pdc_t* pdc, float input, float time);

#endif
