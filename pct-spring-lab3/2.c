#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>

const double PI = 3.14159265358979323846;
int n = 10000000;

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

double getrand(unsigned int *seed)
{
    return (double)rand_r(seed) / RAND_MAX;
}

double getand()
{ return (double)rand() / RAND_MAX; }


double func(double x,double y)
{
    return pow(exp(x + y),2);
}

int main(int argc, char **argv)
{
    int in = 0;
    double s = 0;
    double time = wtime(); 

    for (int i = 0; i < n; i++) {
        double x = getand() * 1;
        double y = getand() * 0.1 - x;
        if (y <= sin(x)) {
            in++;
            s += func(x,y);
        }
    }

    time = wtime() - time;

    double v = PI * in / n;
    double res = v * s / in;

    printf("Result: %.12f, n %d time - %f\n\n\n", res, n, time);

    s = in = 0;
    for(int n = 10000000;n <= 100000000;n *= 10){
        for(int th = 2;th <= 8;th += 2){
        
            double t = omp_get_wtime();
            #pragma omp parallel num_threads(th)
            {
                double s_loc = 0;
                int in_loc = 0;
                unsigned int seed = omp_get_thread_num();

                #pragma omp for nowait

                for (int i = 0; i < n; i++) {
                    double x = getrand(&seed) * 1;
                    double y = getrand(&seed) * 0.1 - x;

                    if (y <= sin(x)) {
                        in_loc++;
                        s_loc += func(x, y);
                    }
                }

                #pragma omp atomic
                s += s_loc;
                #pragma omp atomic
                in += in_loc;
            }
            double v = PI * in / n;
            double res = v * s / in;
            t = omp_get_wtime() - t;

            // printf("Result: %.12f, n - %d , th - %d time - %f\n", res, n, th,time / t);
            // printf("====================================================\n");
            printf("%d\t%.6f\n", th, time / t);
        }
    }

    return 0;
}