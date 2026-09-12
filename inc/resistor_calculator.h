#ifndef _RESISTOR_CALCULATOR_H_
#define _RESISTOR_CALCULATOR_H_

struct res_calc {
  int r1;
  int r2;
  int factor;
  double rnorm;
  double out;
  double error;
};

enum res_calc_optflags{
  RES_OPT_INPUT = 0,
  RES_OPT_OUTPUT,
  RES_OPT_SERIES,
  RES_OPT_TOP,
  RES_OPT_BOTTOM,
  RES_OPT_GREATER,
  RES_OPT_LESS,
  RES_OPT_VERBOSE
};

#endif