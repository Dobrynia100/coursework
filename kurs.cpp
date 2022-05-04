#include <iostream>
#include <fstream>
#include <algorithm>
//#include <math.h>
#include <cmath>
#include <iomanip>      
#include <locale.h>
using namespace std;
int* Best;
int* Best2;
int  nokr = 0;//колич эл-ов окрестности
int* X ; //момент начала выполнения работ
int* Y; //момент конца выполнения работ
int* T; //длительность выполнений работ
int* D; //директивные сроки
int* d; //вектор моментов поступлений работ
int* C ; //стоимость ожидания обслуживания
int* F ;//коэффициент штрафов

void output(int i)
{
	cout << " \t задача №" << i+1 << endl;
	
	cout << "   T=" << T[i];
	cout << "   D=" << D[i];
	cout << "   d=" << d[i];
	cout << endl;
}

void initRaspis(int N)
{


		//srand(time(NULL));
	cout << "Ввод из файла ?(y/n)" << endl;
	char c;
	cin >> c;
	//c = 'y';
	if (c == 'n')
	{
		for (int i = 0; i < N; i++)
		{

			T[i] = rand() % 10 + 1;
			d[i] = rand() % 10 + 1;
			D[i] = rand() % (2 * (T[i] + d[i])) + (T[i] + d[i]);
			C[i] = rand() % 10 + 0;
			X[i] = 0;

			//cout << " \t 1задача №" << i << endl;

			output(i);
		}
	}
	else
	{
		
		ifstream infile;
		infile.open("input.txt");
		
			for (int i = 0; i < N; i++)
			{

				infile >> T[i];
				
			
			}
			for (int i = 0; i < N; i++)
			{

				
				infile >> d[i];
				
				
			}
			for (int i = 0; i < N; i++)
			{

				
				infile >> D[i];
				output(i);
			}
		infile.close();
		
	}
	//return 0;
}
int Raspis(int* p, int N)//получаем порядок выводим его штрафы
{

	int m = 0;
	
	//c конца 
	//cout << "T[N-1] " << T[p[N - 1] - 1] << endl;

	X[N-1] = D[p[N-1] - 1]-T[p[N - 1]-1];
	Y[N-1] = D[p[N-1] - 1];
	F[N-1] = abs(d[p[N - 1] - 1] - X[N-1]);
	
	/*cout << "p=" << p[N - 1] << endl;
	cout << " X=" << X[N-1] << endl;
	cout << " Y=" << Y[N-1] << endl;
	cout << " F=" << F[N-1] << endl;*/
	for (int i = N-2; i >= 0; i--)
	{
		
		Y[i] = X[i+1];
		X[i] = Y[i] - T[p[i] - 1];
	
		if (Y[i] > D[p[i] - 1]) { return -1; }
		F[i] = abs(d[p[i] - 1] - X[i]);
		//cout << " F=" << F[i] << endl;
	}
	int max = 0;
	for (int i = 0; i < N; i++)
	{
		//максимум
		if (F[i] > max)
			max = F[i];
	}
	m = max;
	//cout << " m=" << m << endl;
	//m=штраф расписания
	return m;

}

void test(int* p)
{
	cout << "test" << endl;
	int p1[9] = { 2, 9, 6, 3, 4, 7, 8, 1, 5 };
	for (int i = 0; i < 9; ++i) {
		p[i] = p1[i]; cout << p[i];
	}
	cout << endl;
	int m=Raspis(p, 9);
	cout << " m=" << m << endl;
	exit(0);
	
}
int Perebor(int k, int* p, int N)
{
	static int min1 = INT_MAX;
	int  m;
	{
		if (k == N)//полная перестановка
		{
			for (int i = 0; i < N; i++)
				cout << p[i];
			m = Raspis(p, N);//строим расписание. Написать отдельную функцию 
			if (m == -1) { cout << " некорректная последовательность" << endl; goto here; }
			if (min1 > m)// ищем минимум
			{
				min1 = m;
				for (int j = 0; j < N; j++)
				{
					Best[j] = p[j];//Best - глобальный массив
				}
			}
			cout << " (" << m << ")" << endl;
		}
	}
	//фиксированная часть, уже построенная длины k
	//формируем остаток
	here:
	for (int j = k; j < N; j++)
	{
		swap(p[k], p[j]);//переставили местами
		Perebor(k + 1, p, N);//уменьшили хвост
		swap(p[k], p[j]);
	}
	return min1;//возвращаем минимальное значение критерия
}


int** okr(int N, int* D, int** okrs,int dot)
{

	cout << " окрестность: ";
	int* ar = new int[N];
	for (int i = 0; i < N; ++i) {
		ar[i] = D[i]; //cout << ar[i];
	}
	//cout << endl;

	swap(ar[dot], ar[0]);
	for (int k = 1; k <= N; k++) {
		
		for (int j = 0; j < N; j++) {
			okrs[nokr][j] = ar[j];
			cout << ar[j];
		}
		cout << " ";
		nokr++;
		//							1			2				3->4				
		//12345 dot=2 (3) 32145 1 12345 13245 2 12345 12435 3		12345 12543		
		if (k != dot) { swap(ar[dot], ar[k-1]); swap(ar[k], ar[dot]); }
		else {  swap(ar[dot], ar[k - 1]); swap(ar[k], ar[k + 1]); k++;}
	}
	cout << endl;
	//cout << "nokr-" << nokr;

	return okrs;

}

int hill(int* p,int N)
{

	int c = 0, n = N,i=0;
	unsigned int dot, max = 0, x1=0;
	int* D;
	D = new int[N];
	int* maxO = new int[N];

	for (int j = 0; j < N; j++) //кодировки
	{

		D[j] = p[j];
		maxO[j] = D[j];
		cout << " " << p[j];
	}
	
	cout << endl;
	//srand(time(NULL));
	int* udot = new int[N];
	dot = rand() % (N);
	udot[c] = dot;
	c++;
	cout << " нач точка p[" << dot<<"]" << endl;
	max = Raspis(D, N);
	
	cout << "штраф начальной перестановки-" << max << endl;
	int** okrs = new int* [n];
	for (int j = 0; j < N; j++)
	{
		okrs[j] = new int[N];
	}

	int m = 0;
	int* per = new int[N];
	for (int j = 0; j < N; j++)
	{
		per[j] = 0;
	}

	while (i <= n)
	{

		/*
		12345=D
		max-ее штраф
		ее окрестность
		while{
		окрестность{х1-штраф окр[j] если штраф меньше берем эту перестановку}
		шаг-
		D=перестановка с меньшим штрафом
		}
		*/

		cout << " Шаг-" << i << endl;
		cout << " точка-p[" << dot << "]" <<endl;
		cout << " max штраф-" << max << endl;

		okrs=okr(N, D, okrs,dot);
	
		for (int k = 0; k < nokr; k++)//поиск макс окр
		{

			for (int j = 0; j < N; j++)
			{

				per[j] = okrs[k][j];
			}
			
			
			x1 = Raspis(per, N);
			if (x1 == -1) {
				cout << " некорректная последовательность" << endl;
				goto here1;
			}
			cout << "выбр перестановка ";
			for (int j = 0; j < N; j++)
			{

				cout << per[j];
			}
			cout << endl;
			cout << "ее штраф=" << x1 << endl;

			if (x1 < max)
			{
			

				cout << "нов макс окр =" << endl;
				for (int j = 0; j < N; j++)
				{
					maxO[j] = okrs[k][j];
					cout << maxO[j];
				}
				cout << endl;
				max = x1;

			}
		here1:
			cout << endl;
		
		}
		nokr = 0;
		
		i++;
		cout << "финал макс окр = ";
		for (int j = 0; j < N; j++)
		{

			cout << maxO[j];
		}
		cout << endl;
		
		D = maxO;
		
		dot = rand() % (N);
		for (int j = 0; j < c; j++)
		{
			if (dot == udot[j])
				dot = rand() % (N); 
		}
		udot[c] = dot;
		c++;
		
	}
	for (int j = 0; j < N; j++)
	{
		Best2[j] = maxO[j];
	}
	cout << " финальный штраф " << max << endl;	
	/*try {
		for (int i = 0; i < N; i++) {
			delete[] okrs[i];
		}
		delete[] okrs;
	}
	catch (...)
	{
		cout << "error";
	}*/
	return max;
	
}
int* rand_per(int n,int* p)
{
	int* ar = new int[n];
	int r;
	for (int i = 0; i < n; ++i)
		ar[i] = i + 1;
	
	do
	{
		r = rand() % 100;//n
		if (r >= 90) {
			for (int i = 0; i < n; ++i) {
				//cout << ar[i] << " ";
				p[i] = ar[i];
			}
		}
		//cout << endl;

	} while (next_permutation(ar, ar + n));

	return p;
}
int main()
{

	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int N;//множество работ
	cout << "введите количество работ" << endl;
	cin >> N;
	
	int* p = new int[N];
	X = new int[N];
	Y = new int[N];
	D = new int[N];
	d = new int[N];
	F = new int[N];
	T = new int[N];
	C = new int[N];
	
	for (int i = 0; i < N; i++)p[i] = i + 1;
	initRaspis(N);
	//test(p);
	Best2 = new int[N];
	p = rand_per(N, p);
	if (Raspis(p, N) == -1) {
		
		while (Raspis(p, N) == -1)
		{
			p = rand_per(N, p);
		}
	}
	
	cout << "метод восхождения на холм " << endl;
	int prib = hill(p,N);
	Best = new int[N];
	
	int M = Perebor(0, p, N);
	cout << " Точное решение: " << M << endl;
	
	for (int j = 0; j < N; j++)
	{
		cout << Best[j];
	}
	cout << endl;
	cout << " решение восхождением на холм:" << prib << endl;
	for (int j = 0; j < N; j++)
	{
		cout << Best2[j];
	}
	cout << endl;
	int raz = prib - M;
	double ab = 0, o = 0;
	if (M == 0) {
		ab;
		if (raz != 0) ab = (double)raz / N;
		cout << "Абсолютное отклонение =" << ab;
	}
	else {
		try {
			o = ((double)raz / (double)M) * 100;
			

			cout << "Относительное отклонение ="<< setprecision(2)<< o << "%" << endl;
		}
		catch (const bad_alloc& e)
		{
			cout << "Error: " << e.what() << '\n';
		}


	}
	delete[] Best;
	delete[] Best2;
	delete[] X;
	delete[] Y;
	delete[] T;
	delete[] D;
	delete[] d;
	delete[] C;
	delete[] F;
	return 0;
	
}
