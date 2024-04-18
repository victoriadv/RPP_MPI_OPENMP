#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <ctime>

using namespace std;

double f(double x)

{
    return (pow(x, 5) / sqrt(pow(x, 2) + 1));
}

int main()
{
    int n = 1, N = 1;
    cout << "Enter number of proccesses: ";
    cin >> n;
    cout << "Enter N: ";
    cin >> N;

    double res, res_prev;
    const double a = 0.0;
    const double b = 1.0;
    double h;
    double x;
    double wtime;
    int i;
    wtime = omp_get_wtime();
    h = (b - a) / N;
    res = 0.0;
    res_prev = 0.0;
    omp_set_num_threads(n);
#pragma omp parallel firstprivate(x, res_prev) shared(res)
    {
#pragma omp for 
        for (i = 1; i <= N - 1; i++)
        {
            x = a + i * h;
            res_prev = res_prev + f(x);
        }
#pragma omp critical
        res = res + res_prev;
    }
    res = (res + (f(a) + f(b)) / 2.0) * h;
    wtime = omp_get_wtime() - wtime;
    printf("Integral from %.2lf to %.2lf=%.8lf\n", a, b, res);
    printf("Working time: %.2lf seconds\n", wtime);
    return 0;

}