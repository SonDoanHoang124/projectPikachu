#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iomanip>

const int MAXROW = 7;
const int MAXCOL = 6;

#define ARROW_UP 0x48
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D
#define ARROW_DOWN 0x50
#define ARROW_NONE 0x00
#define ESC_KEY 0x1B
#define ENTER_KEY 0x0D
#define SPACE_KEY 0x20
#define HELP 0x68 // "H" letter

#define BLACK 0
#define AQUA 3
#define RED 4
#define WHITE 7
#define GRAY 8
#define GREEN 10
#define YELLOW 14

using namespace std;

struct Point
{
	int row;
	int col;
	int value;
};
Point pt1;
Point pt2;

void textColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void gotoxy(short x, short y)
{
  static HANDLE h = NULL;  
  if(!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = {x, y};  
  SetConsoleCursorPosition(h,c);
}

void greating()
{
	ifstream ifs;
	ifs.open("Greating.txt", ios::in);

	while(!ifs.eof())
	{
		char c;
		ifs.get(c);
		Sleep(10);
		textColor(YELLOW);
		cout << c;
	}
	
	ifs.close();
}

void congratulating()
{
	ifstream ifs;
	ifs.open("Congratulation.txt", ios::in);

	while(!ifs.eof())
	{
		char c;
		ifs.get(c);
		textColor(YELLOW);
		cout << c;
	}
	
	ifs.close();
}

void deleteScreen()
{
	system("CLS");
}

void createArray(char a[][MAXCOL], int row, int col)
{
	// Set up barrier
	for (int i = 0; i < col; i++)
		a[0][i] = ' ';
	for (int i = 0; i < col; i++)
		a[row - 1][i] = ' ';
	for (int i = 0; i < row; i++)
		a[i][0] = ' ';
	for (int i = 0; i < row; i++)
		a[i][col - 1] = ' ';

	for (int i = 1; i < row - 1; i++)
		for(int j = 1; j < col - 1; j++)			
			a[i][j] = 49 + (rand() % 9);
}

/*int** getArray(int row, int col)
{
	int** arr = 0;
	arr = new int*[row];
	for (int r = 0; r < row; r++)
	{
		arr[r] = new int[col];
		for (int c = 0; c < col; c++)
		{
			arr[0][c] = 0;
			arr[row - 1][c] = 0;
			arr[r][0] = 0;
			arr[r][col - 1] = 0;
			if (r != 0 && r != row - 1 && c != 0 && c != col -1)
				arr[r][c] = 1 + (rand() % 9);
		}
	}
}*/

/*void displayArray(int a[][13], int row, int col)
{
	
	for (int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}*/

void displayArray(char a, int row, int col)
{
    int nRow = 5, mCol = 11; 
	for (int i = 1; i <= nRow; i++) // hàng -> y
	{
		for (int j = 1; j <= mCol; j++)// cột -> x
		{
			if ((i == (nRow/2+1)) && (j == (mCol/2+1)))
			{
				gotoxy(short(col*mCol+j) , short(row*nRow+i));// đi tới ô (x*mCol+j, y*nRow+i)
				textColor(GREEN);
				cout << a;
			}
			else if ((i == 1 || i == nRow) && (1 < j) && (j < mCol))
			{
				gotoxy(short(col*mCol+j) , short(row*nRow+i) );
				textColor(WHITE);
				cout << "-";
			}
			else if ((j == 1 || j == mCol) && (1 < i) && (i < nRow))
			{
				gotoxy(short(col*mCol+j) , short(row*nRow+i) );
				textColor(WHITE);
				cout << "|";
			}
		}
	}
}

// Check on 1 line, linear direction
bool checkLineX(char a[][MAXCOL], int y1, int y2, int x)
{
	// Find point have column max & min
	int minimum = min(y1, y2);
	int maximum = max(y1, y2);
	// run column
	for (int y = minimum + 1; y < maximum; y++)
	{
		if (a[x][y] != ' ') // if there is at least 1 none-0 char, false
		{
			//cout << "die row: column can't run (" << x << ", " << y << ")" << endl;
			return false;
		}
	}
	// if there are only " "s on the way, true
	//cout << "(" << x << ", " << y1 << ") => (" << x << ", " << y2 << "): OK" << endl;
	return true;
}

// Check on 1 column, linear direction
bool checkLineY(char a[][MAXCOL], int x1, int x2, int y)
{
	// Find point have row max & min
	int minimum = min(x1, x2);
	int maximum = max(x1, x2);
	// run row
	for (int x = minimum + 1; x < maximum; x++)
	{
		if (a[x][y] != ' ') // if there is at least 1 none-" " char, false
		{
			//cout << "die col: row can't run (" << x << ", " << y << ")" << endl;
			return false;
		}
	}
	// if there are only " "s on the way, true
	//cout << "(" << x1 << ", " << y << ") => (" << x2 << ", " << y << "): OK" << endl;
	return true;
}

bool checkRectX(char a[][MAXCOL], Point pt1, Point pt2)
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
		if (col > pMinCol.col && a[pMinCol.row][col] != ' ')
			return false;
		// Check 2 line
		if (a[pMaxCol.row][col] == ' ' && checkLineY(a, pMaxCol.row, pMaxCol.row, col) && checkLineX(a, col, pMaxCol.col, pMaxCol.row))
		{
			//cout << "Rect x: (" << pMinCol.row << ", " << pMinCol.col << ") => (" << pMinCol.row << ", " << col << ")  => (" << pMaxCol.row << " ," << col << ")  => (" << pMaxCol.row << " ," << pMaxCol.col << ")" << endl;
			 // If 3 lines then return true
			 return true;
		}
		// If less than 3 lines then return false
		return false;
	}
}

bool checkRectY(char a[][MAXCOL], Point pt1, Point pt2)
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
		if (row > pMinRow.row && a[row][pMinRow.col] != ' ')
			return false;
		// Check 2 line
		if (a[row][pMaxRow.col] == ' ' && checkLineX(a, pMinRow.col, pMaxRow.col, row) && checkLineY(a, row, pMaxRow.row, pMaxRow.col))
		{
			//cout << "Rect y: (" << pMinRow.row << ", " << pMinRow.col << ") => (" << row << ", " << pMinRow.col << ") => (" << row << " ," << pMaxRow.col << ")  => (" << pMaxRow.row << " ," << pMaxRow.col << ")" << endl;
			 // If 3 lines then return true
			 return true;
		}
		// If less than 3 lines then return false
		return false;
	}
}

// Check outer line (go out of the main array to "0" zone)
bool checkOuterLineX(char a[][MAXCOL], Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
{
	// Find point having col min
	Point pMinCol = pt1, pMaxCol = pt2;
	if (pt1.col > pt2.col)
	{
		pMinCol = pt2;
		pMaxCol = pt1;
	}
	// Find begin line & begin col
	int col = pMaxCol.col + direction;
	int row = pMinCol.row;
	int colFinish = pMaxCol.col;
	if (direction == -1)
	{
		colFinish =pMinCol.col;
		col = pMinCol.col + direction;
		row = pMaxCol.row;
		//cout << "ColFinish = " << colFinish << endl;
	}
	// Find column finish of line
	// Check more
	if ((a[row][colFinish] == ' ' || pMinCol.col == pMaxCol.col) && checkLineX(a, pMinCol.col, pMaxCol.col, row))
	{
		while (a[pMinCol.row][col] == ' ' && a[pMaxCol.row][col] == ' ')
		{
			if (checkLineY(a, pMinCol.row, pMaxCol.row, col))
			{
				//cout << "(" << pMinCol.row << ", " << pMinCol.col << ") => (" << pMinCol.row << ", " << col << ") => (" << pMaxCol.row << ", " << col << ") => (" << pMaxCol.row << ", " << pMaxCol.col << ")" << endl;
				return true;
			}
			col += direction;
		}
	}
	return false;
}

// Check outer line (go out of the main array to "0" zone)
bool checkOuterLineY(char a[][MAXCOL], Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
{
	
	Point pMinRow = pt1, pMaxRow = pt2;
	if (pt1.row > pt2.row)
	{
		pMinRow = pt2;
		pMaxRow = pt1;
	}
	
	int row = pMaxRow.row + direction;
	int col = pMinRow.col;
	int rowFinish = pMaxRow.row;
	if (direction == -1)
	{
		rowFinish =pMinRow.row;
		row = pMinRow.row + direction;
		col = pMaxRow.col;
		//cout << "RowFinish = " << rowFinish << endl;
	}
	

	if ((a[rowFinish][col] == ' ' || pMinRow.row == pMaxRow.row) && checkLineY(a, pMinRow.row, pMaxRow.row, col))
	{
		while (a[row][pMinRow.col] == ' ' && a[row][pMaxRow.row] == ' ')
		{
			if (checkLineX(a, pMinRow.col, pMaxRow.col, row))
			{
				//cout << "(" << pMinRow.row << ", " << pMinRow.col << ") => (" << row << ", " << pMinRow.col << ") => (" << row << ", " << pMaxRow.col << ") => (" << pMaxRow.row << ", " << pMaxRow.col << ")" << endl;
				return true;
			}
			row += direction;
		}
	}
	return false;
}

//void deleteBoard(char a[][MAXCOL], Point pt1, Point pt2)
//{

//}

void check2Points(char a[][MAXCOL], Point pt1, Point pt2)
{
	if ((pt1.row != pt2.row || pt1.col != pt2.col) && a[pt1.row][pt1.col] == a[pt2.row][pt2.col])
	{
		// Check line with row
		if(pt1.row == pt2.row)
		{
			//cout << "Row x: ";
			if (checkLineX(a, pt1.col, pt2.col, pt1.row))
			{
				//cout << "OK\n";
				a[pt1.row][pt1.col] = ' ';
				a[pt2.row][pt2.col] = ' ';
			}
		}
		// Check line with column
		if(pt1.col == pt2.col)
		{
			//cout << "Col y: ";
			if (checkLineY(a, pt1.row, pt2.row, pt1.col))
			{
				//cout << "OK col y\n";
				a[pt1.row][pt1.col] = ' ';
				a[pt2.row][pt2.col] = ' ';
			}
		}
		// Check in rectangle
		if (checkRectX(a, pt1, pt2))
		{
			//cout << "Rect x: OK\n";
			a[pt1.row][pt1.col] = ' ';
			a[pt2.row][pt2.col] = ' ';
		}
		// Check in rectangle
		if (checkRectY(a, pt1, pt2))
		{
			//cout << "Rect y: OK\n";
			a[pt1.row][pt1.col] = ' ';
			a[pt2.row][pt2.col] = ' ';
		}
		// Check more right
		if (checkOuterLineX(a, pt1, pt2, 1))
		{
			//cout << "Go right OK\n";
			a[pt1.row][pt1.col] = ' ';
			a[pt2.row][pt2.col] = ' ';
		}
		// Check more left
		if (checkOuterLineX(a, pt1, pt2, -1))
		{
			//cout << "Go left OK\n";
			a[pt1.row][pt1.col] = ' ';
			a[pt2.row][pt2.col] = ' ';
		}
		// Check more down
		if (checkOuterLineY(a, pt1, pt2, 1))
		{
			//cout << "Go down OK\n";
			a[pt1.row][pt1.col] = ' ';
			a[pt2.row][pt2.col] = ' ';
		}
		// Check more up
		if (checkOuterLineY(a, pt1, pt2, -1))
		{
			//cout << "Go up OK\n";
			a[pt1.row][pt1.col] = ' ';
			a[pt2.row][pt2.col] = ' ';
		}
	}	
}

bool checkEndGame(char a[][MAXCOL], char b[][MAXCOL], int row, int col)
{
	for (int i = 1; i < row - 1; i++)
	{
		for (int j = 1; j < col - 1; j++)
			if (a[i][j] != b[i][j])
				return false;
	}
	return true;
}

int main()
{
	greating();
	int initNum;
	cin >> initNum;
	if (initNum == 1)
	{
		deleteScreen();
		int x1, y1, x2, y2;
		char a[MAXROW][MAXCOL] = {
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ','1','1',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ','1',' '},
			{' ','1',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '}
		};
		char b[MAXROW][MAXCOL] = {
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '}
		};

		srand(time(0));
		//createArray(a, MAXROW, MAXCOL);

		while (true)
		{	
			if (checkEndGame(a, b, MAXROW, MAXCOL))
				break;
			//displayArray(a, 8, 8);
			for (int i = 0; i < MAXROW; i++)
			{
				for (int j = 0; j < MAXCOL; j++)	
					displayArray(a[i][j], i, j);
			}
			cout << "\n\n";
			for (int i = 0; i < MAXROW; i++)
			{
				for (int j = 0; j < MAXCOL; j++)	
					cout << a[i][j] << "   ";
				cout << "\n";	
			}		

			gotoxy(0, 0);
			gotoxy(70, 0);
			cout << "Input x1, y1: \n";
			gotoxy(70, 1);
			cin >> x1;
			gotoxy(70, 2);
			cin >> y1;
			pt1.row = x1;
			pt1.col = y1;
			//pt1.value = b[pt1.row][pt1.col];

			gotoxy(70, 4);
			cout << "Input x2, y2: \n";
			gotoxy(70, 5);
			cin >> x2;
			gotoxy(70, 6);
			cin >> y2;
			pt2.row = x2;
			pt2.col = y2;
			//pt2.value = b[pt2.row][pt2.col];

			check2Points(a, pt1, pt2);
			
			//displayArray(b, 5, 5);

			gotoxy(70, 8);
			system("pause");
			deleteScreen();
		}
		congratulating();
		gotoxy(0, 0);
		gotoxy(50, 8);
		system("pause");
	}
	else
		system("pause");
	
	return 0;
}