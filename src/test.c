
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <math.h>
#include "standard_resistors.h"
#include "calc_tools.h"
#include "resistor_calculator.h"

int main(int argc, char **argv){
    int l = atoi(argv[2]);
    int x = target_index(atoi(argv[1]),stdres_lists[l],stdres_list_lengths[l]);
    printf("%d %d\n",x,stdres_lists[l][x]);

    struct res_calc r[4];
    r[0] = (struct res_calc){100,100,0,0.1};
    r[1] = (struct res_calc){100,100,0,0.6};
    r[2] = (struct res_calc){100,100,0,0.8};
    r[3] = (struct res_calc){100,100,0,0.2};
    resistor_percolate(r,4);

    for(int i = 0; i < 4; i++){
        printf("%d %d %d %09.4f\n",r[i].r1,r[i].r2,r[i].factor,r[i].rnorm);
    }
}