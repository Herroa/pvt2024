#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

double func(double x)
{
    return exp(-x * x);
}

int main(int argc, char **argv)
{
    double t = omp_get_wtime();
    const double a = -4.0;
    const double b = 4.0;
    const int n = 10000000;
    printf("Numerical integration: [%f, %f], n = %d\n", a, b, n);
    double h = (b - a) / n;
    double s = 0.0;
    double sumloc[omp_get_max_threads()];

#pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int tid = omp_get_thread_num();
        int points_per_thread = n / nthreads;
        int lo = tid * points_per_thread;
        int hi = (tid == nthreads - 1) ? n - 1 : lo + points_per_thread;
        sumloc[tid] = 0.0;
        for (int i = lo; i <= hi; i++)
            sumloc[tid] += func(a + h * (i + 0.5));

#pragma omp atomic
        s += sumloc[tid];
    }
    s *= h;
    printf("Result Pi: %.12f\n", s * s);
    t = omp_get_wtime() - t;
    printf("Elapsed time (sec.): %.12f\n", t);
    return 0;
}