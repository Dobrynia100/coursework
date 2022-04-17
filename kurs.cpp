#include <iostream>
#include <algorithm>
#include <math.h>
#include <cmath>
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
		cout << " \t 2задача №" << i << endl;
		//cout << "  момент начала выполнения X=" << X[i] << endl;
		//cout << "  момент конца выполнения Y=" << Y[i] << endl;
		cout << "   T=" << T[i];
			cout << "   D=" << D[i];
			cout << "   d=" << d[i];
		//cout << "  коэффициент штрафов F=" << F[i] << endl;
			cout << endl;
	}

	
int* permutation(int n, int* p)
{
	int* ar = new int[n];
	for (int i = 0; i < n; ++i)
		ar[i] = i + 1;

	do
	{
		for (int i = 0; i < n; ++i) {
			//cout << ar[i] << " ";
			p[i] = ar[i];
		}
		//cout << endl;

	} while (next_permutation(ar, ar + n));

	return p;
}
void initRaspis(int* p, int n)
{

	
	cout << "ручной ввод других данных ? (y/n)" << endl;
	char c;
	cin >> c;
	
	if (c == 'n')
	{//
		here:
		srand(time(NULL));
			for (int i = 0; i < N; i++)
			{
				
				T[i] = rand() % 10 + 1;
				d[i] = rand() % 10 + 1;
				D[i] = rand() % 2*T[i]+T[i];
				C[i] = rand() % 10 + 0;
				X[0] = 0;		
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
	
	int m=0;
	/*for (int i = 0; i < N; i++)
	{
		cout << " p[i]-1=" << p[i] - 1 << endl;
		m+= X[p[i]-1] + T[p[i]-1];
		cout << " m=" << m << endl;
	}*/
	/*
	Y[0] = X[p[0]-1] + T[p[0]-1];
	for (int i = 1; i <= N; i++)
	{
		cout << "\t i=" << i << endl;
		X[i] = Y[i - 1];// rand() % 10 + 0;
		cout << " X=" << X[i - 1] << endl;
		Y[i] = X[p[i-1]] + T[p[i-1]];// rand() % 10 + 0;
		cout << " Y=" << Y[i - 1] << endl;
		

		}
	*/
	
	Y[0] = T[p[0]-1];//12345 34512 p0-1=2
	for (int i = 1; i < N; i++)
	{
		
		X[i] =Y[i - 1]+1;
		
		Y[i] = X[i] + T[p[i]-1];
		
		F[i - 1] = abs(d[i - 1] - X[i - 1]);
		
		
		//output(i);


	}

	F[N-1] = abs(d[N - 1] - X[N - 1]);
	int max=0;
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

int Perebor(int k, int* p, int N)
{
	static int min1 = INT_MAX;
	int ty, m;
	{
		if (k == N)//полная перестановка
		{
			for (int i = 0; i < N; i++)
				cout << p[i];
			m = Raspis(p, N);//строим расписание. Написать отдельную функцию 
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
	for (int j = k; j < N; j++)
	{
		swap(p[k], p[j]);//переставили местами
		Perebor(k + 1, p, N);//уменьшили хвост
		swap(p[k], p[j]);
	}
	return min1;//возвращаем минимальное значение критерия
}

int hamingDist(int* ar, int* D)
{
    int hamingCount = 0;
    for (int i = 0; i < N; i++) {
		/*cout << ar[i]<<"-";
		cout << D[i];*/

        if (ar[i] != D[i]) {
            hamingCount++;
        }
    }
	//cout << hamingCount << " ";
    return hamingCount;
}

int** okr(int n, int* D, int** okrs)
{
    
    cout << " окрестность: ";
	int* ar = new int[n];
	for (int i = 0; i < n; ++i) {
		ar[i] = D[i]; //cout << ar[i];
	}
	//cout << endl;
	do
	{
		
		if (hamingDist(ar, D) == 2)
		{
			//cout << "-";
			//for (int k = 0; k < m; k++) {
				for (int j = 0; j < N; j++) {
					okrs[nokr][j] = ar[j];
					cout << ar[j];
				}
				cout << endl;
			//}
				
			nokr++;
		}
		//cout << "+";
	} while (next_permutation(ar, ar + n)&& nokr!=N);//не все перестановки,только ближайшая,выбрать точку

	
    cout << endl;
   // struct result { int** okrs; int nokr; };
	return okrs;//result{ okrs,m };

}

int hill(int* p)
{
	
	int i = 0,n=N;
    unsigned int maxX, max = 0, x0, x = 0,x1;
	int* D;
	D = new int[N];
	int* maxO = new int[N];
	for (int j = 0; j < N; j++) //кодировки
	{

		D[j] = p[j];
		
		cout << " " << p[j];
	}
	cout << endl;
    srand(time(NULL));

    x0 = rand() % (N);
    //x0 = D[x0];
    cout << " нач maxX " << x0 << endl;
    maxX = x0;
    x =Raspis(D,N);
    cout << "приспособленность x-" << x << endl;
    max = x;
    int** okrs = new int* [40];
		for (int j = 0; j < N; j++)
		{
			okrs[j] = new int[N];
		}
	
    int z = 0, m = 0;
	int* per = new int[N];
	
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
		cout << " maxX-" << maxX << endl;
		cout << " max-" << max << endl;
		
		//auto res = okr(N, D, okrs);//почему тригерит maxO ???
		okr(N, D, okrs);
		
        for (int k = 0; k < nokr; k++)//поиск макс окр
        {
						
			for (int j = 0; j < N; j++)
			{

				per[j] = okrs[k][j];
			}
			// x1 = 0;
		
			x1 = Raspis(per, N);
			
			cout << "выбр перестановка ";
			for (int j = 0; j < N; j++)
			{

				cout << per[j];
			}
			cout << endl;
			cout << "ее приспособленность=" << x1 << endl;
		
            if (x1 < max)
            {
                maxO = okrs[k];
               
                cout << "нов макс окр ="<< endl;
				for (int j = 0; j < N; j++)
				{

					cout << maxO[j];
				}
				cout << endl;
                max = x1;

            }
		
			cout << endl;
        }

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
		maxX = rand() % (N);
      //  x = x1;

       // cout << "приспособленность x-" << x << endl;
        // cout << "код. x0-" << x0 << endl;
      /*  if (x > max)
        {
            max = x;
            maxX = maxO;
         

            cout << "новый maxX-" << maxX << endl;
            cout << "новый max-" << max << endl;
        }
        else { break; }
		*/
		
    }
	for (int i = 0; i < N; i++) {
		delete[] okrs[i];
	}
	delete[] okrs;
	cout << " финальный штраф " << x << endl;;
    return x;
}



/*
int hill(int* p, int start)
{
	int next = start;
	int current;
	do {
		current = next;
		for (int i = 0; i < res.m;i++)
		{
			if (Problem.EVAL(x) > problem.EVAL(next))
				next = x;
		}
	} while (current != next)
	return next;
}*/
int main()
{

	setlocale(LC_ALL, "RUS");
	srand(time(NULL));
	cout << "введите количество работ" << endl;
	cin >> N;
	int* p = new int[N];
	for (int i = 0; i < N; i++)p[i] = i+1;
	initRaspis(p, N);
	cout << "метод восхождения на холм " << endl;
	int o = hill(p);
	Best = new int[N];
	int M = Perebor(0, p, N);
	cout << " Точное решение " << M << endl;
	for (int j = 0; j < N; j++)
	{
		cout << Best[j];
	}
	o = ((o - M) / M) * 100 ;
	cout << endl;
	cout << "Относительное отклонение ="<<o<<"%";
	double ab = (o - M) / N;
	cout << "Абсолютное отклонение =" << ab;
	//delete[] X;
	//delete[] Y;
	//delete[] T;
	return 0;
}
