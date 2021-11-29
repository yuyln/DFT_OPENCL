#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const double pi = acos(-1.0);

int main()
{
    FILE *f = fopen("in.in", "w");
    int N = 320;
    double x1 = 1.0;
    double h = x1 / N;
    for(int i = 0; i < N; i++)
    {
        double x = i * h;
        double fx = 0.0, fy = 0.0;
        fx = 3.0 * cos(40.0 * pi * x);
        fy = 3.0 * sin(10.0 * pi * x);
        // fx += sin(2.0 * pi * 4.0 * x);
        // fx += 0.5 * sin(2.0 * pi * 7.0 * x);
        fprintf(f, "%d\t%.12f\t%.12f\n", i, fx, fy);

    }
    fclose(f);

    return 0;
}