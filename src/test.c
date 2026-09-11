
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <math.h>
#include "standard_resistors.h"
#include "calc_tools.h"

int main(int argc, char **argv){
    int l = atoi(argv[2]);
    int x = tgt_idx(atoi(argv[1]),stdres_lists[l],stdres_list_lengths[l]);
    printf("%d %d\n",x,stdres_lists[l][x]);
}