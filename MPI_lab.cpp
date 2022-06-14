/* 13. Умножение матриц, ленточный алгоритм. */

#include <iostream>
#include <cstdio>
#include "mpi.h"

using namespace std;
int main(int *argc, char **argv)
{
    setlocale(LC_ALL, "Rus");
    int size, rank;

        MPI_Init(argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int n = 10;
        int m = 20;
        int k = 15;

        int count_iter = n / size;

        int* A = new int[0];
        int* buff_A = new int[count_iter * m];
        int* B = new int[m * k];
        int* buff_C = new int[count_iter * k];
        int* C = new int[0];

        if (rank == 0)
        {
            C = new int[n * k];
            A = new int[n * m];
            for (int i = 0; i < n * m; i++)
                if (i / m == i % m)
                    A[i] = 1;
                else
                    A[i] = 3;

            for (int i = 0; i < m * k; i++)
                if (i / k == i % k)
                    B[i] = 1;
                else
                    B[i] = 1;
        }

        MPI_Scatter(A, count_iter * m, MPI_INT, buff_A, count_iter * m, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(B, m * k, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < count_iter; i++)
        {
            for (int j = 0; j < k; j++)
            {
                buff_C[i*k + j] = 0;
                for (int v = 0; v < m; v++)
                {
                    buff_C[i * k + j] += buff_A[i * m + v] * B[v * k + j]; // ?????
                }
            }
        }

        MPI_Gather(buff_C, count_iter * k, MPI_INT, C, count_iter * k, MPI_INT, 0, MPI_COMM_WORLD);


        if (rank == 0)
        {
            for (int i = 0; i < n % size; i++)
            {
                for (int j = 0; j < k; j++)
                {
                    C[(count_iter * size +i) * k + j] = 0;
                    for (int v = 0; v < m; v++)
                    {
                        C[(count_iter * size + i) * k + j] += A[(count_iter * size + i) * m + v] * B[v * k + j]; // ?????
                    }
                }
            }

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < k; j++)
                {
                    cout << C[i * k + j] << ' ';
                }
                cout << endl;
            }
        }

        MPI_Finalize();

        //cout << "ln2 = " << sum;
        return 0;
}

/* 
51. 
Напишите программу нахождения по заданному N>0 всех троек x,y,z, x,y,zN, x≤N,  y≤N,z≤N, 
удовлетворяющих условию x+y+z=1000.
*/

#include <iostream>
#include <cstdio>
#include "mpi.h"

using namespace std;



int main(int* argc, char** argv)
{
	setlocale(LC_ALL, "Rus");
	int size, rank;

	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	
	int x = 0;
	int y = 0;
	int z = 0;
	int SIZE = 100;

	for (int x = rank; x  <= SIZE; x += size) 
	{	
			printf("Process: %d, size: %d\n", rank, size);
			for (int y = 0; y <= SIZE; y++)
			{
				for (int z = 0; z <= SIZE; z++)
				{
					if (x + y + z == SIZE)
					{

						
						printf("%d %d %d\n", x, y, z);


					}
				}
			}
	}
	//result_numerator
	//result_denominator
	MPI_Finalize();

	return 0;
}

/*
52.
Напишите программу нахождения по заданному N>0 всех троек x,y,z, x,y,zN, x≤N,  y≤N,z≤N, удовлетворяющих условию 
x2+y2+z3=10000000.
*/

#include <iostream>
#include <cstdio>
#include "mpi.h"

using namespace std;
int main(int* argc, char** argv)
{
	setlocale(LC_ALL, "Rus");
	int size, rank;

	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N = 10000000;

	int xyz;

	for (int x = rank; x * x <= N; x += size)
	{
			
		for (int y = 0; y * y  <= N; y++)
		{
			for (int z = 0; z * z * z <= N; z++)
			{
				if ((xyz = x * x + y * y  + z * z * z) == N)
				{
					printf("Process: %d, size: %d\n", rank, size);
					printf("%d %d %d\n", x, y, z);
				}
				if (xyz > N) break;
			}
			
		}
	}

	MPI_Finalize();

	return 0;
}

/*
55.
ln2 = ?
*/

#include <iostream>
#include<ctime>
#include "mpi.h"
using namespace std;

int main(int* argc, char** argv)
{
    setlocale(LC_ALL, "Rus");
    
    int size, rank;

    MPI_Init(argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int count_iter = 20000;
    float sum = 0;
    float result = 0;

    for (int n = rank+1; n < count_iter; n += size)//int x = rank
    {
        sum += (float)((n % 2 == 0) ? -1 : 1) / n;
    }
    MPI_Reduce(&sum, &result, 1, MPI_FLOAT, MPI_SUM, 1, MPI_COMM_WORLD);
    if (rank == 1) 
    {
        cout << result<< endl;
    }
    MPI_Finalize();
}

/*
57.
Требуется реализовать вычисление числа pi/12 с помощью формулы:
*/

#include <iostream>
#include<ctime>
#include "mpi.h"
using namespace std;

int main(int* argc, char** argv)
{
    setlocale(LC_ALL, "Rus");

    int size, rank;

    MPI_Init(argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int count_iter = 20000;
    float sum = 0;
    float result = 0;

    for (int n = rank + 1; n < count_iter; n += size)//int x = rank
    {
        sum += (float)((n % 2 == 0) ? -1 : 1) / n/n;
    }
    MPI_Reduce(&sum, &result, 1, MPI_FLOAT, MPI_SUM, 1, MPI_COMM_WORLD);
    if (rank == 1)
    {
        cout << result << endl;
    }
    MPI_Finalize();
}
