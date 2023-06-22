//https://ww.github.com/Titytus

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <windows.h>

#define SIZE 	20
#define b_size 	3
#define speed 	250
#define b_LEFT 	6
#define b_RIGHT 45
#define b_TOP 	3
#define b_BOT 	22

struct block
{
	int *block;
	int col, row, color;
};
void gotoXY(int x, int y);
void textColor (int color);
void resizeConsole(int width, int height);

void draw(unsigned long int score);		//Ve khong gian nguoi dung
void init(block *&A);				//Khoi tao cac gia tri ban dau
void clear(block* A, int k, int x, int y);
void printBlock(block* A, int k, int x, int y);	//In block ra man hinh
void xoay(block* A, int k);			//Xoay cac block
void update(block* A, int k, int x, int y);
void control(block* &A, int k, int &x, int &y);	//Dieu khien cac block tren man hinh
void ghiDiem(unsigned long int &score);
void move(int row);
int next(block* A, int k, int x, int y);
int tieptuc();					//Kiem tra dieu kien Stop cua game
void game(block* A);
void endGame(unsigned long int score);		//In ket qua cuoi cung
int board[SIZE+1][SIZE]={0};			//luu gia tri color tren moi o vuong

int main()
{	
	srand(time(NULL));
	resizeConsole(650,450);
	block *A=new block[7];
	game(A);
	delete[] A;
	return 0;
}
void game(block* A)
{
	int k, x, y;
	unsigned long int score=0;
	init(A);
	draw(score);
	while (tieptuc())
	{
		k=rand()%7+0;
		x=(b_LEFT+b_RIGHT+1)/2-2, y=b_TOP;
		control(A,k,x,y);
		update(A,k,x,y);
		ghiDiem(score);
	}
	endGame(score);
}
void init(block* &A)
{
	int i;
	for (i=0;i<SIZE;i++)				//Khoi tao diem stop
		board[SIZE][i]=-1;
	for (i=0;i<7;i++)				//Khoi tao row*col
	{
		A[i].row=2;
		A[i].col=3;
	}
	for (i=0;i<7;i++)				//Khoi tao color
		A[i].color=255-16*i;
	//Khoi tao Block
	int tmp[7][2][3] =
	{
		{ {1,1,1}	   },
		{ {1,1,0}, {1,1,0} },
		{ {0,0,1}, {1,1,1} },	//L
		{ {1,0,0}, {1,1,1} },	//L nguoc
		{ {0,1,1}, {1,1,0} },	//S
		{ {1,1,1}, {0,1,0} },	//T
		{ {1,1,0}, {0,1,1} }	//Z
	};
	for (i=0;i<7;i++)
	{
		A[i].block=new int[A[i].row*A[i].col];
		memcpy(A[i].block,tmp[i],sizeof(tmp[i]));
	}
	A[1].block[2]=1;
	A[0].row=1;	A[0].col=3;
	A[1].row=2;	A[1].col=2;
}

void xoay(block* A, int k)
{
	int pos=0, col=A[k].col, row=A[k].row, B[col][row]={0};
	for (int i=row-1;i>=0;i--)
		for (int j=0;j<col;j++)
			B[j][i]=A[k].block[pos++];
	memcpy(A[k].block,B,sizeof(B));
	A[k].row=col; A[k].col=row;		//Hoan doi kich thuoc col-row
}
void control(block* &A, int k, int &x, int &y)
{
	char key_press;
	while (next(A,k,x,y+1))
	{
		int delay=0;			//Delay thoi gian drop
		clear(A,k,x,y);			//Xoa Block o vi tri hien tai
		if (kbhit())			//Khi co 1 phim duoc nhan
		{
			key_press=getch();	//Doc ki tu tu keyboard
			switch ((int)key_press)
			{
				case 75:	//LEFT
					if (next(A,k,x-2,y) && x-2>=b_LEFT)
						x-=2; 
					break;
				case 77:	//RIGHT
					if (next(A,k,x+2,y) && x+2*A[k].col<=b_RIGHT)
						x+=2; 
					break;	
				case 80:	//DOWN
					while (next(A,k,x,y+1)) 
						y++;
					break;
				case 72: 	//ENTER
					{
						xoay(A,k);
						delay++;
					}
					break;
			};
		} 
		else
		{
			delay++;
			y++;
		}
		printBlock(A,k,x,y);
		if (delay)
			Sleep(speed);
	}	
}
int next(block* A, int k, int x, int y)
{	
	for (int i=0;i<A[k].row;i++)
		for (int j=0;j<A[k].col;j++)
			//Neu phia duoi co vat can thi stop
			if (A[k].block[i*A[k].col+j] * board[y-b_TOP+i][(x-b_LEFT)/2+j]!=0)
				return 0;
	return 1;
}
void endGame(unsigned long int score)
{
	textColor(0);	system("cls");
	int i=(b_RIGHT+b_LEFT)/2,	j=(b_TOP+b_BOT)/2;
	gotoXY(i,j);	textColor(15);	printf("S C O R E");
	gotoXY(i,j+2);	textColor(15);	printf(">> %ld <<",score);
}
void ghiDiem(unsigned long int &score)
{
	int count=0, sodong=0;			//So diem (point)
	for (int i=0;i<SIZE;i++)
	{
		int c=0;			//Dem so o vuong khong con trong (not blank)
		for (int j=0;j<SIZE;j++)
			if (board[i][j]>0) c++;
		if (c==SIZE)			//Xep full hang
		{
			score+=10*(++sodong);	//Xoa cang nhieu dong, diem (point) cang cao
			move(i);		//Dich chuyen xuong 1 hang (row)
			draw(score);		//Ve lai man hinh console
		}
	}		
}
void move(int row)
{
	for (int i=row;i>0;i--)			//Dich chuyen board xuong 1 hang (row)
		for (int j=0;j<SIZE;j++)
			board[i][j]=board[i-1][j];
	for (int k=0;k<SIZE;k++)		//Khoi tao lai board[0][]
		board[0][k]=0;
}
//kiem tra xem co the tiep tuc drop hay ko
int tieptuc()
{
	int x=(b_LEFT+b_RIGHT+1)/2-2;
	for (int i=0;i<b_size;i++)
		for (int j=0;j<b_size;j++)
			if (board[i][(x-b_LEFT)/2+j]!=0)
				return 0;
	return 1;
}
void draw(unsigned long int score)
{
	int i,j;textColor(15);			//Dat lai color => ve border
	for (i=0;i<b_RIGHT-b_LEFT+5;i++)	//Ve dt//Ox
	{
		gotoXY(b_LEFT-2+i,b_TOP-1);	printf(":");
		gotoXY(b_LEFT-2+i,b_BOT+1);	printf(":");
	}
	for (i=0;i<b_BOT-b_TOP+3;i++)		//Ve dt//Oy
	{	
		gotoXY(b_LEFT-2,b_TOP-1+i);	printf("::");
		gotoXY(b_RIGHT+1,b_TOP-1+i);	printf("::");
	}
	for (i=0;i<SIZE;i++)			//Ve lai vi tri cac block tren man hinh console
		for (int j=0;j<SIZE;j++)
		{
			gotoXY(b_LEFT+2*j,b_TOP+i);//Dich chuyen tren man hinh
			textColor(board[i][j]);	printf("  ");
		}
	//Ghi SCORE
	i=b_RIGHT+2*b_LEFT;	j=(b_TOP+b_BOT)/2;
	gotoXY(i,j);	textColor(15);	printf("S C O R E");
	gotoXY(i,j+2);	textColor(15);	printf(">> %ld <<",score);
}
void printBlock(block* A, int k, int x, int y)
{
	for (int i=0;i<A[k].row;i++)
		for (int j=0;j<A[k].col;j++)
			//Neu o ko trong
			if (A[k].block[i*A[k].col+j]>0)
			{
				textColor(A[k].color);	
				gotoXY(x+2*j,y+i);
				printf("  ");
			}
	gotoXY(60,14);
}
void update(block* A, int k, int x, int y)
{
	for (int i=0;i<A[k].row;i++)
		for (int j=0;j<A[k].col;j++)
			if (A[k].block[A[k].col*i+j])
				board[(y-b_TOP)+i][(x-b_LEFT)/2+j]=A[k].color;		//Ghi lai vi tri chua block theo color
}
void clear(block* A, int k, int x, int y)
{
	textColor(0);
	int row=A[k].row, col=A[k].col;
	for (int i=0;i<row;i++)
		for (int j=0;j<col;j++)
		{
			gotoXY(x+2*j,y+i);
			if (A[k].block[i*col+j]!=0) printf("  ");
		}
}
void textColor (int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}
void gotoXY(int x, int y)
{
	static HANDLE h=NULL;
	if (!h)	h=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c={x,y};
	SetConsoleCursorPosition(h,c);
}
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
