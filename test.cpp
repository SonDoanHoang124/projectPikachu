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

void createArray(int a[][13], int row, int col)
{
	// Set up barrier
	for (int i = 0; i < col; i++)
		a[0][i] = 0;
	for (int i = 0; i < col; i++)
		a[row - 1][i] = 0;
	for (int i = 0; i < row; i++)
		a[i][0] = 0;
	for (int i = 0; i < row; i++)
		a[i][col - 1] = 0;

	for (int i = 1; i < row - 1; i++)
		for(int j = 1; j < col - 1; j++)
			a[i][j] = 1 + (rand() % 9);
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

void displayArray(int a[][13], int row, int col)
{
	
	for (int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
			cout << a[i][j] << " ";
		cout << endl;
	}
}

// Check on 1 line, linear direction
bool checkLineX(int a[][13], int y1, int y2, int x)
{
	// Find point have column max & min
	int minimum = min(y1, y2);
	int maximum = max(y1, y2);
	// run column
	for (int y = minimum + 1; y < maximum; y++)
	{
		if (a[x][y] != 0) // if there is at least 1 none-0 char, false
		{
			cout << "die row: column can't run (" << x << ", " << y << ")" << endl;
			return false;
		}
	}
	// if there are only 0s on the way, true
	return true;
}

// Check on 1 column, linear direction
bool checkLineY(int a[][13], int x1, int x2, int y)
{
	// Find point have row max & min
	int minimum = min(x1, x2);
	int maximum = max(x1, x2);
	// run row
	for (int x = minimum + 1; x < maximum; x++)
	{
		if (a[x][y] != 0) // if there is at least 1 none-0 char, false
		{
			cout << "die col: row can't run (" << x << ", " << y << ")" << endl;
			return false;
		}
	}
	// if there are only 0s on the way, true
	return true;
}

bool checkRectX(int a[][13], Point pt1, Point pt2)
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
		if (col > pMinCol.col && a[pMinCol.row][col] != 0)
			return false;
		// Check 2 line
		if (a[pMaxCol.row][col] == 0 && checkLineY(a, pMaxCol.row, pMaxCol.row, col) && checkLineX(a, col, pMaxCol.col, pMaxCol.row))
		{
			cout << "Rect x: (" << pMinCol.row << ", " << pMinCol.col << ") => (" << pMinCol.row << ", " << col << ")  => (" << pMaxCol.row << " ," << col << ")  => (" << pMaxCol.row << " ," << pMaxCol.col << ")" << endl;
			 // If 3 lines then return true
			 return true;
		}
		// If less than 3 lines then return false
		return false;
	}
}

bool checkRectY(int a[][13], Point pt1, Point pt2)
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
		if (row > pMinRow.row && a[row][pMinRow.col] != 0)
			return false;
		// Check 2 line
		if (a[row][pMaxRow.col] == 0 && checkLineX(a, pMinRow.col, pMaxRow.col, row) && checkLineY(a, row, pMaxRow.row, pMaxRow.col))
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
bool checkOuterLineX(int a[][13], Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
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
		cout << "ColFinish = " << colFinish << endl;
	}
	// Find column finish of line
	// Check more
	if ((a[row][colFinish] == 0 || pMinCol.col == pMaxCol.col) && checkLineX(a, pMinCol.col, pMaxCol.col, row))
	{
		while (a[pMinCol.row][col] == 0 && a[pMaxCol.row][col] == 0)
		{
			if (checkLineY(a, pMinCol.row, pMaxCol.row, col))
			{
				cout << "(" << pMinCol.row << ", " << pMinCol.col << ") => (" << pMinCol.row << ", " << col << ") => (" << pMaxCol.row << ", " << col << ") => (" << pMaxCol.row << ", " << pMaxCol.col << ")" << endl;
				return true;
			}
			col += direction;
		}
	}
	return false;
}

// Check outer line (go out of the main array to "0" zone)
bool checkOuterLineY(int a[][13], Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
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
		cout << "RowFinish = " << rowFinish << endl;
	}
	

	if ((a[rowFinish][col] == 0 || pMinRow.row == pMaxRow.row) && checkLineY(a, pMinRow.row, pMaxRow.row, col))
	{
		while (a[row][pMinRow.col] == 0 && a[row][pMaxRow.row] == 0)
		{
			if (checkLineX(a, pMinRow.col, pMaxRow.col, row))
			{
				cout << "(" << pMinRow.row << ", " << pMinRow.col << ") => (" << row << ", " << pMinRow.col << ") => (" << row << ", " << pMaxRow.col << ") => (" << pMaxRow.row << ", " << pMaxRow.col << ")" << endl;
				return true;
			}
			row += direction;
		}
	}
	return false;
}

void check2Points(int a[][13], Point pt1, Point pt2)
{
	if ((pt1.row != pt2.row || pt1.col != pt2.col) && a[pt1.row][pt1.col] == a[pt2.row][pt2.col])
	{
		// Check line with row
		if(pt1.row == pt2.row)
		{
			cout << "Row x: ";
			if (checkLineX(a, pt1.col, pt2.col, pt1.row))
			{
				cout << "OK\n";
				
			}
		}
		// Check line with column
		if(pt1.col == pt2.col)
		{
			cout << "Col y: ";
			if (checkLineY(a, pt1.row, pt2.row, pt1.col))
			{
				cout << "OK col y\n";
				
			}
		}
		// Check in rectangle
		if (checkRectX(a, pt1, pt2))
		{
			cout << "Rect x: OK\n";
			
		}
		// Check in rectangle
		if (checkRectY(a, pt1, pt2))
		{
			cout << "Rect y: OK\n";
			
		}
		// Check more right
		if (checkOuterLineX(a, pt1, pt2, 1))
		{
			cout << "Go right OK\n";
			
		}
		// Check more left
		if (checkOuterLineX(a, pt1, pt2, -1))
		{
			cout << "Go left OK\n";
			
		}
		// Check more down
		if (checkOuterLineY(a, pt1, pt2, 1))
		{
			cout << "Go down OK\n";
			
		}
		// Check more up
		if (checkOuterLineY(a, pt1, pt2, -1))
		{
			cout << "Go up OK\n";
			
		}
	}
	
}

int main()
{
	greating();
	int initNum;
	cin >> initNum;
	if (initNum == 1)
	{
		deleteScreen();
		int a[8][13], x1, y1, x2, y2;
		int b[8][13] =  {{0,0,0,0,0,0,0,0,0,0,0,0,0}
						,{0,1,2,3,0,0,0,0,0,0,0,0,0}
						,{0,0,0,0,0,0,0,0,0,1,2,3,0}
						,{0,0,0,0,0,0,0,0,0,0,0,0,0}
						,{0,0,0,0,0,0,1,0,0,0,0,0,0}
						,{0,0,0,0,0,0,0,0,0,0,0,0,0}
						,{0,0,2,0,0,0,0,0,0,0,0,3,0}
						,{0,0,0,0,0,0,0,0,0,0,0,0,0}};
		srand(time(0));

		createArray(a, 8, 13);
		displayArray(b, 8, 13);

		cout << "Input x1, y1: \n";
		cin >> x1 >> y1;
		pt1.row = x1;
		pt1.col = y1;
		pt1.value = a[pt1.row][pt1.col];

		cout << "Input x2, y2: \n";
		cin >> x2 >> y2;
		pt2.row = x2;
		pt2.col = y2;
		pt2.value = a[pt2.row][pt2.col];

		check2Points(b, pt1, pt2);
		
		//displayArray(a, 8, 13);

		system("pause");
	}
	else
		system("pause");
	
	return 0;
}