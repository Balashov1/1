#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <time.h>
#include <clocale>
#include <stdio.h>
using namespace std;

enum TEXTCOLOR
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	BLUE = FOREGROUND_BLUE,
	GREEN = FOREGROUND_GREEN,
	WHITE = RED | BLUE | GREEN,
	YELLOW = GREEN | RED,
	PURPURE = RED | BLUE,
	INTENSITY = FOREGROUND_INTENSITY
};

inline void textcolor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int first_shot();
//функции проверки и сканеры
int wrong_detect(char**, int, int, int, int);
int end_wrong_detect(char**, int, int, int, int, int);
int begin_wrong_detect(char**, int, int, int);
int scan_deck(char**, int, int, int, int);
//Генераторы чисел
int gen_vert_horiz(int);
int generator();
//Компьютер
int computer_pozition(char**);
int set_computer_pozition(char**, int);
int computer_shot(char**, char**, char**, int, int&, int&, int&, int&);
//Отображение полей
void show_squares(char**, char**, int);
//Функции игрока
int shot_player(char**);
int set_pozition(char**, char**, char**, int, int);
int pozition(char**, char**, char**);
//Игра
int game(char**, char**);
int win(int, int);

int main()
{
	setlocale(0, "");
	system("cls");
	srand((unsigned short int)time(NULL));

	char** square_1 = new char*[10];
	char** square_2 = new char*[10];

	for (int i = 0; i < 10; i++)
	{
		square_1[i] = new char[10];
		square_2[i] = new char[10];
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			square_1[i][j] = '_';
			square_2[i][j] = '_';
		}
	}

	game(square_1, square_2);
	system("cls");

	return 0;
}

int game(char** square_1, char** square_2)
{
	unsigned short int first_atack = 0, i, game_mode;
	unsigned short int count_1 = 20, count_2 = 20;    // число кораблей в каждом поле
	int count_contact = 0, miss_count_1 = 0, miss_count_2 = 0; // счетчики: попаданий, промахов - 1-го и 2-игрока
	int x_save_1 = 0, y_save_1 = 0, flag_1 = 0,  //переменные 1-го компьютера
		x_save_2 = 0, y_save_2 = 0, flag_2 = 0;  //переменные 2-го компьютера

	cout << "Вас приветствует игра Морской Бой. Автор Балашов В.В.\n";
	cout << "Выберите режим игры: \n";
	cout << "1. Игрок - компьютер. \n";
	cout << "2. Компьютер - компьютер. \n";
	cout << "3. Выход. \n";

	cout << "Режим: ";
	cin >> game_mode;

	if (game_mode > 0 && game_mode < 4)
	{
		switch (game_mode)
		{
		case 1:
			cout << "Игра с компьютером.\n";
			Sleep(2000);
			system("cls");

			cout << "Расставить корабли автоматически или вручную?(0 - вручную, 1 - автоматически)\n";
			cin >> i;

			system("cls");

			if (!i)  //Ручная или авторасстановка
			{
				cout << "Расставьте корабли.\n";
				Sleep(1000);
				pozition(square_1, square_1, square_2);
			}
			else
			{
				cout << "Автоматическая расстановка.\n";
				Sleep(1000);
				computer_pozition(square_1);
			}

			show_squares(square_1, square_2, 1);

			cout << "Расставляет компьютер. \n";
			Sleep(3000);

			computer_pozition(square_2);  // растановка на поле компьютера.
			show_squares(square_1, square_1, 1);
			Sleep(3000);

			first_atack = first_shot(); // выбор хода

			while ((count_1 > 0) && (count_2 > 0)) // пока один из счетчиков палуб больше 0.
			{
				if (first_atack == 1) // если 1, атакует игрок
				{
					unsigned short int i = 0; //индикатор промаха

					while (i < 1) // пока нет промаха
					{
						if (shot_player(square_2) == 1)
						{
							count_2--;
							count_contact++;
						}
						else
						{
							count_contact = 0;
							i++; // инкрементируя, выходим из цикла.
						}
					}

					show_squares(square_1, square_2, 1);
					count_contact = 0;
					first_atack = 2; // передача права хода
				}
				else
				{
					unsigned short int i = 0; // индикатор промаха

					while (i < 1) // пока нет промаха
					{
						if ((count_contact = computer_shot(square_1, square_1, square_2, game_mode, x_save_1, y_save_1, flag_1, miss_count_1)) > 0)
						{
							count_1 = count_1 - count_contact;
							show_squares(square_1, square_2, 1);
							i++;
						}
						else
						{
							count_contact = 0;
							i++; // инкрементируя, выходим из цикла.
						}
					}

					count_contact = 0;
					first_atack = 1; // передача права хода
				}
			}

			win(count_2, count_1);
		case 2:
			cout << "Компьтютер против компьютера.";
			system("cls");

			computer_pozition(square_1);
			computer_pozition(square_2);
			Sleep(3000);

			first_atack = first_shot();
			system("cls");

			while ((count_1 > 0) && (count_2 > 0))
			{
				if (first_atack == 1)
				{
					unsigned short int i = 0;

					while (i != 1)
					{
						if ((count_contact = computer_shot(square_2, square_1, square_2, game_mode, x_save_1, y_save_1, flag_1, miss_count_1)) > 0)
						{
							count_2 = count_2 - count_contact;
							show_squares(square_1, square_2, 2);
							i++;
							Sleep(1000);
						}
						else
						{
							count_contact = 0;
							i++; // инкрементируя, выходим из цикла.
							Sleep(1000);
						}
					}

					count_contact = 0;
					first_atack = 2;
					//system("pause");
				}
				else
				{
					unsigned short int i = 0;

					while (i != 1)
					{
						if ((count_contact = computer_shot(square_1, square_1, square_2, game_mode, x_save_2, y_save_2, flag_2, miss_count_2)) > 0)
						{
							count_1 = count_1 - count_contact;
							show_squares(square_1, square_2, 2);
							i++;
							Sleep(1000);
						}
						else
						{
							count_contact = 0;
							i++; // инкрементируя, выходим из цикла.
							Sleep(1000);
						}
					}

					count_contact = 0;
					first_atack = 1;
					//system("pause");
				}
			}

			show_squares(square_1, square_2, game_mode);

			win(count_2, count_1);
		case 3:
			cout << "Выход";
			Sleep(3000);
			exit(1);
		}
	}
	else
	{
		cout << "Такого режима не существует, запустите программу заново.";
		Sleep(3000);
		exit(1);
	}

	exit(1);

	system("pause");

	return 0;
}

int first_shot()
{
	unsigned short int first;

	while (1)
	{
		cout << "Кто ходит первым? (1 - игрок, 2 - второй компьютер/2 игрок): ";
		cin >> first;

		switch (first)
		{
		case 1:
			return 1;
		case 2:
			return 2;
		default:
			cout << "Попробуйте еще раз.\n";
		}

	}

	return 0;
}

int pozition(char** square, char** square_1, char** square_2)
{
	for (int i = 0; i < 1; i++)
	{
		set_pozition(square_1, square_1, square_2, 4, 1);
	}

	for (int i = 0; i < 2; i++)
	{
		set_pozition(square_1, square_1, square_2, 3, 2);
	}

	for (int i = 0; i < 3; i++)
	{
		set_pozition(square_1, square_1, square_2, 2, 3);
	}

	for (int i = 0; i < 4; i++)
	{
		set_pozition(square_1, square_1, square_2, 1, 4);
	}

	return 0;
}

int set_pozition(char** square, char** square_1, char** square_2, int deck, int ship)
{
	int vert_horiz, x, y;

	for (int i = 0; i < 1;)
	{
		switch (ship)
		{
		case 1:
			cout << "Введите коодинаты четырехпалубника: \n";
			break;
		case 2:
			cout << "Введите координаты трехпалубника: \n";
			break;
		case 3:
			cout << "Введите координаты двухпалубника: \n";
			break;
		case 4:
			cout << "Введите координты однопалубника: \n";
			break;
		}

		show_squares(square_1, square_2, 1);
		cout << "Горизонтально или вертикально?(1 - вертикально, 2 - горизонтально): ";
		cin >> vert_horiz;
		cout << "Начало координат корабля: \n";
		cout << "X равно: \n";
		cin >> y;
		cout << "Y равно: \n";
		cin >> x;

		x = x - 1;
		y = y - 1;

		if (((x > -1) && (y > -1)) && ((x < 10) && (y < 10)))
		{
			switch (vert_horiz)
			{
			case 1:  // вертикальное построение
				if ((wrong_detect(square, vert_horiz, x, y, deck)) == 0)
				{
					for (int n = 0; n < 1; n++)
					{
						for (int m = 0; m < deck; m++)
						{
							if ((x + m < 10) && (y < 10))
							{
								square[x + m][y] = 'O';
							}
							else
							{
								cout << "Попробуйте заново. \n";
							}
						}
					}
					i++;
					break;
				}
				else
				{
					cout << "Попробуйте заново.\n";
					Sleep(3000);
				}
			case 2:  // горизонтальное построение
				if ((wrong_detect(square, vert_horiz, x, y, deck)) == 0)
				{
					for (int n = 0; n < 1; n++)
					{
						for (int m = 0; m < deck; m++)
						{
							if ((y + m < 10) && (x < 10))
							{
								square[x][y + m] = 'O';
							}
							else
							{
								cout << "Попробуйте заново. \n";
							}

						}
					}
					i++;
					break;
				}
				else
				{
					cout << "Попробуйте заново.\n";
					Sleep(3000);
				}
			}
		}
		else
		{
			cout << "Неверные данные\n";
			Sleep(3000);
		}
	}
	return 0;
}

int wrong_detect(char** square, int vert_horiz, int x, int y, int decks)
{
	if (((x + decks < 10) && (y + decks < 10)) && ((!(x < 0)) && (!(x > 10)) && (!(y < 0)) && (!(y > 10))))
	{
		switch (vert_horiz)
		{
		case 1:              // вертикальная проверка
			if (begin_wrong_detect(square, vert_horiz, x, y) == 0)
			{
				for (int i = 0; i < 1; i++)
				{
					for (int j = 0; j < decks; j++)
					{
						if (!(((((x - 1 > -1) && (x - 1 < 10)) && ((y + j < 10) && (y + j > -1))) && (square[x - 1][y + j] == 'O'))
							|| ((((x + 1 < 10) && (x + 1 > -1)) && ((y + j < 10) && (y + j > -1))) && (square[x + 1][y + j] == 'O'))))
						{
							if (end_wrong_detect(square, vert_horiz, x, y, i, j))
								return 1; //проверка не пройдена
						}
						else
						{
							return 1; //проверка не пройдена
						}
					}
				}
			}
			else
			{
				return 1; //проверка не пройдена
			}

		case 2:                 // горизонтальная проверка
			if (begin_wrong_detect(square, vert_horiz, x, y) == 0)
			{
				for (int j = 0; j < 1; j++)
				{
					for (int i = 0; i < decks; i++)
					{
						if (!(((((x + i < 10) && (x + i > -1)) && ((y + 1 < 10) && (y + 1 > -1))) && (square[x + i][y + 1] == 'O'))
							|| ((((x + i < 10) && (x + i > -1)) && ((y - 1 > -1) && (y - 1 < 10))) && (square[x + i][y - 1] == 'O'))))
						{
							if (end_wrong_detect(square, vert_horiz, x, y, i, j))
								return 1;
						}
						else
						{
							return 1;
						}
					}
				}

			}
			else
			{
				return 1;
			}

		}
	}
	else
	{
		return 1;
	}

	return 0;
}

int begin_wrong_detect(char** square, int vert_horiz, int x, int y)
{
	switch (vert_horiz)
	{
	case 1:
		if (((((x > -1) && (x < 10)) && ((y - 1 > -1) && (y - 1 < 10))) && (square[x][y - 1] == 'O')) ||
			((((x - 1 > -1) && (x - 1 < 10)) && ((y - 1 > -1) && (y - 1 < 10))) && (square[x - 1][y - 1] == 'O')) ||
			(((((x + 1 < 10) && (x + 1 > -1)) && ((y - 1 > -1) && (y - 1 < 10)))) && (square[x + 1][y - 1] == 'O')))
			return 1; // проверка не пройдена
	case 2:
		if (((((x - 1 > -1) && (x - 1 < 10)) && ((y + 1 < 10) && (y + 1 > -1))) && (square[x - 1][y + 1] == 'O')) ||
			((((x - 1 > -1) && (x - 1 < 10)) && ((y - 1 > -1) && (y - 1 < 1))) && (square[x - 1][y - 1] == 'O')) ||
			((((x - 1 > -1) && (x - 1 < 10)) && ((y > -1) && (y < 10))) && (square[x - 1][y] == 'O')))
			return 1; // проверка не пройдена
	};

	return 0; // проверка пройдена
}

int end_wrong_detect(char** square, int vert_horiz, int x, int y, int i, int j)
{
	switch (vert_horiz)
	{
	case 1:
		if (((((x < 10) && (x > -1)) && ((y + j < 10) && (y + j > -1))) && (square[x][y + j] == 'O')) ||
			((((x - 1 > -1) && (x - 1 < 10)) && ((y + j + 1 < 10) && (y + j + 1 > -1))) && (square[x - 1][y + j + 1] == 'O')) ||
			((((x + 1 < 10) && (x + 1 > -1)) && ((y + j + 1 < 10) && (y + j + 1 < 10))) && (square[x + 1][y + j + 1] == 'O')) ||
			((((x < 10) && (x > -1)) && ((y + j + 1 < 10) && (y + j + 1 > -1))) && (square[x][y + j + 1] == 'O')))
			return 1; //проверка не пройдена
	case 2:
		if (((((x + i < 10) && (x + i > -1)) && (y > -1) && (y < 10)) && (square[x + i][y] == 'O')) ||
			((((x + i + 1 < 10) && (x + i + 1 > -1)) && ((y + 1 < 10) && (y + 1 > -1))) && (square[x + i + 1][y + 1] == 'O')) ||
			((((x + i + 1 < 10) && (x + i + 1 > -1)) && ((y - 1 > -1) && (y - 1 < 10))) && (square[x + i + 1][y - 1] == 'O')) ||
			((((x + i + 1 < 10) && (x + i + 1 > -1)) && ((y < 10) && (y > -1))) && (square[x + i + 1][y] == 'O')))
			return 1; //проверка не пройдена
	};

	return 0; //проверка пройдена
}

void show_squares(char** square_1, char** square_2, int game_mode)
{
	system("cls");
	char letters[10] = { 'А','Б','В','Г','Д','Е','Ж','З','И','К' };

	cout << "Первое поле.\n";
	textcolor(TEXTCOLOR::YELLOW | TEXTCOLOR::INTENSITY);
	cout << "  1 2 3 4 5 6 7 8 9 10 \n";
	textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
	cout << " ---------------------\n";

	for (int i = 0; i < 10; i++)
	{
		textcolor(TEXTCOLOR::YELLOW | TEXTCOLOR::INTENSITY);
		cout << letters[i];
		textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);

		for (int j = 0; j < 10; j++)
		{
			cout << "|";

			if (square_1[i][j] == 'O')
			{
				textcolor(TEXTCOLOR::GREEN | TEXTCOLOR::INTENSITY);
				cout << square_1[i][j];
				textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
			}

			if (square_1[i][j] == 'X')
			{
				textcolor(TEXTCOLOR::RED | TEXTCOLOR::INTENSITY);
				cout << square_1[i][j];
				textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
			}

			if (square_1[i][j] == 'o')
			{
				textcolor(TEXTCOLOR::BLUE | TEXTCOLOR::INTENSITY);
				cout << square_1[i][j];
				textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
			}

			if (square_1[i][j] == '_')
			{
				cout << square_1[i][j];
			}
		}

		cout << "|";
		cout << "\n";
	}

	cout << " ---------------------\n";

	cout << "\nВторое поле.\n";
	textcolor(TEXTCOLOR::YELLOW | TEXTCOLOR::INTENSITY);
	cout << "  1 2 3 4 5 6 7 8 9 10 \n";
	textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
	cout << " ---------------------\n";

	for (int i = 0; i < 10; i++)
	{
		textcolor(TEXTCOLOR::YELLOW | TEXTCOLOR::INTENSITY);
		cout << letters[i];
		textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);

		cout << "|";

		for (int j = 0; j < 10; j++)
		{
			if (square_2[i][j] == 'O')
			{
				if (game_mode == 1)
				{
					cout << "|" << '_';
				}
				else
				{
					textcolor(TEXTCOLOR::GREEN | TEXTCOLOR::INTENSITY);
					cout << square_2[i][j];
					textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
				}
			}

			if (square_2[i][j] == 'X')
			{
				textcolor(TEXTCOLOR::RED | TEXTCOLOR::INTENSITY);
				cout << square_2[i][j];
				textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
			}

			if (square_2[i][j] == 'o')
			{
				textcolor(TEXTCOLOR::BLUE | TEXTCOLOR::INTENSITY);
				cout << square_2[i][j];
				textcolor(TEXTCOLOR::WHITE | TEXTCOLOR::INTENSITY);
			}

			if (square_2[i][j] == '_')
			{
				cout << square_2[i][j];
			}
			cout << "|";
		}
		cout << "\n";
	}

	cout << " ---------------------\n";

	Sleep(1000);
}

int generator()
{
	unsigned short int n = 0;

	n = rand() % 10;

	return n;
}

int gen_vert_horiz(int vert_horiz)
{
	vert_horiz = 0 + rand() % 3;

	return vert_horiz;
}

int shot_player(char** square)
{
	unsigned short int x, y;

	while (1)
	{
		cout << "Координаты выстрела: \n";
		cout << "горизонтальная - ";
		cin >> y;
		cout << "вертикальная - ";
		cin >> x;

		x = x - 1;
		y = y - 1;

		if ((x > -1) && (y > -1))
		{
			switch (square[x][y])
			{
			case 'O':
				square[x][y] = 'X';
				cout << "Попадание.\n";
				Sleep(3000);
				return 1;
			case 'X':
				cout << "Цель поражена, попробуйте еще раз.\n";
				Sleep(3000);
				break;
			case 'o':
				cout << "По этим координатам уже был промах.\n";
				Sleep(3000);
				break;
			case '_':
				square[x][y] = 'o';
				cout << "Промах.\n";
				Sleep(3000);
				return 2;
			}
		}
	}
}

int computer_shot(char** square, char** square_1, char** square_2, int game_mode, int& x_save, int& y_save, int& flag, int& miss_count)
{
	unsigned short int X = 0, Y = 0, vert_horiz = 0,
		var = 0, count_contact = 0, local_count_contact = 0, i = 0;

	while (1)
	{
		if (miss_count > 3)
		{
			flag = 0;
			miss_count = 0;
		}

		if (flag == 0) // Ранее попаданий не было
		{
			X = generator();  // Создание кординат выстрела
			Y = generator();
			x_save = X;       // Сохранение координат для возможного дальнейшео использования
			y_save = Y;
		}
		else // Иначе - присвоим координаты последнего попадания
		{
			X = x_save;
			Y = y_save;
		}

		if ((flag == 1) && (miss_count >= 0)) // Если было попадание, то следующий выстрел
		{
			vert_horiz = gen_vert_horiz(vert_horiz); // горизонтально или вертикально
			var = gen_vert_horiz(var); // вверх/вниз или влево/вправо
		}

		if (flag == 0) // если флаг попаданий 0, то стреляем заново
		{
			switch (square[X][Y])      // Перебор вариантов попадания
			{                         // Условие:
			case 'O':                 // Попадание
				square[X][Y] = 'X';
				show_squares(square_1, square_2, game_mode);
				flag++;
				count_contact++;

				i = 0;

				if (scan_deck(square, X, Y, i, 0) == 1)
				{
					cout << "\nКорабль убит.";
					Sleep(1000);
					flag = 0;
					return count_contact;
				}
				break;
			case '_':                 // Промах
				square[X][Y] = 'o';
				show_squares(square_1, square_2, game_mode);
				return count_contact;
			case 'X':
				break;
			case 'o':
				break;
			}
		}
		else // если больше 0
		{
			if (vert_horiz == 1) // по горизонтали
			{
				if (var == 1) // вправо
				{
					if (Y + 1 < 10)
					{
						switch (square[X][Y + 1])
						{
						case 'O':
							square[X][Y + 1] = 'X';
							show_squares(square_1, square_2, game_mode);
							flag++;
							Y++;
							count_contact++;

							x_save = X;
							y_save = Y;
							i = 1;

							switch (scan_deck(square, X, Y, i, count_contact))
							{
							case 0:
								break;
							case 1:
								cout << "\nКорабль убит.";
								Sleep(1000);
								flag = 0;
								return count_contact;
							case 2:
								if ((Y - count_contact > 0) && (square[X][Y - count_contact] == 'O'))
								{
									square[X][Y - count_contact] = 'X';

									if (count_contact > 2)
									{
										if (scan_deck(square, X, Y, 5, count_contact))
										{
											cout << "\nКорабль убит.";
											flag = 0;
											return count_contact;
										}
									}

									show_squares(square_1, square_2, game_mode);
									switch (scan_deck(square, X, Y, i, count_contact))
									{
									case 0:
										break;
									case 1:
										flag = 0;
										cout << "\nКорабль убит.";
										Sleep(1000);
										return count_contact;
									}
								}
								else
								{
									flag = 0;
									cout << "\nКорабль убит.";
									Sleep(1000);
									return count_contact;
								}
								break;
							}
							break;
						case '_':
							square[X][Y + 1] = 'o';
							show_squares(square_1, square_2, game_mode);
							miss_count++;
							return count_contact;
						case 'X':
							break;
						case 'o':
							break;
						}
					}
				}
				else // по вертикали
				{
					if (Y - 1 > -1) // влево
					{
						switch (square[X][Y - 1])
						{
						case 'O':
							square[X][Y - 1] = 'X';
							show_squares(square_1, square_2, game_mode);
							flag++;
							Y--;
							count_contact++;

							x_save = X;
							y_save = Y;
							i = 2;

							switch (scan_deck(square, X, Y, i, count_contact))
							{
							case 0:
								break;
							case 1:
								cout << "\nКорабль убит.";
								Sleep(1000);
								flag = 0;
								return count_contact;
							case 2:
								if ((Y + count_contact < 10) && (square[X][Y + count_contact] == 'O'))
								{
									square[X][Y + count_contact] = 'X';

									if (scan_deck(square, X, Y, 5, count_contact))
									{
										cout << "\nКорабль убит.";
										flag = 0;
										return count_contact;
									}

									show_squares(square_1, square_2, game_mode);
									switch (scan_deck(square, X, Y, i, count_contact))
									{
									case 0:
										break;
									case 1:
										flag = 0;
										cout << "\nКорабль убит.";
										Sleep(1000);
										return count_contact;
									}
								}
								else
								{
									flag = 0;
									cout << "\nКорабль убит.";
									Sleep(1000);
									return count_contact;
								}
								break;
							}

							break;
						case '_':
							square[X][Y - 1] = 'o';
							show_squares(square_1, square_2, game_mode);
							miss_count++;
							return count_contact;
						case 'X':
							break;
						case 'o':
							break;
						}
					}
				}
			}
			else
			{
				if (var == 1) // по вертикали
				{
					if (X + 1 < 10) // вниз
					{
						switch (square[X + 1][Y])
						{
						case 'O':
							square[X + 1][Y] = 'X';
							show_squares(square_1, square_2, game_mode);
							flag++;
							X++;
							count_contact++;

							x_save = X;
							y_save = Y;
							i = 3;

							switch (scan_deck(square, X, Y, i, count_contact))
							{
							case 0:
								break;
							case 1:
								cout << "\nКорабль убит.";
								Sleep(1000);
								flag = 0;
								return count_contact;
							case 2:
								if ((X - count_contact > 0) && (square[X - count_contact][Y] == 'O'))
								{
									square[X - count_contact][Y] = 'X';

									if (scan_deck(square, X, Y, 5, count_contact))
									{
										cout << "\nКорабль убит.";
										flag = 0;
										return count_contact;
									}

									show_squares(square_1, square_2, game_mode);
									switch (scan_deck(square, X, Y, i, count_contact))
									{
									case 0:
										break;
									case 1:
										flag = 0;
										cout << "\nКорабль убит.";
										Sleep(1000);
										return count_contact;
									}
								}
								else
								{
									flag = 0;
									cout << "\nКорабль убит.";
									Sleep(1000);
									return count_contact;
								}
								break;
							}

							break;
						case '_':
							square[X + 1][Y] = 'o';
							show_squares(square_1, square_2, game_mode);
							miss_count++;
							return count_contact;
						case 'X':
							break;
						case 'o':
							break;
						}
					}
				}
				else
				{
					if (X - 1 > -1) // вверх
					{
						switch (square[X - 1][Y])
						{
						case 'O':
							square[X - 1][Y] = 'X';
							show_squares(square_1, square_2, game_mode);
							flag++;
							X--;
							count_contact++;

							x_save = X;
							y_save = Y;
							i = 4;

							switch (scan_deck(square, X, Y, i, count_contact))
							{
							case 0:
								break;
							case 1:
								cout << "\nКорабль убит.";
								Sleep(1000);
								flag = 0;
								return count_contact;
							case 2:
								if ((X + count_contact < 10) && (square[X + count_contact][Y] == 'O'))
								{
									square[X + count_contact][Y] = 'X';

									if (scan_deck(square, X, Y, 5, count_contact))
									{
										cout << "\nКорабль убит.";
										flag = 0;
										return count_contact;
									}

									show_squares(square_1, square_2, game_mode);
									switch (scan_deck(square, X, Y, i, count_contact))
									{
									case 0:
										break;
									case 1:
										flag = 0;
										cout << "\nКорабль убит.";
										Sleep(1000);
										return count_contact;
									}
								}
								else
								{
									flag = 0;
									cout << "\nКорабль убит.";
									Sleep(1000);
									return count_contact;
								}
								break;
							}

							break;
						case '_':
							square[X - 1][Y] = 'o';
							show_squares(square_1, square_2, game_mode);
							miss_count++;
							return flag;
						case 'X':
							break;
						case 'o':
							break;
						}
					}
				}
			}
		}
	}
}

int computer_pozition(char** square)
{
	for (int i = 0; i < 1; i++) // 1 четырехпалубник
	{
		set_computer_pozition(square, 4);
	}

	for (int i = 0; i < 2; i++) // 2 трехпалубника
	{
		set_computer_pozition(square, 3);
	}

	for (int i = 0; i < 3; i++) // 3 двухпалуника
	{
		set_computer_pozition(square, 2);
	}

	for (int i = 0; i < 4; i++) // 4 однопалубника
	{
		set_computer_pozition(square, 1);
	}

	return 0;
}

int set_computer_pozition(char** square, int deck)
{
	unsigned short int X = 0, Y = 0, vert_horiz = 0;

	int j = 0;

	while (j != 1)
	{
		vert_horiz = gen_vert_horiz(vert_horiz);
		X = generator();
		Y = generator();

		if (((X > -1) && (Y > -1)) && ((X < 10) && (Y < 10)) && ((vert_horiz > 0) && (vert_horiz < 3)))
		{
			if (vert_horiz == 1 && (X > -1) && (Y > -1))
			{
				if ((wrong_detect(square, vert_horiz, X, Y, deck)) == 0)
				{
					for (unsigned short int n = 0; n < 1; n++)
						for (unsigned short int m = 0; m < deck; m++)
							if (square[X][Y + m] == '_')
								square[X][Y + m] = 'O';
					j++;
				}
			}

			if (vert_horiz == 2 && (X > -1) && (Y > -1))
			{
				if ((wrong_detect(square, vert_horiz, X, Y, deck)) == 0)
				{
					for (unsigned short int n = 0; n < 1; n++)
						for (unsigned short int m = 0; m < deck; m++)
							if ((X + m < 10) && (Y < 10) && (square[X + m][Y] == '_'))
								square[X + m][Y] = 'O';
					j++;
				}
			}
		}
	}

	return 0;
}

int win(int count_2, int count_1)
{
	if (count_2 == 0)
	{
		cout << "Победил первый игрок.\n";
		Sleep(3000);
	}

	if (count_1 == 0)
	{
		cout << "Победил второй игрок.\n";
		Sleep(3000);
	}

	system("pause");

	return 0;
}

int scan_deck(char** square, int x, int y, int i, int count_contact)
{
	switch (i)
	{
	case 0:
		if (
			(((y + 1) < 10) && (square[x][y + 1] == 'O')) ||
			(((y - 1) > 0) && (square[x][y - 1] == 'O')) ||
			(((x - 1) > 0) && (square[x - 1][y] == 'O')) ||
			(((x + 1) < 10) && (square[x + 1][y] == 'O'))
			)
		{
			return 0;   // корабль не убит
		}
		else
		{
			return 1;   // корабль убит
		}
	case 1:
		if (((y + 1) < 10) && (square[x][y + 1] == 'O'))
		{
			return 0;   // Корабль не убит
		}
		else
		{
			if (((y - count_contact) > 0) && (square[x][y - count_contact] == 'O'))
			{
				return 2; // Корабль не убит
			}
			else
			{
				return 1; // Корабль убит
			}
		}
	case 2:
		if (((y - 1) < 10) && (square[x][y - 1] == 'O'))
		{
			return 0; // Корабль не убит
		}
		else
		{
			if (((y + count_contact) < 10) && (square[x][y + count_contact] == 'O'))
			{
				return 2; // Корабль не убит
			}
			else
			{
				return 1; // Корабль убит
			}
		}
	case 3:
		if (((x + 1) < 10) && (square[x + 1][y] == 'O'))
		{
			return 0; // Корабль не убит
		}
		else
		{
			if (((x - count_contact) > 0) && (square[x - count_contact][y] == 'O'))
			{
				return 2; // Корабль не убит
			}
			else
			{
				return 1; // Корабль убит
			}
		}
	case 4:
		if (((x - 1) > 0) && (square[x - 1][y] == 'O'))
		{
			return 0; // Корабль не убит
		}
		else
		{
			if (((x + count_contact) < 10) && (square[x + count_contact][y] == 'O'))
			{
				return 2; // Корабль не убит
			}
			else
			{
				return 1; // Корабль убит
			}
		}
	case 5:
	{
		if (((y + 1) < 10) && (square[x][y + 1] == 'O'))
		{
			square[x][y + 1] = 'X';
			return 1;   // Корабль убит
		}

		if (((y - 1) > 0) && (square[x][y - 1] == 'O'))
		{
			square[x][y - 1] = 'X';
			return 1;   // Корабль убит
		}

		if (((x - 1) > 0) && (square[x - 1][y] == 'O'))
		{
			square[x][y + 1] = 'X';
			return 1;   // Корабль убит
		}

		if (((x + 1) < 10) && (square[x + 1][y] == 'O'))
		{
			square[x][y + 1] = 'X';
			return 1;   // Корабль убит
		}

		return 1;
	}
	}

	return 0;
}