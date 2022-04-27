#include <iostream>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <iomanip>      
#include <locale.h>
using namespace std;
int* Best;
int N;//множество работ
int  nokr = 0;
int* X = new int[N]; //момент начала выполнения работ
int* Y = new int[N];//момент конца выполнения работ
int* T = new int[N]; //длительность выполнений работ
int* D = new int[N]; //директивные сроки
int* d = new int[N]; //вектор моментов поступлений работ
int* C = new int[N]; //стоимость ожидания обслуживания
int* F = new int[N];//коэффициент штрафов

void output(int i)
{
	cout << " \t задача №" << i+1 << endl;
	//cout << "  момент начала выполнения X=" << X[i] << endl;
	//cout << "  момент конца выполнения Y=" << Y[i] << endl;
	cout << "   T=" << T[i];
	cout << "   D=" << D[i];
	cout << "   d=" << d[i];
	//cout << "  коэффициент штрафов F=" << F[i] << endl;
	cout << endl;
}

void initRaspis(int* p, int n)
{


	cout << "ручной ввод других данных ? (y/n)" << endl;
	char c;
	cin >> c;

	if (c == 'n')
	{//
	here:
		//srand(time(NULL));
		for (int i = 0; i < N; i++)
		{

			T[i] = rand() % 10 + 1;
			d[i] = rand() % 10 + 1;
			D[i] = rand() % (2 * (T[i] + d[i]))+ (T[i]+d[i]);
			C[i] = rand() % 10 + 0;
			//X[0] =0;
			//cout << " \t 1задача №" << i << endl;

			output(i);
		}

	}
	else
	{
		goto here;
	}
	//return 0;
}
int Raspis(int* p, int N)//получаем порядок выводим его штрафы
{

	int m = 0;
	
	//cout << "T T T " << endl;
	//12345 34512 p0-1=2
	X[0] = d[p[0] - 1];
	Y[0] = T[p[0] - 1]+X[0];
	for (int i = 1; i < N; i++)
	{
		if (d[i] <= Y[i - 1])
		{
			X[i] = Y[i - 1] + 1;
		}
		else X[i] = d[i];

		Y[i] = X[i] + T[p[i] - 1];

		//cout << " X=" << X[i] << endl;
		//cout << " Y=" << Y[i] << endl;
		if (X[i] < d[i]) { return -1; }
		F[i - 1] = abs(d[i - 1] - X[i - 1]);

		
		//output(i);


	}

	F[N - 1] = abs(d[N - 1] - X[N - 1]);
	int max = 0;
	for (int i = 0; i < N; i++)
	{
		//максимум
		if (F[i] > max)
			max = F[i];
	}
	m = max;
	cout << " m=" << m << endl;
	//m=штраф расписания
	return m;

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
			if (m == -1) { cout << "некорректная последовательность" << endl; goto here; }
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


int** okr(int n, int* D, int** okrs,int dot)
{

	cout << " окрестность: ";
	int* ar = new int[N];
	for (int i = 0; i < n; ++i) {
		ar[i] = D[i]; //cout << ar[i];
	}
	//cout << endl;

	swap(ar[dot], ar[0]);
	for (int k = 1; k <= N; k++) {
		
		for (int j = 0; j < N; j++) {
			okrs[nokr][j] = ar[j];
			cout << ar[j];
		}
		cout << endl;
		nokr++;
		//							1			2				3->4				
		//12345 dot=2 (3) 32145 1 12345 13245 2 12345 12435 3		12345 12453		
		if (k != dot) { swap(ar[dot], ar[k-1]); swap(ar[k], ar[dot]); }
		else {  swap(ar[dot], ar[k - 1]); swap(ar[k], ar[k + 1]); k++;}
	}
	
	//cout << "nokr-" << nokr;

	return okrs;

}

int hill(int* p)
{

	int c = 0, n = N,i=0;
	unsigned int dot, max = 0, x = 0, x1=0;
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
	//x0 = D[x0];
	cout << " нач точка p[" << dot<<"]" << endl;
	x = Raspis(D, N);
	
	cout << "штраф начальной перестановки-" << x << endl;
	max = x;
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
		x-ее штраф
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

		cout << "maxO1 ";
		for (int j = 0; j < N; j++)
		{

			cout << maxO[j];
		}
		cout << endl;
		okrs=okr(N, D, okrs,dot);
		cout << "maxO2 ";
		for (int j = 0; j < N; j++)
		{

			cout << maxO[j];
		}
		cout << endl;
		for (int k = 0; k < nokr; k++)//поиск макс окр
		{

			for (int j = 0; j < N; j++)
			{

				per[j] = okrs[k][j];
			}
			// x1 = 0;
			
			x1 = Raspis(per, N);
			if (x1 == -1) {
				cout << "некорректная последовательность" << endl;
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
				maxO = okrs[k];

				cout << "нов макс окр =" << endl;
				for (int j = 0; j < N; j++)
				{

					cout << maxO[j];
				}
				cout << endl;
				max = x1;

			}
		here1:
			cout << endl;
		
		}
		nokr = 0;
		cout << "\t";
		//  mmx = maxO;

		//  
		i++;
		// сout << "\n";


		cout << "финал макс окр = ";
		for (int j = 0; j < N; j++)
		{

			cout << maxO[j];
		}
		cout << endl;
		D = maxO;
		x = max;
		dot = rand() % (N);
		for (int j = 0; j < c; j++)
		{
			if (dot == udot[j])
				dot = rand() % (N); 
		}
		udot[c] = dot;
		c++;
		

	}
	
	cout << " финальный штраф " << x << endl;
	/*for (int i = 0; i < N; i++) {
		delete[] okrs[i];
	}
	delete[] okrs;*/
	return x;
}

int main()
{

	setlocale(LC_ALL, "RUS");
	srand(time(0));
	cout << "введите количество работ" << endl;
	cin >> N;
	int* p = new int[N];
	for (int i = 0; i < N; i++)p[i] = i + 1;
	initRaspis(p, N);
	if (Raspis(p, N) == -1) {
		cout << "некорректная последовательность" << endl;
/*
		int* p1 = new int[N];
		int* p2 = new int[N];
		for (int i = 0; i < N; i++) { p1[i] = i + 1; p2[i] = i + 1;
		}
		int min = 100;
		cout << "Упорядочивание" << endl;
		//с помощью F или с помощью d ??
		//c помощью F берем одно из чисел и смотрим на какой из позиций F меньше-лучше подойдет для 3тьего
		//с помощью смотрим по времени поступления ,сравниваем -лучше подойдет для 4 условия
		
		for (int k = 0; k < N; k++)
		{
			for (int k1 = 0; k1 < N; k1++)
			{
				cout << p1[k1];
			}
			cout << endl;
			/*
			Y[0] = T[p1[0] - 1];
			while(Y[]<D[k])
			{
				X[j] = Y[j - 1] + 1;

				Y[j] = X[j] + T[p[j] - 1];
					F[k] = abs(d[k] - X[j - 1]);
				if (F[k] < min)
				{
					min = F[k];
				}
				swap(p1[k], p1[j+1]);
			}//
			swap(p1[k], p1[0]);
			cout << "свап-";
			for (int k1 = 0; k1 < N; k1++)
			{
				cout << p1[k1];
			}
			cout << endl;
			Y[0] = T[p1[0] - 1];
			X[0] = 0;
			F[k] = abs(d[k] - X[0]);
			min = F[k];
			cout << "min-" << min << endl;
			for (int i = 0; i < N; i++)p2[i] = p1[i];
			if (min != 0) {
				for (int j = 1; j <= N; j++) {
					Y[0] = T[p1[0] - 1];
					X[0] = 0;
					X[j] = Y[j - 1] + 1;
					Y[j] = X[j] + T[p1[j] - 1];
					cout << " X=" << X[j] << endl;
					cout << " Y=" << Y[j] << endl;
					cout << " D=" << D[j-1] << endl;
					if (Y[j] > D[j]) { break; }
					F[k] = abs(d[k] - X[j - 1]);
					cout << "F[" << k << "]" << F[k] << endl;
					if (F[k] < min)
					{
						min = F[k];
						for (int i = 0; i < N; i++)p2[i] = p1[i];
						cout << "p2-";
						for (int k1 = 0; k1 < N; k1++)
						{
							cout << p2[k1];
						}

						cout << endl;
						cout << "min- " << min << endl;
					}
					cout << "p1-";

					//							1			2				3->4				
					//12345 dot=2 (3) 32145 1 12345 13245 2 12345 12435 3		12345 12453		

					if (j != k) { swap(p1[k], p1[j - 1]); swap(p1[j], p1[k]); }
					else { swap(p1[k], p1[j - 1]); swap(p1[j], p1[j + 1]); j++; }

					for (int k1 = 0; k1 < N; k1++)
					{
						cout << p1[k1];
					}
					cout << endl;
				}

				for (int i = 0; i < N; i++)p1[i] = p2[i];
			}

		}
		for (int i = 0; i < N; i++)p[i] = p1[i];
		*/	}
	
	
	cout << "метод восхождения на холм " << endl;
	int prib = hill(p);
	Best = new int[N];
	int M = Perebor(0, p, N);
	cout << " Точное решение " << M << endl;
	for (int j = 0; j < N; j++)
	{
		cout << Best[j];
	}
	cout << endl;
	int raz = prib - M;
	double ab=0,o=0,o1=0;
	if (M == 0) {
		ab;
		if (raz != 0) ab = (double)raz / N;
		cout << "Абсолютное отклонение =" << ab;
	}
	else {
		 o = ((double)raz / (double)M)*100;
		 //o = o1 * 100;
		
		cout << "Относительное отклонение ="
			<< o << "%" << endl;

	}
	delete[] Best;
	delete[] X;
	delete[] Y;
	delete[] T;
	delete[] D;
	delete[] d;
	delete[] C;
	delete[] F;
	return 0;
	
}
