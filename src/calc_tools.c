
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <math.h>
#include "calc_tools.h"

double norm(int r1, int r2)
{
  if (r1 < r2) return 10.0 * r1 / r2;
  else return (double)r1 / r2;
}

int target_index(int target, int* list, int list_len)
{
  int index = 0;
  int bit = list_len >> 1;
  int test;

  while (bit > 0) {
    test = index | bit;
    if (target >= list[test]) index = test;
    bit = bit >> 1;
  }

  return index;
}

// This is basically a binary search
// Except it returns the index of the greatest resistor
// that is less than or equal to <target>
int tgt_idx(int target, int *list, int list_len){
    int left = 0;
    int right = list_len - 1;
    int middle;
    while(left <= right){
        middle = (left + right)/2;
        printf("%d %d %d\n",left,middle,right);
        if((list[middle] <= target) && (list[middle+1]) > target) return middle;
        if(list[middle] < target) left = middle + 1;
        else right = middle - 1;
    }
    return middle;
}