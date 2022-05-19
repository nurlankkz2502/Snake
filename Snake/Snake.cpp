#include <iostream> 
#include <cstdio> 
#include <cstdlib> 
#include <ctime> 
#include <windows.h> 
#include <fstream> 
#include <string> 
// Библиотека, нужна для использования функций kbhit() и getch(). 
#include <conio.h> 

// snake_size - размер змейки. 
// change_x, change_y - в какую сторону движется змейка. 
// coordinates_x[1000], coordinates_y[1000] - массивы, хранящие координаты 
//                                            частей тела змейки. 
// Kоординаты головы змейки хранятся в coordinates_x[1], coordinates_y[1]. 
// food_x, food_y - координаты еды. 

int snake_size, change_x, change_y, coordinates_x[1000], coordinates_y[1000];
int food_x = -1, food_y = -1;
int count = 0;
int score = 0;
int level = 1;
int high_score = 0;

// symbol - хранит в себе ASCII код нажатой клавиши. 
// a[1000][1000] - наша таблица, в которой происходит вся игра. 
char symbol, a[1000][1000];


// Константы: 
// N - размер таблицы, а именно высота. 
// M - ширина таблицы. 
// INTERVAL - интервал в миллисекундах, через каждый этот промежуток 
//            времени змейка будет передвигаться. 
const int N = 13, M = 25;
int INTERVAL = 200;
// функция, считывающая нажатую клавишу. 
void change_direction() {
	// Cчитываем нажатую клавишу с помощью функции getch(). 
	symbol = _getch();
	switch (symbol) {
		// Управление змейкой у нас через wasd. 
	case 'w': if (change_x != 1 || change_y != 0) {
		change_x = -1; change_y = 0;
	}
			break;
	case 'a': if (change_x != 0 || change_y != 1) {
		change_x = 0; change_y = -1;
	}
			break;
	case 's': if (change_x != -1 || change_y != 0) {
		change_x = 1; change_y = 0;
	}
			break;
	case 'd': if (change_x != 0 || change_y != -1) {
		change_x = 0; change_y = 1;
	}
			break;
	default: break;
	}
}


// функция для вывода таблицы 
void show_table() {
	// Очищаем консоль. 
	system("cls");
	// Выводим таблицу и края. 
	for (int i = 0; i <= N + 1; ++i)
		for (int j = 0; j <= M + 1; ++j)
			std::cout << (i == 0 || j == 0 || i == N + 1 || j == M + 1 ? '#' : a[i][j]) << (j <= M ? "" : "\n");

}


// Очищаем координаты, в которых располагалась змейка. 
void clear_snake_on_table() {
	for (int i = 1; i <= snake_size; ++i)
		a[coordinates_x[i]][coordinates_y[i]] = ' ';
}

// Красим координаты змейки. 
void show_snake_on_table() {
	// Изменяем тип головы. 
	if (change_x == 1 && change_y == 0)
		a[coordinates_x[1]][coordinates_y[1]] = 'v';
	if (change_x == -1 && change_y == 0)
		a[coordinates_x[1]][coordinates_y[1]] = '^';
	if (change_x == 0 && change_y == 1)
		a[coordinates_x[1]][coordinates_y[1]] = '>';
	if (change_x == 0 && change_y == -1)
		a[coordinates_x[1]][coordinates_y[1]] = '<';

	// Красим змейку. 
	for (int i = 2; i <= snake_size; ++i)
		a[coordinates_x[i]][coordinates_y[i]] = '@';
}

// Проверяем, съела ли змейка саму себя. 
bool game_over() {
	for (int i = 2; i <= snake_size; ++i)
		// Eсли координаты головы змейки равны координате какой-либо части тела 
		// змейки, то змейка съела саму себя. 
		if (coordinates_x[1] == coordinates_x[i] &&
			coordinates_y[1] == coordinates_y[i])
			return true;
	// Если все координаты различны, то все в порядке - играем дальше. 
	return false;
}

// Проверяем, не вышла ли змейка за поле, если да то возвращаем ее обратно. 
void check_coordinates() {
	if (coordinates_x[1] > N) coordinates_x[1] = 1;
	if (coordinates_x[1] < 1) coordinates_x[1] = N;
	if (coordinates_y[1] > M) coordinates_y[1] = 1;
	if (coordinates_y[1] < 1) coordinates_y[1] = M;
}

// функция следующего хода, в которой наша змейка сдвигается в сторону 
// на 1 ячейку. 
void next_step() {
	// Чистим таблицу от змейки. 
	clear_snake_on_table();

	// Передвигаем тело змейки. 
	for (int i = snake_size; i >= 2; --i) {
		coordinates_x[i] = coordinates_x[i - 1];
		coordinates_y[i] = coordinates_y[i - 1];
	}

	// Передвигаем голову змейки. 
	coordinates_x[1] += change_x;
	coordinates_y[1] += change_y;

	// Проверяем в порядке ли координаты. 
	check_coordinates();

	// Если голова змейки там же где и еда, то увеличиваем размер змейки
		// и очищаем координаты змейки. 
	if (coordinates_x[1] == food_x && coordinates_y[1] == food_y) {
		snake_size++;
		count++;
		score = score + level;
		food_x = -1;
		food_y = -1;
	}

	// Рисуем змейку. 
	show_snake_on_table();

	// Если змея укусила себя. 
	if (game_over()) {
		// Cообщаем всю правду о игроке. 
		std::cout << "You're looser! \n";
		// Приостанавливаем игру. 
		system("pause");
	}
}

// функция проверки на наличие еды на карте. 
bool food_check() {
	// Если координаты еды неопределенны, то возвращаем true. 
	if (food_x == -1 && food_y == -1) return false;
	// В остальных случаях false. 
	return true;
}

// функция добавления еды на карту. 
void place_food() {
	std::srand(std::time(NULL));
	// Cтавим в рандомное место еду. 
	for (int i = 1; i <= 9; ++i) {
		int x = std::rand(), y = std::rand();
		if (x < 0) x *= -1;
		if (y < 0) y *= -1;
		x %= (N + 1);
		y %= (M + 1);
		if (x == 0) ++x;
		if (y == 0) ++y;
		if (a[x][y] != '@' && a[x][y] != 'v' && a[x][y] != '^' &&
			a[x][y] != '<' && a[x][y] != '>') {
			food_x = x;
			food_y = y;
			a[x][y] = '+';
			return;
		}
	}
}
//Считываем high score в файл 
void read_high_score()
{
	std::fstream fin;

	fin.open("data.txt");

	if (!fin.is_open())
	{
		std::cout << "Data open error\n";
	}
	else
	{
		std::string str;

		fin >> str;
		char st[10];
		size_t j = 0;

		for (size_t i = 0; i < str.size(); i++)
		{
			st[j] = str[i];
			j++;
		}
		st[j] = '\0';
		high_score = atoi(st);

	}
	fin.close();
}
//записываем high score в файл 
void save_high_score()
{
	std::ofstream fout;

	fout.open("data.txt");

	if (!fout.is_open())
	{
		std::cout << "Data open error\n";
	}
	else
	{
		fout << high_score;
	}
	fout.close();
}
// Начальные установки. 
void standart_settings() {
	// Размер змеи - 2. 
	snake_size = 2;

	// Змейка занимает две клетки вправо от координаты {1,1}. 
	coordinates_x[1] = 1;
	coordinates_y[1] = 1;
	coordinates_x[2] = 1;
	coordinates_y[2] = 1;

	// Змейка движется вправо. 
	change_x = 0;
	change_y = 1;

	//заполняем таблицу пробелами 
	for (size_t i = 0; i < 1000; i++)
		for (size_t j = 0; j < 1000; j++)
			a[i][j] = ' ';
	read_high_score();
}

int main() {




	// Задаем стандартные настройки. 
	standart_settings();

	// Бесконечный цикл. 
	while (true) {
		// Если нажата клавиша, обрабатываем нажатую клавишу. 
		if (_kbhit() != 0)
			change_direction();

		// Двигаем змейку. 
		next_step();

		// Если нет еды, то ставим ее. 
		if (!food_check())
			place_food();

		// Рисуем змейку. 
		show_table();

		//Меняем интервал времени и количство набранных очков 
		if ((count == 5) && (count != 0))
		{
			if (INTERVAL == 50)
			{
				INTERVAL = 25;
				count = 0;
				level++;
				continue;
			}
			if (INTERVAL == 25)
			{
				count = 0;
				level;
				continue;
			}
			INTERVAL -= 50;
			level++;
			count = 0;
		}
		if (score > high_score)
		{
			high_score = score;
		}
		std::cout << "\nScore: " << score << std::endl;
		std::cout << "\nHigh Score: " << high_score << std::endl;
		// "Усыпляем" программу на заданный интервал. 
		Sleep(INTERVAL);
		if (game_over())
			break;
	}

	if (score == high_score)
	{
		high_score = score;
		save_high_score();
	}
}