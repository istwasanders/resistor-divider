// resistor_divider.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <math.h>
#define STATIC_GETOPT
#include "getopt.h"
#include "standard_resistors.h"
#include "calc_tools.h"
#include "resistor_calculator.h"

void calc(double vi, double vo, int list_num, int optflag, struct res_calc *results, int num_results){
  double ratio;
  int norm_constant;
  double vn;
  int r1, r2;
  int r_target;
  int r_c[2];
  int r_n[2];

  int* resistor_list = stdres_lists[list_num];
  int list_length = stdres_list_lengths[list_num];

  ratio = vi / vo - 1;
  norm_constant = (int)floor(log10(ratio));
  vn = ratio / pow(10, norm_constant);

  r_n[0] = 100;
  r_n[1] = 100;
  printf("%d\n",list_num);
  if((results[0].r1 == 0) && (results[0].r2 == 0)){
    printf("A list of compatible R1s and R2s shall be compiled\n");
  }
  else if(results[0].r1 == 0){
    printf("Only R1 shall be calculated\n");
    float r1_temp = ((vi/vo) - 1)*results[0].r2;
    int something = (int)floor(log10(r1_temp));
    int something_else = target_index((int)(r1_temp/pow(10,something-2)),resistor_list,list_length);
    printf("%.2f, %d, %d\n",r1_temp,something,something_else);
    results[0].r1 = resistor_list[something_else];
    results[0].factor = 0;
  }
  else if(results[0].r2 == 0){
    printf("Only R2 shall be calculated\n");
    r1 = results[0].r1;
    while(r1 >= 1000) r1 = r1 / 10;
    while(r1 < 100) r1 = r1 * 10;
    results[0].r1 = r1;
  }
  else printf("Hey wait a minute\n");
}

struct res_calc calculate(double vi, double vo, int list_num, int optflag)
{
  double ratio;
  int norm_constant;
  double vn;
  int r1, r2;
  int r_target;
  int r_c[2];
  int r_n[2];

  struct res_calc ret;

  int* resistor_list = stdres_lists[list_num];
  int list_length = stdres_list_lengths[list_num];

  ratio = vi / vo - 1;
  norm_constant = (int)floor(log10(ratio));
  vn = ratio / pow(10, norm_constant);
  r_n[0] = 100;
  r_n[1] = 100;
  for (int i = 0; i < list_length; i++)
  {
    r2 = resistor_list[i];
    r1 = (int)(r2 * vn);
    if (r1 >= 1000) r1 = r1 / 10;
    r_target = target_index(r1, resistor_list, list_length);
    r_c[0] = resistor_list[r_target];
    r_c[1] = resistor_list[(r_target + 1) % list_length];
    if(optflag & (1 << 3)){
      printf("(%d, %d), %d\n",r_c[0],r_c[1],r2);
    }
    if (fabs(norm(r_c[0], r2) - vn) < fabs(norm(r_n[0], r_n[1]) - vn)) {
      r_n[0] = r_c[0];
      r_n[1] = r2;
    }
    if (fabs(norm(r_c[1], r2) - vn) < fabs(norm(r_n[0], r_n[1]) - vn)) {
      r_n[0] = r_c[1];
      r_n[1] = r2;
    }
  }
  ret.r1 = r_n[0];
  ret.r2 = r_n[1];
  ret.factor = (int)norm_constant;
  return ret;
}

void print_usage()
{
  printf("Usage: .\\resistor_divider.exe -i vin -o vout [-s series] [-r resistors] [-v]\n");
  printf("    -i vin: Input voltage to the divider\n");
  printf("    -o vout: Output voltage from the divider\n");
  printf("    -s series: Resistor series (E6, E12, E24, E48, E96, E192, or custom, default E96)\n");
  printf("    -r resistors: CSV file with list of custom resistances (NOT IMPLEMENTED)\n");
  printf("    -v verbose: print all candidate dividers\n");
}

int main(int argc, char *argv[])
{
  struct res_calc c;
  int correction;
  int o;
  double vin = 0;
  double vout = 0;
  char* series = "E96";

  int optflag = 0;

  while ((o = getopt(argc, argv, "vi:o:s:r:t:b:")) != -1)
    switch (o)
    {
      case 'i':
        vin = atof(optarg);
        optflag |= (1 << 0);
        break;
      case 'o':
        vout = atof(optarg);
        optflag |= (1 << 1);
        break;
      case 's':
        series = optarg;
        optflag |= (1 << 2);
        break;
      case 't':
        c.r1 = atoi(optarg);
        c.r2 = 0;
        break;
      case 'b':
        c.r2 = atoi(optarg);
        c.r1 = 0;
        break;
      case 'r':
        break;
      case 'v':
        optflag |= (1 << 3);
        break;
      case '?':
        print_usage();
        return 1;
      default:
        return 2;
    }
  int series_index = 0;
  double error;
  double out_actual;

  if ((optflag & 3) != 3) {
    print_usage();
    return 3;
  }

  while (strcmp(series, stdres_series_names[series_index])) {
    series_index++;
    if(series_index > 4) break;
  }
  if (vin/vout < 1) {
    printf("Invalid vout to vin ratio\n");
    return -1;
  }

  calc(vin,vout,series_index,optflag,&c,1);
  printf("%d\n%d\n",c.r1,c.r2);
  return 0;
  c = calculate(vin, vout, series_index, optflag);
  if (c.r1 < c.r2) c.factor = c.factor + 1;
  correction = (int)pow(10.0, abs(c.factor));
  if (c.factor < 0) c.r2 = c.r2 * correction;
  else c.r1 = c.r1 * correction;
  out_actual = (vin * c.r2) / ((c.r1 + c.r2));
  error = (vin * c.r2) / ((c.r1 + c.r2) * vout) - 1;
  printf("Vin: %.5g\n", vin);
  printf("Vout: %.5g\n", vout);
  printf("Series: %s\n", stdres_series_names[series_index]);
  printf("R1: %d\n", c.r1);
  printf("R2: %d\n", c.r2);
  printf("Actual Output: %.7g\n", out_actual);
  printf("Error: %.5g%%\n", error*100);
  return 0;
}
