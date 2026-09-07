// resistor_divider.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <math.h>
#define STATIC_GETOPT
#include "getopt.h"


int e6[] = { 100,150,220,330,470,680,680,680 };
int e12[] = { 100,120,150,180,220,270,
    330,390,470,560,680,820,820,820,820,820 };
int e24[] = { 100,110,120,130,150,160,
    180,200,220,240,270,300,
    330,360,390,430,470,510,
    560,620,680,750,820,910,910,910,910,910,910,910,910,910 };
int e48[] = { 100,105,110,115,121,127,
    133,140,147,154,162,169,
    178,187,196,205,215,226,
    237,249,261,274,287,301,
    316,332,348,365,383,402,
    422,442,464,487,511,536,
    562,590,619,649,681,715,
    750,787,825,866,909,953,953,953,953,953,953,953,953,953,953,953,953,953,953,953,953,953 };

int e96[] = { 100,102,105,107,110,113,
    115,118,121,124,127,130,
    133,137,140,143,147,150,
    154,158,162,165,169,174,
    178,182,187,191,196,200,
    205,210,215,221,226,232,
    237,243,249,255,261,267,
    274,280,287,294,301,309,
    316,324,332,340,348,357,
    365,374,383,392,402,412,
    422,432,442,453,464,475,
    487,499,511,523,536,549,
    562,576,590,604,619,634,
    649,665,681,698,715,732,
    750,768,787,806,825,845,
    866,887,909,931,953,976
,976,976,976,976,976,976,976,976,976,976,976,976,976,976,976,976 
,976,976,976,976,976,976,976,976,976,976,976,976,976,976,976,976 };

int e192[] = { 100,101,102,104,105,106,107,109,110,111,113,114,
    115,117,118,120,121,123,124,126,127,129,130,132,
    133,135,137,138,140,142,143,145,147,149,150,152,
    154,156,158,160,162,164,165,167,169,172,174,176,
    178,180,182,184,187,189,191,193,196,198,200,203,
    205,208,210,213,215,218,221,223,226,229,232,234,
    237,240,243,246,249,252,255,258,261,264,267,271,
    274,277,280,284,287,291,294,298,301,305,309,312,
    316,320,324,328,332,336,340,344,348,352,357,361,
    365,370,374,379,383,388,392,397,402,407,412,417,
    422,427,432,437,442,448,453,459,464,470,475,481,
    487,493,499,505,511,517,523,530,536,542,549,556,
    562,569,576,583,590,597,604,612,619,626,634,642,
    649,657,665,673,681,690,698,706,715,723,732,741,
    750,759,768,777,787,796,806,816,825,835,845,856,
    866,876,887,898,909,920,931,942,953,965,976,988
,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988
,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988 
,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988
,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988,988 };

struct res_calc {
    int r1;
    int r2;
    int factor;
};

int* lists[] = { e6,e12,e24,e48,e96,e192 };
int lengths[] = { sizeof(e6)/sizeof(e6[0]),
                    sizeof(e12)/sizeof(e12[0]),
                    sizeof(e24)/sizeof(e24[0]),
                    sizeof(e48)/sizeof(e48[0]),
                    sizeof(e96)/sizeof(e96[0]),
                    sizeof(e192)/sizeof(e192[0]) };

char* serieses[] = {
    "E6",
    "E12",
    "E24",
    "E48",
    "E96",
    "E192",
};

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

    int* resistor_list = lists[list_num];
    int list_length = lengths[list_num];

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

    while ((o = getopt(argc, argv, "vi:o:s:r:t:b")) != -1)
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

    if (optflag & 3 != 3) {
        print_usage();
        return 3;
    }

    while (strcmp(series, serieses[series_index])) {
        series_index++;
        if(series_index > 4) break;
    }
    if (vout / vin > 1) {
        printf("Invalid vout to vin ratio\n");
        return -1;
    }
    c = calculate(vin, vout, series_index, optflag);
    if (c.r1 < c.r2) c.factor = c.factor + 1;
    correction = (int)pow(10.0, abs(c.factor));
    if (c.factor < 0) c.r2 = c.r2 * correction;
    else c.r1 = c.r1 * correction;
    out_actual = (vin * c.r2) / ((c.r1 + c.r2));
    error = (vin * c.r2) / ((c.r1 + c.r2) * vout) - 1;
    printf("Vin: %.5g\n", vin);
    printf("Vout: %.5g\n", vout);
    printf("Series: %s\n", serieses[series_index]);
    printf("R1: %d\n", c.r1);
    printf("R2: %d\n", c.r2);
    printf("Actual Output: %.7g\n", out_actual);
    printf("Error: %.5g%%\n", error*100);
    return 0;
}
