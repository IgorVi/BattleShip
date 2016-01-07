///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																											 //
//			ЧЕРЕВКО ИГОР ВИКТОРОВИЧ																			 //
//			12.11.2014 г.																					 //
//			ШАГ		1 семестр																				 //
//			Email: igor.viktorovich.ua@gmail.com															 //
//																											 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include<iostream>
#include<Windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
using namespace std;


const int nSIZE = 10;
//-------------------------------------------------------------------------------------------------------------
struct ships
{
	int deck_4;
	int n_0_deck_3;
	int n_1_deck_3;
	int n_0_deck_2;
	int n_1_deck_2;
	int n_2_deck_2;
	int n_0_deck_1;
	int n_1_deck_1;
	int n_2_deck_1;
	int n_3_deck_1;
	int all_deck;
};
//-------------------------------------------------------------------------------------------------------------
void Loading(void);											// загрузка
void Init_Maus(void);
void buffer_clearing_mouse(void);							// очистка буфера мыши
void Start_Game(void);										// Старт игры
void Game_S_U(void);
void Game_S_E(void);
void Init_Nul(int ar[][nSIZE], ships& obj);					// инициализация  нулями 
void UpperBlock(int n);										// верхний блок
void MiddleBlock(void);										// средний блок
void left_block_ships(int i);								// левый блок корабли 
void right_block_ships(int i);								// правый блок корабли
void LowerBlock(int n);										// нижний  блок
void drawOneLine(int count, char symbol);					// вывод символа count раз
void OutputLine(int i);
bool f_gorizont(int ar[][10], int x, int y, int size);
bool f_vrtical(int ar[][nSIZE], int x, int y, int size);
bool checking_cells(int ar[][nSIZE], int x, int y);			// проверка ячейки
void Automatic_Placement(int ar[][nSIZE], ships &obj);		// автоматическая расстановка
void Manual_Placement(int ar[][nSIZE]);						// ручная расстановка
void ShowUser(int n);										// вывод символа на поле user
void ShowEnemy(int n);										// вывод символа на поле противника
void bombardmentUser(int ar[][nSIZE]);						// бомбардировка User
void bombardmentEnemy(int ar[][nSIZE]);						// бомбардировка противник
int User_Input_Manual_Placement(int& X, int& Y);			// ввод ручная расстановка
int User_Input_Bombardment(int& X, int& Y);					//ввод user стрильба
void CheckEntireDecks(int ar[][nSIZE], ships &obj, int x, int y);			// проверка целых палуб
void stroke_two_deck(int ar[][nSIZE], int x, int y);		// обводка двох-палубный
void stroke_three_Decker(int ar[][nSIZE], int x, int y);	// обводка трехпалубный
void stroke_four_deck(int ar[][nSIZE], int x, int y);		// обводка четырех палубный
void finished_DOUBLE_DESK(int &x, int &y);					// обстрел двохпалубник
void finished_THREE_DECKER(int &x, int &y);					// обстрел трехпалубный
void finished_FOUR_DECK(int &x, int &y);					// обстрел четырехпалубный
void Exit(void);											// выход
//*************************************************************************************************************

#endif // _FUNCTION_H_