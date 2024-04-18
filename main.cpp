#include <stdio.h>
#include "mpi.h"
#include <math.h>

// Интегрируемая функция 

double f(double x)

{

	return (pow(x, 2) / sqrt(pow(x, 2) + 1));

}

// Вычислить интеграл по отрезку [a,b] с числом точек разбиений n по формуле трапеций

double integrate(double a, double b, int n)

{

	double res; // результат

	double h; // шаг интегрирования 

	double x;

	int i;

	h = (b - a) / n;

	res = 0.5 * (f(a) + f(b)) * h;

	x = a;

	for (i = 1; i < n; i++) {

		x += h;

		res += f(x) * h;

	}

	return res;

}

int main(int argc, char* argv[])

{

	int my_rank; // ранг текущего процесса 

	int numprocs; // общее число процессов 

	double a = 0.0; // левый конец интервала 

	double b = 100.0; // правый конец интервала 

	int n = 10000000; // число точек разбиения 

	double len; // длина отрезка интегрирования для текущего процесса

	double local_a; // левый конец интервала для текущего процесса 

	double local_b; // правый конец интервала для текущего процесса

	int local_n; // число точек разбиения для текущего процесса 

	double local_res; // значение интеграла в текущем процессе 

	double result; // результат интегрирования 

	double wtime; // время работы программы 

	// Начать работу с MPI 

	MPI_Init(&argc, &argv);

	// Получить номер текущего процесса в группе всех процессов 

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Получить общее количество запущенных процессов 

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// Получить данные


	// Рассылаем данные из процесса 0 остальным 

	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Синхронизация процессов 

	MPI_Barrier(MPI_COMM_WORLD);

	// Запускаем таймер

	wtime = MPI_Wtime();

	// Вычисляем отрезок интегрирования для текущего процесса

	len = (b - a) / numprocs;

	local_n = n / numprocs;

	local_a = a + my_rank * len;

	local_b = local_a + len;

	// Вычислить интеграл на каждом из процессов 

	local_res = integrate(local_a, local_b,

		local_n);

	// Сложить все ответы и передать процессу 0

	MPI_Reduce(&local_res, &result, 1, MPI_DOUBLE,

		MPI_SUM, 0, MPI_COMM_WORLD);

	// Синхронизация процессов 

	MPI_Barrier(MPI_COMM_WORLD);

	// Вычисляем время работы 

	wtime = MPI_Wtime() - wtime;

	// Напечатать ответ

	if (my_rank == 0) {

		printf("Integral from %.2lf to %.2lf=%.8lf\n",

			a, b, result);

		printf("Working time: %.2lf seconds\n",

			wtime);

	}

	// Заканчиваем работу с MPI

	MPI_Finalize();

	return 0;

}