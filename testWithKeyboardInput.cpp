#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <mmsystem.h>

const int MAXROW = 6;
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
#define PINK 13
#define YELLOW 14

using namespace std;

struct Point
{
	int row;
	int col;
	bool isSelected;
};
Point pt1;
Point pt2;

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void HideScrollbar()
{
    HWND hWnd = GetConsoleWindow();
    ShowScrollBar(hWnd, SB_BOTH, false);
}

void textColor(int backgroundColor, int textColor)
{
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorCode = backgroundColor * 16 + textColor;
	SetConsoleTextAttribute(color, colorCode);
}

void gotoxy(SHORT x, SHORT y)
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
		//Sleep(1);
		textColor(BLACK, YELLOW);
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
		textColor(BLACK, YELLOW);
		cout << c;
	}
	
	ifs.close();
}

unsigned char getKey()
{
    unsigned char ch = getch();
    if (ch == 0x00)
    {
        // first char is a zero so lets look at the next char
        ch = getch();
        switch (ch)
        {
        case ARROW_UP:
            return ch;
        case ARROW_LEFT:
            return ch;
        case ARROW_RIGHT:
            return ch;
        case ARROW_DOWN:
            return ch;
        default:
            return ARROW_NONE;
        }
    }
    else
    {
        // When user use wasd keys
        switch (ch)
        {
        case 'w':
            return ARROW_UP;
        case 'a':
            return ARROW_LEFT;
        case 'd':
            return ARROW_RIGHT;
        case 's':
            return ARROW_DOWN;
        case 'h':
            return HELP;
        }
    }

    // Enter key is the same as space key
    if (ch == ENTER_KEY || ch == SPACE_KEY)
    {
        return ENTER_KEY;
    }

    if (ch == ESC_KEY)
    {
        return ch;
    }

    return ch;
}

void deleteScreen()
{
	system("CLS");
}

/*void createArray(char a[][MAXCOL], int row, int col)
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
			a[i][j] = 49 + (rand() % 4);
}*/

void createArray(char **&a, int row, int col)
{
	// Create a matrix for the given row and col
    a = new char*[row];
	
	for (int i = 0; i < row; i++)
        a[i] = new char [col];

	// Set up barrier
	for (int i = 0; i < col; i++)
		a[0][i] = ' ';
	for (int i = 0; i < col; i++)
		a[row - 1][i] = ' ';
	for (int i = 0; i < row; i++)
		a[i][0] = ' ';
	for (int i = 0; i < row; i++)
		a[i][col - 1] = ' ';

	// Set up randomness with even repeats
	char set[(row - 2) * (col - 2)];
	for (int i = 0; i < (row - 2) * (col - 2); i++)
	{
		if(i % 2 == 0)	set[i] = 65 + i;
		else	set[i] = 66 + i;
		//set[i] = 65 + (rand() % ((row - 2) * (col - 2) / 2)) ;
	}
	int check = 11 + rand() % 20;

	for (int i = 1; i < (row - 2) * (col - 2); i++)
	{
	    for (int j = 0; i < i; j++)
	        swap(set[i], set[j]);
	}
    
	// Put random numbers into array
	int runner = 0;
	for (int i = 1; i < row - 1; i++)
	{
		for(int j = 1; j < col - 1; j++)
		{
			a[i][j] = set[runner];
		    runner++;
		}
	}
}

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

void displayArray(char a,/*char **&table,*/ int row, int col)
{
    int nRow = 5, mCol = 11; 

	/*table = new char*[row * nRow];
	for (int i = 0; i < col; i++)
		table[i] = new char [col * mCol];*/
		
	for (int i = 1; i <= nRow; i++) // hàng -> y
	{
		for (int j = 1; j <= mCol; j++)// cột -> x
		{
			if ((i == (nRow/2+1)) && (j == (mCol/2+1)))
			{
				gotoxy(short(col*mCol+j) , short(row*nRow+i));// đi tới ô (x*mCol+j, y*nRow+i)
				textColor(BLACK, GREEN);
				cout << a;
			}
			else if ((i == 1 || i == nRow) && (1 < j) && (j < mCol))
			{
				gotoxy(short(col*mCol+j) , short(row*nRow+i) );
				textColor(BLACK, WHITE);
				cout << "-";
			}
			else if ((j == 1 || j == mCol) && (1 < i) && (i < nRow))
			{
				gotoxy(short(col*mCol+j) , short(row*nRow+i) );
				textColor(BLACK, WHITE);
				cout << "|";
			}
		}
	}
}

void overlapCube(char **a,/* int row, int col,*/ int bgColor, int txtColor, Point A)
{
	//textColor(BLACK, BLACK);
	int nRow = 5, mCol = 11; 
	gotoxy(0, 0);
	for (int i = 1; i <= nRow; i++) // hàng -> y
	{
		for (int j = 1; j <= mCol; j++)// cột -> x
		{
			if ((i == (nRow/2+1)) && (j == (mCol/2+1)))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i));// đi tới ô (x*mCol+j, y*nRow+i)
				textColor(bgColor, txtColor);
				a[A.row][A.col] = ' ';
				cout << a[A.row][A.col];
			}
			else if ((i == 1 || i == nRow) && (1 < j) && (j < mCol))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(bgColor, txtColor);
				cout << "-";
			}
			else if ((j == 1 || j == mCol) && (1 < i) && (i < nRow))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(bgColor, txtColor);
				cout << "|";
			}
			else if (i > 1 && i < nRow && j > 1 && j < mCol)
			{
				textColor(BLACK, BLACK);
				cout << " ";
			}
		}
	}
	textColor(BLACK, WHITE);
}

void selectedCube(char **a,int bgColor, int txtColor, Point A)
{
	
	//textColor(BLACK, BLACK);
	int nRow = 5, mCol = 11; 
	//gotoxy(0, 0);
	for (int i = 1; i <= nRow; i++) // hàng -> y
	{
		for (int j = 1; j <= mCol; j++)// cột -> x
		{
			if ((i == (nRow/2+1)) && (j == (mCol/2+1)))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i));// đi tới ô (x*mCol+j, y*nRow+i)
				textColor(bgColor, txtColor);
				cout << a[A.row][A.col];
			}
			else if ((i == 1 || i == nRow) && (1 < j) && (j < mCol))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(BLACK, WHITE);
				cout << "-";
			}
			else if ((j == 1 || j == mCol) && (1 < i) && (i < nRow))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(BLACK, WHITE);
				cout << "|";
			}
			else if (i > 1 && i < nRow && j > 1 && j < mCol)
			{
				textColor(WHITE, WHITE);
				cout << " ";
			}
		}
	}
	textColor(BLACK, WHITE);
}

void deSelectedCube(char **a,int bgColor, int txtColor, Point A)
{
	
	//textColor(BLACK, BLACK);
	int nRow = 5, mCol = 11; 
	//gotoxy(0, 0);
	for (int i = 1; i <= nRow; i++) // hàng -> y
	{
		for (int j = 1; j <= mCol; j++)// cột -> x
		{
			if ((i == (nRow/2+1)) && (j == (mCol/2+1)))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i));// đi tới ô (x*mCol+j, y*nRow+i)
				textColor(bgColor, txtColor);
				cout << a[A.row][A.col];
			}
			else if ((i == 1 || i == nRow) && (1 < j) && (j < mCol))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(BLACK, WHITE);
				cout << "-";
			}
			else if ((j == 1 || j == mCol) && (1 < i) && (i < nRow))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(BLACK, WHITE);
				cout << "|";
			}
			else if (i > 1 && i < nRow && j > 1 && j < mCol)
			{
				textColor(BLACK, BLACK);
				cout << " ";
			}
		}
	}
	textColor(BLACK, WHITE);
}

void reSelectedCube(char **a,int bgColor, int txtColor, Point A)
{
	
	//textColor(BLACK, BLACK);
	int nRow = 5, mCol = 11; 
	//gotoxy(0, 0);
	for (int i = 1; i <= nRow; i++) // hàng -> y
	{
		for (int j = 1; j <= mCol; j++)// cột -> x
		{
			if ((i == (nRow/2+1)) && (j == (mCol/2+1)))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i));// đi tới ô (x*mCol+j, y*nRow+i)
				textColor(bgColor, txtColor);
				cout << a[A.row][A.col];
			}
			else if ((i == 1 || i == nRow) && (1 < j) && (j < mCol))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(BLACK, YELLOW);
				cout << "-";
			}
			else if ((j == 1 || j == mCol) && (1 < i) && (i < nRow))
			{
				gotoxy(SHORT(A.col*mCol+j) , SHORT(A.row*nRow+i) );
				textColor(BLACK, YELLOW);
				cout << "|";
			}
			else if (i > 1 && i < nRow && j > 1 && j < mCol)
			{
				textColor(bgColor, txtColor);
				cout << " ";
			}
		}
	}
	textColor(BLACK, WHITE);
}

void deleteBarrier(char **a)
{
	// Delete extra boxes
	Point barrier;
	for (int i = 0; i < MAXCOL; i++)
	{
		barrier.row = 0;
		barrier.col = i;
		overlapCube(a, BLACK, BLACK, barrier);
	}
	for (int i = 0; i < MAXCOL; i++)
	{
		barrier.row = MAXROW - 1;
		barrier.col = i;
		overlapCube(a, BLACK, BLACK, barrier);
	}
	for (int i = 0; i < MAXROW; i++)
	{
		barrier.row = i;
		barrier.col = 0;
		overlapCube(a, BLACK, BLACK, barrier);
	}
	for (int i = 0; i < MAXROW; i++)
	{
		barrier.row = i;
		barrier.col = MAXCOL - 1;
		overlapCube(a, BLACK, BLACK, barrier);
	}
}

// Check on 1 line, linear direction
bool checkLineX(char **a, int y1, int y2, int x)
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
bool checkLineY(char **a, int x1, int x2, int y)
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

bool checkRectX(char **a, Point pt1, Point pt2)
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
		
	}
	// If less than 3 lines then return false
	return false;
}

bool checkRectY(char **a, Point pt1, Point pt2)
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
	}
	// If less than 3 lines then return false
	return false;
}

// Check outer line (go out of the main array to "0" zone)
bool checkOuterLineX(char **a, Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
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
bool checkOuterLineY(char **a, Point pt1, Point pt2, int direction)  // direction return 1 (go forward) and -1 (go backward)
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

void check2Points(char **a, Point pt1, Point pt2)
{
	if ((pt1.row != pt2.row || pt1.col != pt2.col) && a[pt1.row][pt1.col] == a[pt2.row][pt2.col])
	{
		// Check line with row
		if(pt1.row == pt2.row)
		{
			if (checkLineX(a, pt1.col, pt2.col, pt1.row))
			{
				//overlapCube(a, BLACK, BLACK, pt1);
				//overlapCube(a, BLACK, BLACK, pt2);

				//deSelectedCube(a, BLACK, GREEN, pt1);
				//deSelectedCube(a, BLACK, GREEN, pt2);
				reSelectedCube(a, GREEN, BLACK, pt1);
				reSelectedCube(a, GREEN, BLACK, pt2);
			}
		}
		// Check line with column
		else if(pt1.col == pt2.col)
		{
			if (checkLineY(a, pt1.row, pt2.row, pt1.col))
			{
				//overlapCube(a, BLACK, BLACK, pt1);
				//overlapCube(a, BLACK, BLACK, pt2);

				//deSelectedCube(a, BLACK, GREEN, pt1);
				//deSelectedCube(a, BLACK, GREEN, pt2);
				reSelectedCube(a, GREEN, BLACK, pt1);
				reSelectedCube(a, GREEN, BLACK, pt2);
			}
		}
		// Check in rectangle
		else if (checkRectX(a, pt1, pt2))
		{

			//overlapCube(a, BLACK, BLACK, pt1);
			//overlapCube(a, BLACK, BLACK, pt2);

			//deSelectedCube(a, BLACK, GREEN, pt1);
			//deSelectedCube(a, BLACK, GREEN, pt2);
			reSelectedCube(a, GREEN, BLACK, pt1);
			reSelectedCube(a, GREEN, BLACK, pt2);
		}
		// Check in rectangle
		else if (checkRectY(a, pt1, pt2))
		{
			//overlapCube(a, BLACK, BLACK, pt1);
			//overlapCube(a, BLACK, BLACK, pt2);

			//deSelectedCube(a, BLACK, GREEN, pt1);
			//deSelectedCube(a, BLACK, GREEN, pt2);
			reSelectedCube(a, GREEN, BLACK, pt1);
			reSelectedCube(a, GREEN, BLACK, pt2);
		}
		// Check more right
		else if (checkOuterLineX(a, pt1, pt2, 1))
		{
			//overlapCube(a, BLACK, BLACK, pt1);
			//overlapCube(a, BLACK, BLACK, pt2);

			//deSelectedCube(a, BLACK, GREEN, pt1);
			//deSelectedCube(a, BLACK, GREEN, pt2);
			reSelectedCube(a, GREEN, BLACK, pt1);
			reSelectedCube(a, GREEN, BLACK, pt2);
		}
		// Check more left
		else if (checkOuterLineX(a, pt1, pt2, -1))
		{
			//overlapCube(a, BLACK, BLACK, pt1);
			//overlapCube(a, BLACK, BLACK, pt2);

			//deSelectedCube(a, BLACK, GREEN, pt1);
			//deSelectedCube(a, BLACK, GREEN, pt2);
			reSelectedCube(a, GREEN, BLACK, pt1);
			reSelectedCube(a, GREEN, BLACK, pt2);
		}
		// Check more down
		else if (checkOuterLineY(a, pt1, pt2, 1))
		{
			//overlapCube(a, BLACK, BLACK, pt1);
			//overlapCube(a, BLACK, BLACK, pt2);

			//deSelectedCube(a, BLACK, GREEN, pt1);
			//deSelectedCube(a, BLACK, GREEN, pt2);
			reSelectedCube(a, GREEN, BLACK, pt1);
			reSelectedCube(a, GREEN, BLACK, pt2);
		}
		// Check more up
		else if (checkOuterLineY(a, pt1, pt2, -1))
		{
			//overlapCube(a, BLACK, BLACK, pt1);
			//overlapCube(a, BLACK, BLACK, pt2);

			//deSelectedCube(a, BLACK, GREEN, pt1);
			//deSelectedCube(a, BLACK, GREEN, pt2);
			reSelectedCube(a, GREEN, BLACK, pt1);
			reSelectedCube(a, GREEN, BLACK, pt2);
		}
	}
	else 
	{
		//overlapCube(a, BLACK, BLACK, pt1);
		//overlapCube(a, BLACK, BLACK, pt2);

		//deSelectedCube(a, BLACK, GREEN, pt1);
		//deSelectedCube(a, BLACK, GREEN, pt2);
		reSelectedCube(a, RED, BLACK, pt1);
		reSelectedCube(a, RED, BLACK, pt2);
		Sleep(800);
		reSelectedCube(a, WHITE, BLACK, pt1);
		reSelectedCube(a, BLACK, WHITE, pt2);
	}	
}

bool checkEndGame(char **a, char b[][MAXCOL], int row, int col)
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
	resizeConsole(900, 600);
	HideScrollbar();
	greating();
	// Play BG MUSIC
    //PlaySound(TEXT("music\\originalKhorobeiniki.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);	
	int initNum;
	cin >> initNum;
	if (initNum == 1)
	{
		deleteScreen();
		Sleep(500);
		// Play BG MUSIC
    	//PlaySound(TEXT("music\\katyusha8bit.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
		int x1, y1, x2, y2;
		//char a[MAXROW][MAXCOL];
		char **a = NULL;
		char inputKey = ENTER_KEY;

		char b[MAXROW][MAXCOL] = {
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' '}
		};


		srand(time(0));
		createArray(a, MAXROW, MAXCOL);
		for (int i = 0; i < MAXROW; i++)
			for (int j = 0; j < MAXCOL; j++)
				displayArray(a[i][j], i, j);
		deleteBarrier(a);

		// Always choosing the very 1st box.
		Point Selected;
		Selected.row = 1;
		Selected.col = 1;
		Selected.isSelected = false;
		selectedCube(a, WHITE, RED, Selected);

		while ((inputKey = getKey()) != ESC_KEY)
		{	
			if (checkEndGame(a, b, MAXROW, MAXCOL))
				break;
			// Refresh input lines
			/*gotoxy(70, 1);
			textColor(BLACK, BLACK);
			cout << " ";
			gotoxy(70, 2);
			cout << " ";
			gotoxy(70, 5);
			cout << " ";
			gotoxy(70, 6);
			cout << " ";
			gotoxy(70, 8);
			cout << "                                ";

			gotoxy(70, 0);
			textColor(BLACK, AQUA);
			cout << "Input x1, y1: \n";
			gotoxy(70, 1);
			textColor(BLACK, YELLOW);
			cin >> x1;
			gotoxy(70, 2);
			cin >> y1;
			pt1.row = x1;
			pt1.col = y1;
			//pt1.value = b[pt1.row][pt1.col];

			gotoxy(70, 4);
			textColor(BLACK, AQUA);
			cout << "Input x2, y2: \n";
			gotoxy(70, 5);
			textColor(BLACK, YELLOW);
			cin >> x2;
			gotoxy(70, 6);
			cin >> y2;
			pt2.row = x2;
			pt2.col = y2;
			//pt2.value = b[pt2.row][pt2.col];

			gotoxy(70, 8);
			textColor(BLACK, RED);
			system("pause");*/

			switch (inputKey)
			{
			case ARROW_UP:
				if (Selected.row > 1 && Selected.row <= MAXROW - 2)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.row -= 1;
					selectedCube(a, WHITE, RED, Selected);
				}
				else if (Selected.row == 1)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.row = MAXROW - 2;
					selectedCube(a, WHITE, RED, Selected);
				}
				
				break;
			case ARROW_DOWN:
				if (Selected.row >= 1 && Selected.row < MAXROW - 2)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.row += 1;
					selectedCube(a, WHITE, RED, Selected);
				}
				else if (Selected.row == MAXROW - 2)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.row = 1;
					selectedCube(a, WHITE, RED, Selected);
				}
				break;
			case ARROW_LEFT:
				if (Selected.col > 1 && Selected.col <= MAXCOL - 2)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.col -= 1;
					selectedCube(a, WHITE, RED, Selected);
				}
				else if (Selected.col == 1)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.col = MAXCOL - 2;
					selectedCube(a, WHITE, RED, Selected);
				}
				break;
			case ARROW_RIGHT:
				if (Selected.col >= 1 && Selected.col < MAXCOL - 2)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.col += 1;
					selectedCube(a, WHITE, RED, Selected);
				}
				else if (Selected.col == MAXCOL - 2)
				{
					deSelectedCube(a, BLACK, GREEN, Selected);
					Selected.col = 1;
					selectedCube(a, WHITE, RED, Selected);
				}
				break;
			case HELP:
				//helpMove(matrix, difficulty);
				break;
			case ENTER_KEY:
				if (a[Selected.row][Selected.col] == ' ')
					break;
				if (!pt1.isSelected)
				{
					pt1 = Selected;
					pt1.isSelected = true;
					
					deSelectedCube(a, BLACK, GREEN, pt1);
					reSelectedCube(a, AQUA, YELLOW, pt1);
					//Selected.row++;
					Selected.col++;
					selectedCube(a, WHITE, RED, Selected);
					
				}
				else if (!pt2.isSelected)
				{
					if (Selected.row == pt1.row && Selected.col == pt1.col)
					{
						/*GoTo(WORD_WIDTH_SPACING, calculatePositionHeight(difficulty, difficulty) + 1);
						wprintf(L"You can't select the same node twice\n");*/
						gotoxy(71, 3);
						textColor(BLACK, YELLOW);
						cout << "You can't choose the same cube twice!";
						textColor(BLACK, WHITE);
						continue;
					}
					/*GoTo(WORD_WIDTH_SPACING, calculatePositionHeight(difficulty, difficulty) + 1);
					wprintf(L"Second Node Selected\n");
					secondNode.isSelected = true;
					secondNode.posX = posX;
					secondNode.posY = posY;*/
					pt2 = Selected;
					pt2.isSelected = true;

					deSelectedCube(a, BLACK, GREEN, pt2);
					reSelectedCube(a, AQUA, YELLOW, pt2);

					check2Points(a, pt1, pt2);

					//Selected.row++;
					Selected = pt1;
					Selected.isSelected = false;

					//selectedCube(a, WHITE, RED, Selected);
				}
				else
				{
					/*GoTo(WORD_WIDTH_SPACING, calculatePositionHeight(difficulty, difficulty) + 3);
					wprintf(L"You can't select more than 2 nodes\n");*/
				}
				break;
			default:
				break;
			}

			// Restore the previous Node to original state
			/*DrawCube(matrix, difficulty, tempNode, BLACK, WHITE);

			if (firstNode.isSelected)
			{
				DrawCube(matrix, difficulty, firstNode, AQUA, BLACK);
			}

			if (secondNode.isSelected)
			{
				DrawCube(matrix, difficulty, secondNode, AQUA, BLACK);
				firstNode.prepareSelected(secondNode);

				// Check if 2 nodes are selected is the same character
				if (checkNodeIdentical(matrix, difficulty, firstNode, secondNode, backGround))
				{
					GoTo(WORD_WIDTH_SPACING, calculatePositionHeight(difficulty, difficulty) + 2);
					wprintf(L"2 nodes are identical\n");
					// Draw background at the posX and posY coordinates

					// Check local elements of the matrix if it different from ' '
					Selected newCor = checkLocalElements(matrix, difficulty, secondNode);

					posX = newCor.posX;
					posY = newCor.posY;
				}
				else
				{
					GoTo(WORD_WIDTH_SPACING, calculatePositionHeight(difficulty, difficulty) + 2);
					wprintf(L"2 nodes are different\n");
				}

				if (isSolved(matrix, difficulty))
				{
					GoTo(WORD_WIDTH_SPACING, calculatePositionHeight(difficulty, difficulty) + 3);
					wprintf(L"You win\n");
				}
			}

			if (moveToPosition(matrix, difficulty, posX, posY))
			{
				break;
			}

			tempNode.posX = posX;
			tempNode.posY = posY;

			check2Points(a, pt1, pt2);*/
		}
		for (int i = 0; i < MAXROW; i++)
			delete [] a[i];
		delete [] a;
		congratulating();
		gotoxy(0, 0);
		gotoxy(50, 8);
		system("pause");
	}
	else
		system("pause");
	
	return 0;
}