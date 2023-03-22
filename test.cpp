#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Point
{
	int row;
	int col;
	//int value;
};
Point pt1;
Point pt2;

void textColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void greating()
{
	ifstream ifs;
	ifs.open("Greating.txt", ios::in);

	while(!ifs.eof())
	{
		char c;
		ifs.get(c);
		Sleep(12);
		textColor(14);
		cout << c;

		// Nếu mà nhận được sự tương tác 1 phím bất kỳ thì sẽ kết thúc lời giới thiệu.
		if(kbhit())
		{
			return;
		}
	}
	
	ifs.close();
}

void deleteScreen()
{
	system("CLS");
}

void createArray(char a[][13], int row, int col)
{
	// Set up barrier
	for (int i = 0; i < col; i++)
		a[0][i] = '0';
	for (int i = 0; i < col; i++)
		a[row - 1][i] = '0';
	for (int i = 0; i < row; i++)
		a[i][0] = '0';
	for (int i = 0; i < row; i++)
		a[i][col - 1] = '0';

	for (int i = 1; i < row - 1; i++)
		for(int j = 1; j < col - 1; j++)
			a[i][j] = 49 + (rand() % 9);
}

void displayArray(char a[][13], int row, int col)
{
	
	for (int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
			cout << a[i][j] << " ";
		cout << endl;
	}
}

// Check on 1 line, linear direction
bool checkLineX(char a[][], int y1, int y2, int x)
{
	// Find point have column max & min
	int minimum = min(y1, y2);
	int maximum = max(y1, y2);
	// run column
	for (int y = min + 1; y < max; y++)
	{
		if (a[x][y] != '0') // if there is at least 1 none-0 char, false
		{
			cout << "die: (" << x << ", " << y << ")" << endl;
			return false;
		}
	}
	// if there are only 0s on the way, true
	return true;
}

// Check on 1 column, linear direction
bool checkLineY(char a[][], int x1, int x2, int y)
{
	// Find point have row max & min
	int minimum = min(x1, x2);
	int maximum = max(x1, x2);
	// run row
	for (int x = min + 1; x < max; x++)
	{
		if (a[x][y] != '0') // if there is at least 1 none-0 char, false
		{
			cout << "die: (" << x << ", " << y << ")" << endl;
			return false;
		}
	}
	// if there are only 0s on the way, true
	return true;
}

bool checkRectX(char a[][], Point pt1, Point pt2)
{
	// Find point having y min & max
	Point pMinCol = pt1, pMaxCol = pt2;
	if (pt1.col > pt2.col)
	{
		pMinCol = pt2;
		pMaxCol = pt1;
	}

	for (int col = pMinCol.col; col <= pMaxCol.col; col++)
	{
		if (col > pMinCol.col && a[pMinCol.row][col] != '0')
			return false;
		// Check 2 line
		if (a[pMaxCol.row][col] == '0' && checkLineY(pMaxCol.row, pMaxCol.row, col) && checkLineX(col, pMaxCol.col, pMaxCol.row))
		{
			cout << "Rect x: (" << pMinCol.row << ", " << pMinCol.col << ") => (" << pMinCol.row << ", " << col << ")  => (" << pMaxCol.row << " ," << col << ")  => (" << pMaxCol.row << " ," << pMaxCol.col << ")" << endl;
			 // If 3 lines then return true
			 return true;
		}
		// If less than 3 lines then return false
		return false;
	}
}

bool checkRectY(char a[][], Point pt1, Point pt2)
{
	// Find point having x min
	Point pMinRow = pt1, pMaxRow = pt2;
	if (pt1.row > pt2.row)
	{
		pMinRow = pt2;
		pMaxRow = pt1;
	}

	for (int row = pMinRow.row; row <= pMaxRow.row; row++)
	{
		if (row > pMinRow.row && a[row][pMinRow.col] != '0')
			return false;
		// Check 2 line
		if (a[row][pMaxRow.col] == '0' && checkLineX(pMinRow.col, pMaxRow.col, row) && checkLineY(row, pMaxRow.row, pMaxRow.col))
		{
			cout << "Rect y: (" << pMinRow.row << ", " << pMinRow.col << ") => (" << row << ", " << pMinRow.col << ") => (" << row << " ," << pMaxRow.col << ")  => (" << pMaxRow.row << " ," << pMaxRow.col << ")" << endl;
			 // If 3 lines then return true
			 return true;
		}
		// If less than 3 lines then return false
		return false;
	}
}

// Check outer line (go out of the main array to "0" zone)
bool checkOuterLineX(char a[][], Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
{
	// Find point having col min
	Point pMinCol = pt1, pMaxCol = pt2;
	if (pt1.col > pt2.col)
	{
		pMinCol = pt2;
		pMaxCol = pt1;
	}
	// Find begin line & begin col
	int col = pMaxCol.col + type;
	int row = pMinCol.row;
	int colFinish = pMaxCol.col;
	if (type == -1)
	{
		colFinish =pMinCol.col;
		col = pMinCol.col + type;
		row = pMaxCol.row;
		cout << "ColFinish = " << colFinish << endl;
	}
	// Find column finish of line
	// Check more
	if ((a[row][colFinish] == '0' || pMinCol.col == pMaxCol.col) && checkLineX(pMinCol.col, pMaxCol.col, row))
	{
		while (a[pMinCol.row][col] == '0' && a[pMaxCol.row][col] == '0')
		{
			if (checkLineY(pMinCol.row, pMaxCol.row, col))
			{
				cout << "(" << pMinCol.row << ", " << pMinCol.col << ") => (" << pMinCol.row << ", " << col << ") => (" << pMaxCol.row << ", " << col << ") => (" << pMaxCol.row << ", " pMaxCol.col << ")" << endl;
				return true;
			}
			col += type;
		}
	}
	return false;
}

// Check outer line (go out of the main array to "0" zone)
bool checkOuterLineY(char a[][], Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
{
	
	Point pMinRow = pt1, pMaxRow = pt2;
	if (pt1.row > pt2.row)
	{
		pMinCol = pt2;
		pMaxCol = pt1;
	}
	
	int row = pMaxRow.row + type;
	int col = pMinRow.col;
	int rowFinish = pMaxRow.row;
	if (type == -1)
	{
		rowFinish =pMinRow.row;
		row = pMinRow.row + type;
		col = pMaxRow.col;
		cout << "RowFinish = " << rowFinish << endl;
	}
	
	
	if ((a[rowFinish][col] == '0' || pMinRow.row == pMaxRow.row) && checkLineY(pMinRow.row, pMaxRow.row, col))
	{
		while (a[row][pMinRow.col] == '0' && a[row][pMaxRow.row] == '0')
		{
			if (checkLineX(pMinRow.col, pMaxRow.col, row))
			{
				cout << "(" << pMinRow.row << ", " << pMinRow.col << ") => (" << row << ", " << pMinRow.col << ") => (" << row << ", " << pMaxRow.col << ") => (" << pMaxRow.row << ", " pMaxRow.col << ")" << endl;
				return true;
			}
			row += type;
		}
	}
	return false;
}


int main()
{
	greating();
	int initNum;
	cin >> initNum;
	if (initNum == 1)
	{
		deleteScreen();
		char a[8][13], x1, y1, x2, y2;
		srand(time(0));

		createArray(a, 8, 13);
		displayArray(a, 8, 13);

		
		system("pause");
	}
	else
		system("pause");
	
	return 0;
}