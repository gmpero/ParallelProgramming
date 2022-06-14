/*
8. 
Даны n обыкновенных дробей (массив числителей и массив знаменателей). 
Выполнить их рациональное сложение и умножение.
*/

#include <iostream>
#include <omp.h>
#include <iomanip>
#include <ctime>
#include <algorithm>

#define ADD

using namespace std;

class Fraction
{
private:
	int numerator;
	int denominator;

public:

	Fraction()
	{

#ifdef  ADD
		this->numerator = 0;
#else
		this->numerator = 1;
#endif

		this->denominator = 1;
	}

	Fraction(int numerator, int denominator)
	{
		this->numerator = numerator;
		this->denominator = denominator;
		if (denominator == 0)
		{
			cout << "denominator == 0" << endl;
			exit(1);
		}
	}

	friend Fraction operator+(Fraction a, Fraction b)
	{
		int tmp;
		int tmp2;

		//приведение к общему знаменателю
		if (b.denominator > a.denominator)
		{
			tmp = a.denominator;
			while (a.denominator % b.denominator != 0) a.denominator += tmp;

			tmp = a.denominator / tmp;

			tmp2 = a.denominator / b.denominator;

			b.denominator = a.denominator;

			a.numerator *= tmp;
			b.numerator *= tmp2;
		}
		else
		{
			tmp = b.denominator;
			while (b.denominator % a.denominator != 0) b.denominator += tmp;

			tmp = b.denominator / tmp;

			tmp2 = b.denominator / a.denominator;

			a.denominator = b.denominator;

			b.numerator *= tmp;
			a.numerator *= tmp2;
		}

		Fraction result = Fraction(a.numerator + b.numerator, a.denominator);
		result.fraction_reduction();

		return result;
	}

	friend Fraction operator*(Fraction a, Fraction b)
	{
		Fraction result = Fraction(a.numerator * b.numerator, a.denominator * b.denominator);
		result.fraction_reduction();

		return result;
	}

	void fraction_reduction()
	{
		int n = min(numerator, denominator);
		for (int i = n; i > 1; i--)
		{
			if (numerator % i == 0 && denominator % i == 0)
			{
				numerator /= i;
				denominator /= i;
				//break;
			}
		}
	}

	friend ostream& operator<<(ostream& os, const Fraction& a)
	{
		os << a.numerator << "/" << a.denominator;
		return os;
	}
};

int main()
{
	srand(time(NULL));

	int count = 2;

	double start;
	double end;
	int count_threads = omp_get_max_threads();
	start = omp_get_wtime();

	Fraction result = Fraction();
	Fraction* fraction = new Fraction[count];
	Fraction* thread_fraction = new Fraction[count_threads];

	for (int i = 0; i < count; i++)
	{
		fraction[i] = Fraction(rand() % 9 + 1, rand() % 9 + 1);
	}

#pragma omp parallel
	{
#pragma omp for ordered schedule(static)
		for (int i = 0; i < count; i++)
		{
			fraction[i].fraction_reduction();

#ifdef ADD
			thread_fraction[omp_get_thread_num()] = thread_fraction[omp_get_thread_num()] + fraction[i];
#else
			thread_fraction[omp_get_thread_num()] = thread_fraction[omp_get_thread_num()] * fraction[i];
#endif

#pragma omp ordered
			std::cout << fraction[i]
#ifdef ADD
				<< ((i != count - 1) ? '+' : ' ');
#else
				<< ((i != count - 1) ? '*' : ' ');
#endif
		}
	}

	for (int i = 0; i < count_threads; i++)
	{
#ifdef ADD
		result = result + thread_fraction[i];
#else
		result = result * thread_fraction[i];
#endif
	}

	cout << "= " << result << endl;

	end = omp_get_wtime();
	std::printf("time = %lf\n", end - start);
}

/*
51. 
Напишите программу нахождения по заданному N>0 всех троек x,y,z, x,y,zN, x≤N,  y≤N,z≤N, удовлетворяющих условию 
x+y+z=1000.
*/

#include <iostream>
#include<ctime>
#include <fstream>
#include <omp.h>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Rus");

    ofstream fout;
    fout.open("file.txt");
    int x = 0;
    int y = 0;
    int z = 0;
    int SIZE = 100;

        
#pragma omp for ordered schedule(static) collapse(3)
        for (x = 0; x <= SIZE; x++)
        {
            for (int y = 0; y <= SIZE; y++)
            {
                for (int z = 0; z <= SIZE; z++)
                {
                    if (x + y + z == SIZE)
                    {

                        fout << x << " " << y << " " << z << endl;


                    }
                }
            }
        }
    
    fout.close();
}

/*
55. Требуется реализовать вычисление числа ln 2 с помощью формулы:
*/

#include <iostream>
#include<ctime>
#include <omp.h>
using namespace std;
float sum = 0;
int main()
{
    setlocale(LC_ALL, "Rus");


    int count_iter = 2000000;
#pragma omp parallel
#pragma omp for ordered schedule(static) reduction(+:sum)//
    for (int n = 1; n < count_iter; n++)
    {
        if ((n + 1) % 2 == 0)
        {
            sum += 1 / (float)n;
        }
        else
        {
            sum += -1 / (float)n;
        }
    }

    cout << sum;

}
