#ifndef _CALC_TOOLS_H_
#define _CALC_TOOLS_H_

#include "resistor_calculator.h"

double norm(int r1, int r2);
int target_index(int target, int* list, int list_len);
void resistor_percolate(struct res_calc *divider_list, int list_len);
void div_output_error(double vi, double vo, struct res_calc *divider);

#endif