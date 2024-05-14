#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

double func(double x)
{
    return log(1 + x) / x;
}

int main(int argc, char **argv)
{
    const double eps = 1E-5;
    const double a = 0.1;
    const double b = 1.0;
    const int n0 = 10000000;

    double time = wtime();
    int k,n = n0;
    double sq[2], delta = 1;
    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = 0; i < n; i++)
            s += func(a + h * (i + 0.5));// left right
        sq[k] = s * h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }

    time = wtime() - time;

    printf("Runge rule: EPS %e, n %d time - %f\n\n\n", eps, n / 2, time);
    printf("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0, eps);

    for(int th = 2;th <= 8;th += 2){
        double sq[2];
        double t = omp_get_wtime();
        #pragma omp parallel num_threads(th)
        {
            int n = n0, k;
            double delta = 1;
            for (k = 0; delta > eps; n *= 2, k ^= 1) {
                double h = (b - a) / n; // ширина подотрезка
                double s = 0.0;
                sq[k] = 0;
                #pragma omp barrier // избегание гонки данных
                #pragma omp for nowait // распределение нагрузки по потокам
                for (int i = 0; i < n; i++)
                    s += (func(a + h * (i + 1)) + func(a + h * (i + 0))) / 2; // left right
                #pragma omp atomic // дает доступ для одного потока и запрещает другим
                sq[k] += s * h;
                #pragma omp barrier 
                if(n > n0)
                    delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
                #if 0
                printf("n=%d i=%d sq=%.12f delta=%.12f\n", n, k, sq[k], delta);
                #endif
            }
            #pragma omp master // один поток выводит
            printf("Result: %.12f; Runge rule: EPS %e, n %d th - %d\n", sq[k], eps, n / 2,th);
            // printf("%d\t", th);
        }

        t = omp_get_wtime() - t;
        printf("Elapsed time (sec.): %.6f\n",time / t);
        // printf("%.6f\n",time / t);
    }

    return 0;
}