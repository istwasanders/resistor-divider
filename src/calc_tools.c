#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <math.h>
#include "calc_tools.h"
#include "resistor_calculator.h"

// This is a "normalization" function
// It returns the ratio of two resistances but between 1 and 10
// Its arguments will always be between 100 and 999
double norm(int r1, int r2)
{
  if (r1 < r2) return 10.0 * r1 / r2;
  else return (double)r1 / r2;
}

// This is basically a binary search
// Except it returns the index of the greatest resistor
// that is less than or equal to <target>
int target_index(int target, int *list, int list_len){
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

// Percolation function, puts the resistor pair under evaluation into
// its proper position in the list
void resistor_percolate(struct res_calc *divider_list, int list_len){
    struct res_calc temp;
    list_len--;
    while((list_len > 0) && (divider_list[list_len].rnorm < divider_list[list_len-1].rnorm)){
        // Swap divider positions
        temp = divider_list[list_len];
        divider_list[list_len] = divider_list[list_len - 1];
        divider_list[list_len - 1] = temp;
        list_len--;
    }
}