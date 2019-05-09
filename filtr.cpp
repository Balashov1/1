#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
using namespace std;

struct bits
{
	unsigned int kiril : 1;
	unsigned int latyn : 1;
	unsigned int punctuation : 1;
	unsigned int numb : 1;
}filter;

void viewing_of_filter(bits filter);

int main()
{
	char string[255], simbol_filter[30];
	char choice;
	char standard[] = ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя.,:;-");
	int p, length_line, length_simb, temp, w, y;
	unsigned int n, m;
	m = strlen(standard);
	SetConsoleOutputCP(1251);
	cout << "Введите текст: ";
	gets_s(string);
	length_line = strlen(string);
	do
	{
		cout << "\nХотите просмотреть и изменить фильтры";
			cout << "\ny-Да n-Нет ";
			cin >> choice;
		while ((choice != 'y') && (choice != 'n'))
		{
			cout << "\nВыбор сделан неправильно пожалуйста повторите";
				cin >> choice;
		}
		if (choice == 'y')
		{
			viewing_of_filter(filter);
			cout << "\n1-Изменяет фильтр кирилицы";
				cout << "\n2-Изменяет фильтр латиницы";
				cout << "\n3-Изменяет фильтр для знаков пунктуации";
				cout << "\n4-Изменяет фильтр для цифр";
				cout << "\n5-Ничего не менять" << endl;
				cin >> p;
			switch (p)
			{
			case 1:
			{
				n = filter.kiril ^ 1;
				filter.kiril = n;
			}
			break;
			case 2:
			{
				n = filter.latyn ^ 1;
				filter.latyn = n;
			}
			break;
			case 3:
			{
				n = filter.punctuation ^ 1;
				filter.punctuation = n;
			}
			break;
			case 4:
			{
				n = filter.numb ^ 1;
				filter.numb = n;
			}
			break;
			case 5: continue;
			}
			viewing_of_filter(filter);
		}
		gets_s(simbol_filter);
		cout << "\nВведите символы которые необходимо отфильтровать ";
		gets_s(simbol_filter);
		length_simb = strlen(simbol_filter);
		// определяю к чему относятся введённые символы сравнивая их с эталоном
		for (int x = 0; x <= length_simb; x++)
		{
			for (int q = 0; q <= m; q++)
			{
				if (simbol_filter[x] == standard[q])
				{
					if (q < 10)//Если это цифра
					{
						if (filter.numb == 1)//проверяю наличие разрешающего флага
						{
							for (int m = 0; m <= length_line; m++)//далеаю замену символа
							{
								if (simbol_filter[x] == string[m])
								{
									string[m] = ' ';
								}
							}
						}
						else break;
					}
					if ((q > 9) && (q < 62))//Если это латиница
					{
						if (filter.latyn == 1)//проверяю наличие разрешающего флага
						{
							for (int m = 0; m <= length_line; m++)//делаю замену символа
							{
								if (simbol_filter[x] == string[m])
								{
									string[m] = ' ';
								}
							}
						}
						else break;
					}
					if ((q > 61) && (q < 126))//Если это кирилица
					{
						if (filter.kiril == 1)//проверяю наличие разрешающего флага
						{
							for (int m = 0; m <= length_line; m++)//делаю замену символа
							{
								if (simbol_filter[x] == string[m]) string[m] = ' ';
							}
						}
						else break;
					}
					if (q > 125)//если знаки пунктуации
					{
						if (filter.punctuation == 1)//проверяю наличие разрешающего флага
						{
							for (int m = 0; m <= length_line; m++)//проверяю замену символа
							{
								if (simbol_filter[x] == string[m]) string[m] = ' ';
							}
						}
						else break;
					}
				}
			}
		}
		//вывожу результат
		cout << string << "";
			cout << "\nХотите продолжить";
			cout << "\ny-Да n-Нет";
			cin >> choice;
		while ((choice != 'y') && (choice != 'n'))
		{
			cout << "Выбор сделан неправильно пожалуйста повторите";
				cin >> choice;
		}
	} while (choice != 'n');
	_getch();
}
void viewing_of_filter(bits filter)
{
	cout << "В данный момент фильтры стоят на:\n";
		cout << "Знаки кирилицы: ";
	if (filter.kiril == 0) cout << " Нет";
	else cout << " Да";
		cout << "\nЗнаки латиницы: ";
	if (filter.latyn == 0) cout << " Нет";
	else cout << " Да";
		cout << "\nЗнаки пунктуации: ";
	if (filter.punctuation == 0) cout << " Нет";
	else cout << " Да";
		cout << "\nЦифры: ";
	if (filter.numb == 0) cout << " Нет";
	else cout << " Да";
}