#include <stdio.h>
#include "mpi.h"
#include <math.h>

// ������������� ������� 

double f(double x)

{

	return (pow(x, 2) / sqrt(pow(x, 2) + 1));

}

// ��������� �������� �� ������� [a,b] � ������ ����� ��������� n �� ������� ��������

double integrate(double a, double b, int n)

{

	double res; // ���������

	double h; // ��� �������������� 

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

	int my_rank; // ���� �������� �������� 

	int numprocs; // ����� ����� ��������� 

	double a = 0.0; // ����� ����� ��������� 

	double b = 100.0; // ������ ����� ��������� 

	int n = 10000000; // ����� ����� ��������� 

	double len; // ����� ������� �������������� ��� �������� ��������

	double local_a; // ����� ����� ��������� ��� �������� �������� 

	double local_b; // ������ ����� ��������� ��� �������� ��������

	int local_n; // ����� ����� ��������� ��� �������� �������� 

	double local_res; // �������� ��������� � ������� �������� 

	double result; // ��������� �������������� 

	double wtime; // ����� ������ ��������� 

	// ������ ������ � MPI 

	MPI_Init(&argc, &argv);

	// �������� ����� �������� �������� � ������ ���� ��������� 

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// �������� ����� ���������� ���������� ��������� 

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// �������� ������


	// ��������� ������ �� �������� 0 ��������� 

	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// ������������� ��������� 

	MPI_Barrier(MPI_COMM_WORLD);

	// ��������� ������

	wtime = MPI_Wtime();

	// ��������� ������� �������������� ��� �������� ��������

	len = (b - a) / numprocs;

	local_n = n / numprocs;

	local_a = a + my_rank * len;

	local_b = local_a + len;

	// ��������� �������� �� ������ �� ��������� 

	local_res = integrate(local_a, local_b,

		local_n);

	// ������� ��� ������ � �������� �������� 0

	MPI_Reduce(&local_res, &result, 1, MPI_DOUBLE,

		MPI_SUM, 0, MPI_COMM_WORLD);

	// ������������� ��������� 

	MPI_Barrier(MPI_COMM_WORLD);

	// ��������� ����� ������ 

	wtime = MPI_Wtime() - wtime;

	// ���������� �����

	if (my_rank == 0) {

		printf("Integral from %.2lf to %.2lf=%.8lf\n",

			a, b, result);

		printf("Working time: %.2lf seconds\n",

			wtime);

	}

	// ����������� ������ � MPI

	MPI_Finalize();

	return 0;

}