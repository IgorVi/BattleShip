///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																											 //
//			ЧЕРЕВКО ИГОР ВИКТОРОВИЧ																			 //
//			12.11.2014 г.																					 //
//			ШАГ		1 семестр																				 //
//			Email: igor.viktorovich.ua@gmail.com															 //
//																											 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "function.h"
using namespace std;
//*************************************************************************************************************
#define _width 78
#define _height 38
#define TITLE 3									// Nstr. Название
#define TOP_MESSAGE 5							// Nstr. ВЕРХНЕЕ СООБЩЕНИЕ
#define UPPER_EDGE_OF_FIELD	7					// Nstr. верхней край поля
#define AVERAGE_UNIT 9							// Nstr. средний блок
#define WHOSE_FIELD 21							// Nstr. чьё поле
#define BOTTOM_MESSAGE_SELECTION 33				// Nstr. НИЖНЕЕ СООБЩЕНИЕ ВЫБОР
#define STR_EXIT 35								// Nstr. выход
#define COLOR_HIT_THE_SHIP 0xFC
#define COLOR_BY_SHIP 0xF9
#define COLOR_BATTLE 0xF4
#define COLOR_FON_TEXT 0xF0
#define COLOR_TEXT 0xF9
//*************************************************************************************************************
const char* message[] = {
	" M O R S K O I   B O I",			//Морской бой				0
	"Razmestite korabli",				//Разместите корабли		1
	"Sluchainym obrazom",				//Случайным образом			2
	"Igra nachalas",					//Играть					3
	"Vashe pole",						//Ваше поле					4
	"Pole protivnika",					//Поле противника			5
	"Vash hod",							//Ваш ход					6
	"Vyiti iz igry",					//Выйти из игры				7
	"Novaia igra",						//Новая игра				8
	"                    ",	            // 20 пробелов				9
	"Igra nachalas, vash hod",			// Игра началась, ваш ход	10
	"Bay...",							//							11
	"Vy vyigrali",						// Вы выиграли				12
	"Vy proigrali",						// Вы проиграли				13
	"Protivnik khodit, zhdite",			// Противник ходит ждите	14
	"Loading...    %",					//							15
};
//-------------------------------------------------------------------------------------------------------------
unsigned const char sumbol[] = {
	218,	//┌	0
	196,	//─	1
	191,	//┐	2
	179,	//│	3
	197,	//┼	4
	192,	//└	5
	217,	//┘	6
	194,	//┬ 7
	193,	//┴ 8
	195,	//├ 9
	180,	//┤ 10
	32,		    //пробел 11
	178,	//▓ 12
	253,	//¤ 13	//42 * 	
	88,		//  X 14
	219,		//█ 15
	1		// ☺ 16 
};
//-------------------------------------------------------------------------------------------------------------
enum {
	SINGLE_DESK = 1,	/*однопалубный*/
	DOUBLE_DESK = 2,	/*двухпалубный*/
	THREE_DECKER = 3,	/*трехпалубный*/
	FOUR_DECK = 4,		/*четырехпалубный*/
	BY_SHIP = -2,		/*мимо корабля*/
	HIT_THE_SHIP = -1,	/*попал в корабль*/
};
//-------------------------------------------------------------------------------------------------------------
ships UserShips;
ships EnemyShips;
int UserBoard[nSIZE][nSIZE];								// Доска с кораблями user
int EnemyBoard[nSIZE][nSIZE];								// Доска с кораблями противника
COORD p2, p4, p3;
int d2, f4, i4, j4, l4, t3, i3, j3, l3;						// для добивания корабля противником
//*************************************************************************************************************
COORD c;													//объект для хранения координат
COORD coord = { 0, 0 };										//создание объекта координат
HANDLE hIn, hOut;
DWORD x, consoleMode;
//*************************************************************************************************************
void Init_Maus(void)
{
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cinfo = { 100, false };
	SetConsoleCursorInfo(hOut, &cinfo);
}
//*************************************************************************************************************
void buffer_clearing_mouse(void)		// очистка буфера мыши
{
	HANDLE h_mouse = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD all_events[256];								// массив произошедших событий
	DWORD read_event;											//конкретное количество произошедших событий 
	ReadConsoleInput(h_mouse, all_events, 256, &read_event);	//считать все события 	
}
//*************************************************************************************************************
int User_Input_Bombardment(int& X, int& Y)			//ввод user стрильба
{
	buffer_clearing_mouse();
	INPUT_RECORD ir;
	while (1){
		SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
		while (ReadConsoleInput(hIn, &ir, 1, &x)){
			if (ir.EventType == MOUSE_EVENT){
				if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
					c = ir.Event.MouseEvent.dwMousePosition;
					X = c.X - 46;
					Y = c.Y - 10;
					if (X >= 0 && X <= 19 && Y >= 0 && Y <= 19 && !(X % 2) && !(Y % 2) && UserShips.all_deck && EnemyShips.all_deck){
						X /= 2, Y /= 2;
						return 1;
					}
					else if (c.Y == STR_EXIT && c.X >= _width / 2 - 7 && c.X <= _width / 2 + 7){
						Exit();
					}
					else if (c.Y == BOTTOM_MESSAGE_SELECTION && c.X >= _width / 2 - 10 && c.X <= _width / 2 + 10){
						Start_Game();
						return 0;
					}
				}
			}
		}
	}
}
//*************************************************************************************************************
void bombardmentUser(int ar[][nSIZE])				//  бомбардировка User
{
	int x, y;
	bool flag;
	do{
		flag = false;
		if (!EnemyShips.all_deck){ UpperBlock(12); }
		else if (!UserShips.all_deck){ UpperBlock(13); }
		if (!User_Input_Bombardment(x, y)) return;	// <<======= ввод данных
		if (ar[y][x] == BY_SHIP || ar[y][x] == HIT_THE_SHIP) flag = true;	// <=======
		if (ar[y][x] > 0){
			flag = true;
			CheckEntireDecks(ar, EnemyShips, x, y);
		}
		MiddleBlock();					// =======>> вывод на екран
	} while (flag);
	if (ar[y][x] == 0){
		ar[y][x] = BY_SHIP;
		MiddleBlock();					// =======>> вывод на екран
	}
}
//*************************************************************************************************************
void finished_DOUBLE_DESK(int &x, int &y)
{
	do{
		d2++;
		switch (d2){
		case 2:	x = p2.X + 1; y = p2.Y;	break;
		case 3:	x = p2.X - 1; y = p2.Y;	break;
		case 4:	x = p2.X; y = p2.Y + 1;	break;
		case 5:	x = p2.X; y = p2.Y - 1;	break;
		}
	} while (!(x > -1 && x < nSIZE && y > -1 && y < nSIZE));

}
//-------------------------------------------------------------------------------------------------------------
void finished_FOUR_DECK(int &x, int &y)
{
	if (!i4){
		do{
			f4++;
			switch (f4){
			case 2:	x = p4.X + 1; y = p4.Y; if (x < nSIZE && UserBoard[y][x] == FOUR_DECK) i4 = 1;	break;
			case 3:	x = p4.X - 1; y = p4.Y;	if (x > -1 && UserBoard[y][x] == FOUR_DECK) i4 = 1;	break;
			case 4:	x = p4.X; y = p4.Y + 1;	if (y < nSIZE && UserBoard[y][x] == FOUR_DECK) i4 = -1;	break;
			case 5:	x = p4.X; y = p4.Y - 1;	if (y > -1 && UserBoard[y][x] == FOUR_DECK) i4 = -1;	break;
			}
		} while (!(x > -1 && x < nSIZE && y > -1 && y < nSIZE));
	}
	do{
		if (i4 > 0){
			j4++;
			switch (j4) {
			case 1:	x = p4.X + 1; y = p4.Y; break;
			case 2: x = p4.X - 1; y = p4.Y; break;
			case 3:	x = p4.X + 2; y = p4.Y; break;
			case 4: x = p4.X - 2; y = p4.Y; break;
			case 5:	x = p4.X + 3; y = p4.Y; break;
			case 6: x = p4.X - 3; y = p4.Y; break;
			}
		}
		else if (i4 < 0){
			l4++;
			switch (l4){
			case 1: y = p4.Y + 1; x = p4.X; break;
			case 2: y = p4.Y - 1; x = p4.X; break;
			case 3: y = p4.Y + 2; x = p4.X; break;
			case 4: y = p4.Y - 2; x = p4.X; break;
			case 5: y = p4.Y + 3; x = p4.X; break;
			case 6: y = p4.Y - 3; x = p4.X; break;
			}
		}
	} while (!(x > -1 && x < nSIZE && y > -1 && y < nSIZE));
}
//-------------------------------------------------------------------------------------------------------------
void finished_THREE_DECKER(int &x, int &y)
{
	if (!i3){
		do{
			t3++;
			switch (t3){
			case 2:	x = p3.X + 1; y = p3.Y; if (x < nSIZE && UserBoard[y][x] / 10 == THREE_DECKER) i3 = 1;	break;
			case 3:	x = p3.X - 1; y = p3.Y;	if (x > -1 && UserBoard[y][x] / 10 == THREE_DECKER) i3 = 1;	break;
			case 4:	x = p3.X; y = p3.Y + 1;	if (y < nSIZE && UserBoard[y][x] / 10 == THREE_DECKER) i3 = -1;	break;
			case 5:	x = p3.X; y = p3.Y - 1;	if (y > -1 && UserBoard[y][x] / 10 == THREE_DECKER) i3 = -1;	break;
			}
		} while (!(x > -1 && x < nSIZE && y > -1 && y < nSIZE));
	}
	do{
		if (i3 > 0){
			j3++;
			switch (j3) {
			case 1:	x = p3.X + 1; y = p3.Y; break;
			case 2: x = p3.X - 1; y = p3.Y; break;
			case 3:	x = p3.X + 2; y = p3.Y; break;
			case 4: x = p3.X - 2; y = p3.Y; break;
			}
		}
		else if (i3 < 0){
			l3++;
			switch (l3){
			case 1: y = p3.Y + 1; x = p3.X; break;
			case 2: y = p3.Y - 1; x = p3.X; break;
			case 3: y = p3.Y + 2; x = p3.X; break;
			case 4: y = p3.Y - 2; x = p3.X; break;
			}
		}
	} while (!(x > -1 && x < nSIZE && y > -1 && y < nSIZE));
}
//-------------------------------------------------------------------------------------------------------------
void bombardmentEnemy(int ar[][nSIZE])				//  бомбардировка противник
{
	int delay_time;					// задержка времени
	int x, y;
	bool flag;
	do{
		flag = false;
		if (!UserShips.all_deck){ UpperBlock(13); return; }

		if (d2){
			finished_DOUBLE_DESK(x, y);									// <<======= ввод данных
		}
		else if (f4){
			finished_FOUR_DECK(x, y);
		}
		else if (t3){
			finished_THREE_DECKER(x, y);
		}
		else { x = rand() % 10; y = rand() % 10; }
		if (ar[y][x] == BY_SHIP || ar[y][x] == HIT_THE_SHIP) flag = true;
		else if (ar[y][x] > 0){
			delay_time = rand() % 1000;
			Sleep(delay_time);
			flag = true;
			CheckEntireDecks(ar, UserShips, x, y);
			if (UserShips.n_0_deck_2 == 1 || UserShips.n_1_deck_2 == 1 || UserShips.n_2_deck_2 == 1) { if (!d2) { p2.X = x;  p2.Y = y; d2 = 1; } }
			else if (UserShips.deck_4 == FOUR_DECK - 1){ if (!f4) { p4.X = x; p4.Y = y; f4 = 1; } }
			else if (UserShips.n_0_deck_3 == THREE_DECKER - 1 || UserShips.n_1_deck_3 == THREE_DECKER - 1){ if (!t3) { p3.X = x; p3.Y = y; t3 = 1; } }
			MiddleBlock();					// =======>> вывод на екран
		}
	} while (flag);
	if (ar[y][x] == 0){
		delay_time = rand() % 1000;
		Sleep(delay_time);
		ar[y][x] = BY_SHIP;
		MiddleBlock();					// =======>> вывод на екран
	}
}
//*************************************************************************************************************

void CheckEntireDecks(int ar[][nSIZE], ships &obj, int x, int y)			// проверка целых палуб
{
	if (ar[y][x] / 10 == SINGLE_DESK){
		if (x - 1 > -1 && y - 1 > -1) { ar[y - 1][x - 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y - 1 > -1) { ar[y - 1][x + 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y + 1 < nSIZE) { ar[y + 1][x + 1] = BY_SHIP; }
		if (x - 1 > -1 && y + 1 < nSIZE) { ar[y + 1][x - 1] = BY_SHIP; }
		if (x - 1 > -1) { ar[y][x - 1] = BY_SHIP; }
		if (x + 1 < nSIZE) { ar[y][x + 1] = BY_SHIP; }
		if (y + 1 < nSIZE) { ar[y + 1][x] = BY_SHIP; }
		if (y - 1 > -1) { ar[y - 1][x] = BY_SHIP; }
		if (ar[y][x] % 10 == 0)	obj.n_0_deck_1--;
		if (ar[y][x] % 10 == 1)	obj.n_1_deck_1--;
		if (ar[y][x] % 10 == 2)	obj.n_2_deck_1--;
		if (ar[y][x] % 10 == 3)	obj.n_3_deck_1--;
		obj.all_deck--;
		ar[y][x] = HIT_THE_SHIP;
	}
	else if (ar[y][x] == FOUR_DECK){
		if (x - 1 > -1 && y - 1 > -1) { ar[y - 1][x - 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y - 1 > -1) { ar[y - 1][x + 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y + 1 < nSIZE) { ar[y + 1][x + 1] = BY_SHIP; }
		if (x - 1 > -1 && y + 1 < nSIZE) { ar[y + 1][x - 1] = BY_SHIP; }
		obj.all_deck--;
		obj.deck_4--;
		if (!obj.deck_4) { stroke_four_deck(ar, x, y); }
		if (!UserShips.deck_4){ f4 = 0; }
		ar[y][x] = HIT_THE_SHIP;
	}
	else if (ar[y][x] / 10 == THREE_DECKER){
		if (x - 1 > -1 && y - 1 > -1) { ar[y - 1][x - 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y - 1 > -1) { ar[y - 1][x + 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y + 1 < nSIZE) { ar[y + 1][x + 1] = BY_SHIP; }
		if (x - 1 > -1 && y + 1 < nSIZE) { ar[y + 1][x - 1] = BY_SHIP; }
		obj.all_deck--;
		if (ar[y][x] % 10 == 0){
			obj.n_0_deck_3--;
			if (!obj.n_0_deck_3)  { stroke_three_Decker(ar, x, y); }
			if (!UserShips.n_0_deck_3){ t3 = 0, i3 = 0, j3 = 0, l3 = 0; }
		}
		else if (ar[y][x] % 10 == 1){
			obj.n_1_deck_3--;
			if (!obj.n_1_deck_3)  { stroke_three_Decker(ar, x, y); }
			if (!UserShips.n_1_deck_3){ t3 = 0, i3 = 0, j3 = 0, l3 = 0; }
		}
		ar[y][x] = HIT_THE_SHIP;
	}


	else if (ar[y][x] / 10 == DOUBLE_DESK){
		if (x - 1 > -1 && y - 1 > -1) { ar[y - 1][x - 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y - 1 > -1) { ar[y - 1][x + 1] = BY_SHIP; }
		if (x + 1 < nSIZE && y + 1 < nSIZE) { ar[y + 1][x + 1] = BY_SHIP; }
		if (x - 1 > -1 && y + 1 < nSIZE) { ar[y + 1][x - 1] = BY_SHIP; }
		obj.all_deck--;

		if (ar[y][x] % 10 == 0){
			obj.n_0_deck_2--;
			if (!obj.n_0_deck_2)  { stroke_two_deck(ar, x, y); }
			if (!UserShips.n_0_deck_2){ d2 = 0; }
		}
		else if (ar[y][x] % 10 == 1){
			obj.n_1_deck_2--;
			if (!obj.n_1_deck_2) { stroke_two_deck(ar, x, y); }
			if (!UserShips.n_1_deck_2){ d2 = 0; }
		}
		else if (ar[y][x] % 10 == 2){
			obj.n_2_deck_2--;
			if (!obj.n_2_deck_2) { stroke_two_deck(ar, x, y); }
			if (!UserShips.n_2_deck_2){ d2 = 0; }
		}
		ar[y][x] = HIT_THE_SHIP;
	}
}
//-------------------------------------------------------------------------------------------------------------
void stroke_four_deck(int ar[][nSIZE], int x, int y)		// обводка четырех палубный
{
	if (x + 1 < nSIZE){
		if (ar[y][x + 1] == HIT_THE_SHIP){
			if (x + 2 < nSIZE){
				if (ar[y][x + 2] == HIT_THE_SHIP){
					if (x + 3 < nSIZE){
						if (ar[y][x + 3] == HIT_THE_SHIP){
							if (x + 4 < nSIZE) ar[y][x + 4] = BY_SHIP;
						}
						else  ar[y][x + 3] = BY_SHIP;
					}
				}
				else ar[y][x + 2] = BY_SHIP;
			}

		}
		else ar[y][x + 1] = BY_SHIP;
	}
	if (x - 1 > -1){
		if (ar[y][x - 1] == HIT_THE_SHIP){
			if (x - 2 > -1){
				if (ar[y][x - 2] == HIT_THE_SHIP){
					if (x - 3 > -1){
						if (ar[y][x - 3] == HIT_THE_SHIP){
							if (x - 4 > -1) ar[y][x - 4] = BY_SHIP;
						}
						else  ar[y][x - 3] = BY_SHIP;
					}
				}
				else ar[y][x - 2] = BY_SHIP;
			}

		}
		else ar[y][x - 1] = BY_SHIP;
	}
	if (y + 1 < nSIZE){
		if (ar[y + 1][x] == HIT_THE_SHIP){
			if (y + 2 < nSIZE){
				if (ar[y + 2][x] == HIT_THE_SHIP){
					if (y + 3 < nSIZE){
						if (ar[y + 3][x] == HIT_THE_SHIP){
							if (y + 4 < nSIZE) ar[y + 4][x] = BY_SHIP;
						}
						else  ar[y + 3][x] = BY_SHIP;
					}
				}
				else ar[y + 2][x] = BY_SHIP;
			}

		}
		else ar[y + 1][x] = BY_SHIP;
	}
	if (y - 1 > -1){
		if (ar[y - 1][x] == HIT_THE_SHIP){
			if (y - 2 > -1){
				if (ar[y - 2][x] == HIT_THE_SHIP){
					if (y - 3 > -1){
						if (ar[y - 3][x] == HIT_THE_SHIP){
							if (y - 4 > -1) ar[y - 4][x] = BY_SHIP;
						}
						else  ar[y - 3][x] = BY_SHIP;
					}
				}
				else ar[y - 2][x] = BY_SHIP;
			}

		}
		else ar[y - 1][x] = BY_SHIP;
	}
}
//-------------------------------------------------------------------------------------------------------------
void stroke_three_Decker(int ar[][nSIZE], int x, int y)		// обводка трехпалубный
{
	if (x + 1 < nSIZE){
		if (ar[y][x + 1] == HIT_THE_SHIP){
			if (x + 2 < nSIZE){
				if (ar[y][x + 2] == HIT_THE_SHIP){
					if (x + 3 < nSIZE) ar[y][x + 3] = BY_SHIP;
				}
				else ar[y][x + 2] = BY_SHIP;
			}

		}
		else ar[y][x + 1] = BY_SHIP;
	}
	if (x - 1 > -1){
		if (ar[y][x - 1] == HIT_THE_SHIP){
			if (x - 2 > -1){
				if (ar[y][x - 2] == HIT_THE_SHIP){
					if (x - 3 > -1) ar[y][x - 3] = BY_SHIP;
				}
				else ar[y][x - 2] = BY_SHIP;
			}

		}
		else ar[y][x - 1] = BY_SHIP;
	}
	if (y + 1 < nSIZE){
		if (ar[y + 1][x] == HIT_THE_SHIP){
			if (y + 2 < nSIZE){
				if (ar[y + 2][x] == HIT_THE_SHIP){
					if (y + 3 < nSIZE) ar[y + 3][x] = BY_SHIP;
				}
				else ar[y + 2][x] = BY_SHIP;
			}

		}
		else ar[y + 1][x] = BY_SHIP;
	}
	if (y - 1 > -1){
		if (ar[y - 1][x] == HIT_THE_SHIP){
			if (y - 2 > -1){
				if (ar[y - 2][x] == HIT_THE_SHIP){
					if (y - 3 > -1) ar[y - 3][x] = BY_SHIP;
				}
				else ar[y - 2][x] = BY_SHIP;
			}

		}
		else ar[y - 1][x] = BY_SHIP;
	}
}
//-------------------------------------------------------------------------------------------------------------
void stroke_two_deck(int ar[][nSIZE], int x, int y)		// обводка двох-палубный
{
	if (x + 1 < nSIZE){
		if (ar[y][x + 1] == HIT_THE_SHIP){
			if (x + 2 < nSIZE) ar[y][x + 2] = BY_SHIP;
		}
		else ar[y][x + 1] = BY_SHIP;
	}
	if (x - 1 > -1){
		if (ar[y][x - 1] == HIT_THE_SHIP){
			if (x - 2 > -1) ar[y][x - 2] = BY_SHIP;
		}
		else ar[y][x - 1] = BY_SHIP;
	}
	if (y + 1 < nSIZE){
		if (ar[y + 1][x] == HIT_THE_SHIP){
			if (y + 2 < nSIZE) ar[y + 2][x] = BY_SHIP;
		}
		else ar[y + 1][x] = BY_SHIP;
	}
	if (y - 1 > -1){
		if (ar[y - 1][x] == HIT_THE_SHIP){
			if (y - 2 > -1) ar[y - 2][x] = BY_SHIP;
		}
		else ar[y - 1][x] = BY_SHIP;
	}
}
//*************************************************************************************************************
void Start_Game(void)				// Старт игры
{
	Init_Nul(UserBoard, UserShips);
	Init_Nul(EnemyBoard, EnemyShips);
	UpperBlock(1);
	MiddleBlock();
	LowerBlock(2);
	Manual_Placement(UserBoard);
	Automatic_Placement(EnemyBoard, EnemyShips);
	MiddleBlock();
	LowerBlock(8);
	rand() % 2 ? Game_S_U() : Game_S_E();
}
//-------------------------------------------------------------------------------------------------------------
void Game_S_E(void)
{
	UpperBlock(3);
	Sleep(1500);
	do {
		UpperBlock(14);
		bombardmentEnemy(UserBoard);				//  бомбардировка противник
		UpperBlock(6);
		bombardmentUser(EnemyBoard);				//  бомбардировка User
	} while (true);
}
//-------------------------------------------------------------------------------------------------------------
void Game_S_U(void)
{
	UpperBlock(10);
	bombardmentUser(EnemyBoard);				//  бомбардировка User
	do {
		UpperBlock(14);
		bombardmentEnemy(UserBoard);				//  бомбардировка противник
		UpperBlock(6);
		bombardmentUser(EnemyBoard);				//  бомбардировка User
	} while (true);
}
//*************************************************************************************************************
int User_Input_Manual_Placement(int& X, int& Y)			//ввод ручная расстановка
{
	INPUT_RECORD ir;
	while (1){
		SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
		while (ReadConsoleInput(hIn, &ir, 1, &x)){
			if (ir.EventType == MOUSE_EVENT){
				if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
					c = ir.Event.MouseEvent.dwMousePosition;
					X = c.X - 14;
					Y = c.Y - 10;
					if (X >= 0 && X <= 19 && Y >= 0 && Y <= 19 && !(X % 2) && !(Y % 2)){
						X /= 2, Y /= 2;
						return 1;
					}
					else if (c.Y == STR_EXIT && c.X >= _width / 2 - 7 && c.X <= _width / 2 + 7){
						Exit();
					}
					else if (c.Y == BOTTOM_MESSAGE_SELECTION && c.X >= _width / 2 - 10 && c.X <= _width / 2 + 10){
						Init_Nul(UserBoard, UserShips);
						Automatic_Placement(UserBoard, UserShips);
						return 0;
					}
				}
				else if (ir.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED){
					c = ir.Event.MouseEvent.dwMousePosition;
					X = c.X - 14;
					Y = c.Y - 10;
					if (X >= 0 && X <= 19 && Y >= 0 && Y <= 19){
						Init_Nul(UserBoard, UserShips);
						MiddleBlock();
						Manual_Placement(UserBoard);						// ручная расстановка
						return 0;
					}
				}
			}
		}
	}
}
//*************************************************************************************************************
void Loading(void)				// загрузка
{
	SetConsoleTitleA("MORSKOI BOI");
	system("mode con cols=78 lines=38");//установка размеров окна консоли
	Init_Maus();
	srand(unsigned(time(NULL)));			//запуск генератора случайных чисел
	rand();
	SetConsoleTextAttribute(hOut, 0xF9);
	int temp_j = _width / 2 - 9;
	int temp_i = _height / 2 - 2;
	for (int i = 0; i < _height; i++){
		for (int j = 0; j < _width; j++){
			putchar(' ');
			if (i == temp_i && j >= temp_j){
				j == temp_j && putchar('W');
				j == temp_j + 1 && putchar('E');
				j == temp_j + 2 && putchar('L');
				j == temp_j + 3 && putchar('C');
				j == temp_j + 4 && putchar('O');
				j == temp_j + 5 && putchar('M');
				j == temp_j + 6 && putchar('E');
			}
			Sleep(1);
		}
	}
	coord = { short((_width - strlen(message[15])) / 2), short(_height - 5) };
	SetConsoleCursorPosition(hOut, coord);
	cout << message[15] << "\b";
	for (int i = 0; i < 100; i++)
	{
		printf("\b\b%2d", i);
		Sleep(10);
	}
	system("cls");
	system("color F0"); // izmenenie cveta fona i teksta   F - fon, 0 - text
}
//*************************************************************************************************************
void UpperBlock(int n)		// верхний блок
{
	short temp = (_width - strlen(message[0])) / 2;
	coord = { temp, TITLE };
	SetConsoleCursorPosition(hOut, coord);
	SetConsoleTextAttribute(hOut, COLOR_TEXT);
	cout << message[0];
	SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
	coord = { 0, TOP_MESSAGE };
	SetConsoleCursorPosition(hOut, coord);
	drawOneLine(_width, sumbol[11]);
	temp = (_width - strlen(message[n])) / 2;
	coord = { temp, TOP_MESSAGE };
	SetConsoleCursorPosition(hOut, coord);
	SetConsoleTextAttribute(hOut, 0xFC);
	cout << message[n];
	SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
	coord = { 0, UPPER_EDGE_OF_FIELD };
	SetConsoleCursorPosition(hOut, coord);
	drawOneLine(13, sumbol[11]);
	for (int i = 0; i < 2; i++){
		for (char j = 65; j < nSIZE + 65; j++){
			cout << " " << j;
		}
		drawOneLine(12, sumbol[11]);
	}
}
//-------------------------------------------------------------------------------------------------------------
void left_block_ships(int i)				// левый блок корабли 
{
	switch (i){
	case 0: UserShips.deck_4 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); break;
	case 1: UserShips.n_0_deck_3 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 2: UserShips.n_1_deck_3 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 3: UserShips.n_0_deck_2 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	case 4: UserShips.n_1_deck_2 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	case 5: UserShips.n_2_deck_2 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	case 6: UserShips.n_0_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	case 7: UserShips.n_1_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	case 8: UserShips.n_2_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	case 9: UserShips.n_3_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); break;
	}
	SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
}
//-------------------------------------------------------------------------------------------------------------
void right_block_ships(int i)				// правый блок корабли
{
	switch (i){
	case 0: EnemyShips.deck_4 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 1: EnemyShips.n_0_deck_3 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 2: EnemyShips.n_1_deck_3 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 3: EnemyShips.n_0_deck_2 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 4: EnemyShips.n_1_deck_2 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 5: EnemyShips.n_2_deck_2 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 6: EnemyShips.n_0_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 7: EnemyShips.n_1_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 8: EnemyShips.n_2_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	case 9: EnemyShips.n_3_deck_1 ? SetConsoleTextAttribute(hOut, COLOR_BY_SHIP) : SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[11]); putchar(sumbol[16]); putchar(sumbol[11]); break;
	}
	SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
}
//-------------------------------------------------------------------------------------------------------------
void MiddleBlock(void)		// средний блок
{
	coord = { 0, AVERAGE_UNIT };
	SetConsoleCursorPosition(hOut, coord);
	int i = 0, j, x;
	drawOneLine(13, sumbol[11]);
	OutputLine(i);
	OutputLine(i);
	cout << "\n";
	for (i = 0; i < nSIZE;)
	{
		left_block_ships(i);
		drawOneLine(5, sumbol[11]);
		cout.width(2);
		cout << i + 1 << " ";
		for (j = 0; j < nSIZE; j++)
		{
			cout << sumbol[3]; ShowUser(UserBoard[i][j]);
		}
		putchar(sumbol[3]);
		drawOneLine(8, sumbol[11]);

		cout.width(2);
		cout << i + 1 << " ";
		for (x = 0; x < nSIZE; x++)
		{
			cout << sumbol[3]; ShowEnemy(EnemyBoard[i][x]);// ShowUser(EnemyBoard[i][x]);
		}
		putchar(sumbol[3]);
		drawOneLine(7, sumbol[11]);
		right_block_ships(i);

		i++;
		drawOneLine(13, sumbol[11]);
		OutputLine(i);
		OutputLine(i);
		putchar('\n');
	}
}
//-------------------------------------------------------------------------------------------------------------
void LowerBlock(int n)		// нижний  блок
{
	short temp;
	temp = (WHOSE_FIELD - strlen(message[4])) / 2 + 13;
	coord = { temp, 31 };
	SetConsoleCursorPosition(hOut, coord);
	cout << message[4];
	temp = (WHOSE_FIELD - strlen(message[5])) / 2 + 45;
	coord = { temp, 31 };
	SetConsoleCursorPosition(hOut, coord);
	cout << message[5];

	coord = { 0, BOTTOM_MESSAGE_SELECTION };
	SetConsoleCursorPosition(hOut, coord);
	drawOneLine(_width, sumbol[11]);
	temp = (_width - strlen(message[n])) / 2;
	coord = { temp, BOTTOM_MESSAGE_SELECTION };
	SetConsoleCursorPosition(hOut, coord);
	SetConsoleTextAttribute(hOut, COLOR_TEXT);
	cout << message[n];
	temp = (_width - strlen(message[7])) / 2;
	coord = { temp, STR_EXIT };
	SetConsoleCursorPosition(hOut, coord);
	cout << message[7];
	SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
}
//-------------------------------------------------------------------------------------------------------------
void ShowUser(int n)
{
	switch (n)
	{
	case HIT_THE_SHIP:
		SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[14]);
		SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
		break;
	case BY_SHIP:
		SetConsoleTextAttribute(hOut, COLOR_BY_SHIP);
		putchar(sumbol[13]);
		SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 20:
	case 21:
	case 22:
	case 30:
	case 31:
	case 4:
		SetConsoleTextAttribute(hOut, COLOR_BATTLE);
		putchar(sumbol[15]);
		SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
		break;
	default:
		putchar(sumbol[11]);
	}
}
//-------------------------------------------------------------------------------------------------------------
void ShowEnemy(int n)
{
	switch (n)
	{
	case HIT_THE_SHIP:
		SetConsoleTextAttribute(hOut, COLOR_HIT_THE_SHIP);
		putchar(sumbol[14]);
		SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
		break;
	case BY_SHIP:
		SetConsoleTextAttribute(hOut, COLOR_BY_SHIP);
		putchar(sumbol[13]);
		SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
		break;
	default:
		SetConsoleTextAttribute(hOut, 0xF7);
		putchar(sumbol[12]);
		SetConsoleTextAttribute(hOut, COLOR_FON_TEXT);
	}
}
//-------------------------------------------------------------------------------------------------------------
void drawOneLine(int count, char symbol)
{
	for (int i = 0; i < count; i++){
		putchar(symbol);
	}
}
//-------------------------------------------------------------------------------------------------------------
void OutputLine(int i)
{
	for (int j = 0; j <= nSIZE; j++)
	{
		if (i == 0){
			if (j == 0) cout << sumbol[0];
			else if (j == nSIZE) cout << sumbol[1] << sumbol[2];
			else cout << sumbol[1] << sumbol[7];
		}
		else if (i == nSIZE){
			if (j == 0) cout << sumbol[5];
			else if (j == nSIZE) cout << sumbol[1] << sumbol[6];
			else cout << sumbol[1] << sumbol[8];
		}
		else{
			if (j == 0) cout << sumbol[9];
			else if (j == nSIZE) cout << sumbol[1] << sumbol[10];
			else cout << sumbol[1] << sumbol[4];
		}
	}
	drawOneLine(11, sumbol[11]);
}
//*************************************************************************************************************
void Init_Nul(int ar[][nSIZE], ships &obj)
{
	int i, j;
	for (i = 0; i < nSIZE; i++){
		for (j = 0; j < nSIZE; j++){
			ar[i][j] = 0;
		}
	}
	obj = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20 };
	d2 = 0;
	f4 = 0, i4 = 0, j4 = 0, l4 = 0;
	t3 = 0, i3 = 0, j3 = 0, l3 = 0;
}
//*************************************************************************************************************
void Exit(void)											// выход
{
	system("cls");
	coord = { _width / 2 - 2, _height / 2 };
	SetConsoleCursorPosition(hOut, coord);
	cout << message[11] << endl;
	Sleep(1000);
	exit(0);
}
//*************************************************************************************************************
void Automatic_Placement(int ar[][nSIZE], ships &obj)				//автоматическая расстановка
{
	rand();									//холостой ход генератора случаный чисел
	int i, offset;							//направление смищения			
	bool flag;

	offset = rand() % 2;
	switch (offset)
	{
	case 0:
		c.X = rand() % 7;
		c.Y = rand() % 10;
		for (i = 0; i < 4; i++){
			ar[c.X + i][c.Y] = FOUR_DECK;
		}
		break;
	case 1:
		c.X = rand() % 10;
		c.Y = rand() % 7;
		for (i = 0; i < 4; i++){
			ar[c.X][c.Y + i] = FOUR_DECK;
		}
		break;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	for (i = 0; i < 2; i++){
		do{
			flag = true;
			offset = rand() % 2;
			switch (offset)
			{
			case 0:							//смещение по X
				c.X = rand() % 8;
				c.Y = rand() % 10;
				if (flag = f_gorizont(ar, c.X, c.Y, THREE_DECKER))
					ar[c.Y][c.X] = THREE_DECKER * 10 + i, ar[c.Y][c.X + 1] = THREE_DECKER * 10 + i, ar[c.Y][c.X + 2] = THREE_DECKER * 10 + i;
				break;
			case 1:							//смещение по Y
				c.X = rand() % 10;
				c.Y = rand() % 8;
				if (flag = f_vrtical(ar, c.X, c.Y, THREE_DECKER))
					ar[c.Y][c.X] = THREE_DECKER * 10 + i, ar[c.Y + 1][c.X] = THREE_DECKER * 10 + i, ar[c.Y + 2][c.X] = THREE_DECKER * 10 + i;
				break;
			}
		} while (!flag);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	for (i = 0; i < 3; i++){
		do{
			flag = true;
			offset = rand() % 2;
			switch (offset)
			{
			case 0:							//смещение по X
				c.X = rand() % 9;
				c.Y = rand() % 10;
				if (flag = f_gorizont(ar, c.X, c.Y, DOUBLE_DESK))
					ar[c.Y][c.X] = DOUBLE_DESK * 10 + i, ar[c.Y][c.X + 1] = DOUBLE_DESK * 10 + i;
				break;
			case 1:							//смещение по Y
				c.X = rand() % 10;
				c.Y = rand() % 9;
				if (flag = f_vrtical(ar, c.X, c.Y, DOUBLE_DESK))
					ar[c.Y][c.X] = DOUBLE_DESK * 10 + i, ar[c.Y + 1][c.X] = DOUBLE_DESK * 10 + i;
				break;
			}
		} while (!flag);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	for (i = 0; i < 4; i++){
		do{
			flag = true;
			c.X = rand() % 10;
			c.Y = rand() % 10;
			if (flag = f_gorizont(ar, c.X, c.Y, SINGLE_DESK))
				ar[c.Y][c.X] = SINGLE_DESK * 10 + i;
		} while (!flag);
	}
	obj = { FOUR_DECK, THREE_DECKER, THREE_DECKER, DOUBLE_DESK, DOUBLE_DESK, DOUBLE_DESK, SINGLE_DESK, SINGLE_DESK, SINGLE_DESK, SINGLE_DESK, 20 };
}
//*****************************************************************************************************************************
bool f_gorizont(int ar[][nSIZE], int x, int y, int size)
{
	int i = x - 1, count = size + i + 2;
	for (; i < count; i++){
		if (i > -1 && i < nSIZE){
			if (ar[y][i])
				return false;
			if (y - 1 > -1)
			if (ar[y - 1][i])
				return false;
			if (y + 1 < nSIZE)
			if (ar[y + 1][i])
				return false;
		}
	}
	return true;
}

bool f_vrtical(int ar[][nSIZE], int x, int y, int size)
{
	int i = y - 1, count = size + i + 2;
	for (; i < count; i++){
		if (i > -1 && i < nSIZE){
			if (ar[i][x])
				return false;
			if (x - 1 > -1)
			if (ar[i][x - 1])
				return false;
			if (x + 1 < nSIZE)
			if (ar[i][x + 1])
				return false;
		}
	}
	return true;
}
//*************************************************************************************************************
bool checking_cells(int ar[][nSIZE], int x, int y)	// проверка ячейки
{
	if (x > -1 && x < nSIZE && y > -1 && y < nSIZE && !ar[y][x])
		return	   !(x - 1 > -1 && ar[y][x - 1])
		&& !(y - 1 > -1 && ar[y - 1][x])
		&& !(x + 1 < nSIZE && ar[y][x + 1])
		&& !(y + 1 < nSIZE && ar[y + 1][x])
		&& !(x - 1 > -1 && (y - 1 > -1 && ar[y - 1][x - 1]))
		&& !(x + 1 < nSIZE && (y - 1 > -1 && ar[y - 1][x + 1]))
		&& !(x + 1 < nSIZE && (y + 1 < nSIZE && ar[y + 1][x + 1]))
		&& !(x - 1 > -1 && (y + 1 < nSIZE && ar[y + 1][x - 1]));
	return false;
}
//*************************************************************************************************************
void Manual_Placement(int ar[][nSIZE])						//ручная расстановка
{
	int mas_X[3] = { 0 }, mas_Y[3] = { 0 };
	int x, y, temp_x, temp_y, temp_x2, temp_y2, temp_x3, temp_y3, flag = 0;

	if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
	temp_x = x;
	temp_y = y;
	ar[y][x] = FOUR_DECK;
	MiddleBlock();					// =======>> вывод на екран
	do{
		if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
		if (!ar[y][x]){
			if (x == temp_x){
				if (temp_y - y == -1 || temp_y - y == 1){
					ar[y][x] = FOUR_DECK; --flag;
					temp_y2 = y;
				}
			}
			else if (y == temp_y){
				if (temp_x - x == -1 || temp_x - x == 1){
					ar[y][x] = FOUR_DECK; ++flag;
					temp_x2 = x;
				}
			}
		}
		MiddleBlock();				// =======>> вывод на екран
	} while (!flag);
	do{
		if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
		if (!ar[y][x]){
			if (flag < 0 && x == temp_x){
				if (temp_y - 1 == y || temp_y + 1 == y){
					ar[y][x] = FOUR_DECK; --flag;
					temp_y3 = y;
				}
				else if (temp_y2 - 1 == y || temp_y2 + 1 == y){
					ar[y][x] = FOUR_DECK; --flag;
					temp_y3 = y;
				}
			}
			else if (flag > 0 && y == temp_y){
				if (temp_x - 1 == x || temp_x + 1 == x){
					ar[y][x] = FOUR_DECK; ++flag;
					temp_x3 = x;
				}
				else if (temp_x2 - 1 == x || temp_x2 + 1 == x){
					ar[y][x] = FOUR_DECK; ++flag;
					temp_x3 = x;
				}
			}
		}
		MiddleBlock();				// =======>> вывод на екран
	} while (flag == 1 || flag == -1);
	do{
		if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
		if (!ar[y][x]){
			if (flag < 0 && x == temp_x){
				if (temp_y - 1 == y || temp_y + 1 == y){
					ar[y][x] = FOUR_DECK; --flag;
				}
				else if (temp_y2 - 1 == y || temp_y2 + 1 == y){
					ar[y][x] = FOUR_DECK; --flag;
				}
				else if (temp_y3 - 1 == y || temp_y3 + 1 == y){
					ar[y][x] = FOUR_DECK; --flag;
				}
			}
			else if (flag > 0 && y == temp_y){
				if (temp_x - 1 == x || temp_x + 1 == x){
					ar[y][x] = FOUR_DECK; ++flag;
				}
				else if (temp_x2 - 1 == x || temp_x2 + 1 == x){
					ar[y][x] = FOUR_DECK; ++flag;
				}
				else if (temp_x3 - 1 == x || temp_x3 + 1 == x){
					ar[y][x] = FOUR_DECK; ++flag;
				}
			}
		}
		MiddleBlock();					// =======>> вывод на екран
	} while (flag == 2 || flag == -2);
	UserShips.deck_4 = FOUR_DECK;
	MiddleBlock();					// =======>> вывод на екран
	//-------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 2; i++){
		int fromX = -1, fromY = -1, toX = -1, toY = -1;
		do{
			flag = 0;
			if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
			if (checking_cells(ar, x, y)){
				fromX = x, fromY = y, toX = x, toY = y;
				if (checking_cells(ar, x - 1, y)){
					fromX--;
					if (checking_cells(ar, x - 2, y)){
						fromX--;
					}
				}
				if (checking_cells(ar, x + 1, y)){
					toX++;
					if (checking_cells(ar, x + 2, y)){
						toX++;
					}
				}
				if (checking_cells(ar, x, y - 1)){
					fromY--;
					if (checking_cells(ar, x, y - 2)){
						fromY--;
					}
				}
				if (checking_cells(ar, x, y + 1)){
					toY++;
					if (checking_cells(ar, x, y + 2)){
						toY++;
					}
				}
			}
			if (toX - fromX >= 2 && toY - fromY >= 2){
				ar[y][x] = THREE_DECKER * 10 + i;
				temp_y = y, temp_x = x;
				flag++;
				MiddleBlock();		// =======>> вывод на екран
				do{
					flag = 0;
					if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
					if (y == temp_y && x >= fromX && x <= toX){
						if (x - 1 == temp_x || x + 1 == temp_x){
							ar[y][x] = THREE_DECKER * 10 + i;
							temp_y2 = y, temp_x2 = x;
							flag--;
							MiddleBlock();		// =======>> вывод на екран
							do{
								flag = 0;
								if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
								if (!ar[y][x] && y == temp_y && x >= fromX && x <= toX && (x - 1 == temp_x || x + 1 == temp_x || x - 1 == temp_x2 || x + 1 == temp_x2)){
									ar[y][x] = THREE_DECKER * 10 + i;
									flag--;
									MiddleBlock();		// =======>> вывод на екран
								}
							} while (!flag);
						}
					}
					else if (x == temp_x && y >= fromY && y <= toY){
						if (y - 1 == temp_y || y + 1 == temp_y){
							ar[y][x] = THREE_DECKER * 10 + i;
							temp_y2 = y, temp_x2 = x;
							flag++;
							MiddleBlock();		// =======>> вывод на екран
							do{
								flag = 0;
								if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
								if (!ar[y][x] && x == temp_x && y >= fromY && y <= toY && (y - 1 == temp_y || y + 1 == temp_y || y - 1 == temp_y2 || y + 1 == temp_y2)){
									ar[y][x] = THREE_DECKER * 10 + i;
									flag++;
									MiddleBlock();		// =======>> вывод на екран
								}
							} while (!flag);
						}
					}
				} while (!flag);
			}
			else if (toX - fromX >= 2){
				ar[y][x] = THREE_DECKER * 10 + i;
				temp_y = y, temp_x = x;
				flag++;
				MiddleBlock();		// =======>> вывод на екран
				do{
					flag = 0;
					if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
					if (y == temp_y && x >= fromX && x <= toX){
						if (x - 1 == temp_x || x + 1 == temp_x){
							ar[y][x] = THREE_DECKER * 10 + i;
							temp_x2 = x;
							MiddleBlock();		// =======>> вывод на екран
							do{
								if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
								if (!ar[y][x] && y == temp_y && x >= fromX && x <= toX && (x - 1 == temp_x || x + 1 == temp_x || x - 1 == temp_x2 || x + 1 == temp_x2)){
									ar[y][x] = THREE_DECKER * 10 + i;
									flag++;
									MiddleBlock();		// =======>> вывод на екран
								}
							} while (!flag);
						}
					}
				} while (!flag);
			}
			else if (toY - fromY >= 2){
				ar[y][x] = THREE_DECKER * 10 + i;
				temp_y = y, temp_x = x;
				flag++;
				MiddleBlock();		// =======>> вывод на екран
				do{
					flag = 0;
					if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
					if (x == temp_x && y >= fromY && y <= toY){
						if (y - 1 == temp_y || y + 1 == temp_y){
							ar[y][x] = THREE_DECKER * 10 + i;
							temp_y2 = y;
							MiddleBlock();		// =======>> вывод на екран
							do{
								if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
								if (!ar[y][x] && x == temp_x && y >= fromY && y <= toY && (y - 1 == temp_y || y + 1 == temp_y || y - 1 == temp_y2 || y + 1 == temp_y2)){
									ar[y][x] = THREE_DECKER * 10 + i;
									flag++;
									MiddleBlock();		// =======>> вывод на екран
								}
							} while (!flag);
						}
					}
				} while (!flag);
			}
		} while (!flag);
		switch (i)
		{
		case 0: UserShips.n_0_deck_3 = THREE_DECKER; break;
		case 1: UserShips.n_1_deck_3 = THREE_DECKER; break;
		}
		MiddleBlock();					// =======>> вывод на екран
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3;)
	{
		if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
		if (checking_cells(ar, x, y)){
			if (checking_cells(ar, x + 1, y) || checking_cells(ar, x - 1, y) || checking_cells(ar, x, y - 1) || checking_cells(ar, x, y + 1)){
				ar[y][x] = DOUBLE_DESK * 10 + i;
				temp_x = x;
				temp_y = y;
				MiddleBlock();					// =======>> вывод на екран
				ar[y][x] = 0;
				do{
					if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
					if (checking_cells(ar, x, y)){
						if (y == temp_y && (x + 1 == temp_x || x - 1 == temp_x)){
							ar[y][x] = DOUBLE_DESK * 10 + i;
							ar[temp_y][temp_x] = DOUBLE_DESK * 10 + i;
							i++;
							MiddleBlock();					// =======>> вывод на екран
						}
						else if (x == temp_x && (y - 1 == temp_y || y + 1 == temp_y)){
							ar[y][x] = DOUBLE_DESK * 10 + i;
							ar[temp_y][temp_x] = DOUBLE_DESK * 10 + i;
							i++;
							MiddleBlock();					// =======>> вывод на екран
						}
					}
				} while (!ar[temp_y][temp_x]);
			}
		}
		switch (i)
		{
		case 1:UserShips.n_0_deck_2 = DOUBLE_DESK; break;
		case 2:UserShips.n_1_deck_2 = DOUBLE_DESK; break;
		case 3:UserShips.n_2_deck_2 = DOUBLE_DESK; break;
		}
		MiddleBlock();					// =======>> вывод на екран
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 4;)
	{
		do{
			if (!User_Input_Manual_Placement(x, y)) return;	// <<======= ввод данных
		} while (!checking_cells(ar, x, y));
		ar[y][x] = SINGLE_DESK * 10 + i;
		i++;
		switch (i)
		{
		case 1:UserShips.n_0_deck_1 = SINGLE_DESK; break;
		case 2:UserShips.n_1_deck_1 = SINGLE_DESK; break;
		case 3:UserShips.n_2_deck_1 = SINGLE_DESK; break;
		case 4:UserShips.n_3_deck_1 = SINGLE_DESK; break;
		}
		MiddleBlock();					// =======>> вывод на екран
	}
}