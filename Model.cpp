#include <string>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
using namespace std;

#define BOARD_SIZE 12 //Kích thước ma trận bàn cờ
#define LEFT 3 //Tọa độ trái màn hình bàn cờ
#define TOP 1 //Tọa độ trên màn hình bàn cờ
#define OPTION_HIGH 2
#define OPTION_WIDTH  20


//Khai báo kiểu dữ liệu
extern struct _POINT { int x, y, c; }; //x: tọa độ dòng, y: tọa độ cột, c: đánh dấu
extern _POINT _A[BOARD_SIZE][BOARD_SIZE]; //Ma trận bàn cờ
extern bool _TURN; //true là lượt người thứ nhất và false là lượt người thứ hai
extern int _COMMAND; //Biến nhận giá trị phím người dùng nhập
extern int _X, _Y; //Tọa độ hiện hành trên màn hình bàn cờ
extern struct MENU
{
	string opt1;
	string opt2;
	string opt3;
	string opt4;
};
extern MENU menu;
extern int _OPTION;
extern int Score1;
extern int Score2;
extern string Player1_name;
extern string Player2_name;
extern int win_location[11];

//nhom ham view
void GotoXY(int x, int y);
void DrawOption(int x, int y, int w, int h, int b_color, int t_color, string s);
void HighLight(int x, int y, int w, int h, int color);
void SetColor(int backgound_color, int text_color);
void Draw_newgame_opt(int x, int y, int w, int h);
void DrawMenu(int x, int y, int w, int h, MENU m);
void DrawAbout();
void printLogo();
void Highlight_win();
//nhom ham control
void MenuUp(int& o);
void MenuDown(int& o, int n);
void StartGame();
void PlayPvP();
void PlayPvC();


/*Hàm khởi tạo dữ liệu mặc định ban đầu cho ma trận bàn cờ*/
void ResetGame()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			_A[i][j].x = 4 * j + LEFT + 2; //Trùng với hoành độ màn hình bàn cờ
			_A[i][j].y = 2 * i + TOP + 1; //Trùng với tung độ màn hình bàn cờ
			_A[i][j].c = 0; //0: chưa đánh dấu, -1: true đánh, 1: fasle đánh
		}
	}
	_TURN = true; _COMMAND = -1; //Gán lượt và phím mặt định
	_X = _A[0][0].x; _Y = _A[0][0].y; //Thiết lập lại tọa đồ hiện hành ban đầu
}

/*Hàm khởi tạo lại dữ liệu cho một game mới khi new game*/
void ResetData()
{
	ResetGame();
	Score1 = 0;
	Score2 = 0;
	Player1_name = "";
	Player2_name = "";
}

//Kiểm tra đã đủ 80% quân cờ trên bàn cờ chưa
bool isfull()
{
	int dem = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j].c != 0)
				dem++;
		}
	}
	float phan_tram = (1.0 * dem) / (BOARD_SIZE * BOARD_SIZE);
	if (phan_tram >= 0.8)
		return true;
	return false;
}

//Các hàm kiểm tra có win không
bool xet_dong() {
	int demX = 0, demO = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		demX = 0;
		demO = 0;
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j].c == 1) //O
				demO++;
			else
			{
				if (_A[i][j].c == -1) //X
				{
					if (demO >= 5 && _A[i][j - (demO + 1)].c != -1)
						demO = 5;
					else
						demO = 0;
				}
				if (_A[i][j].c == 0) // Chưa đánh dấu
				{
					if (demO >= 5)
						demO = 5;
					else
						demO = 0;
				}
			}

			if (_A[i][j].c == -1) //X
				demX++;
			else
			{
				if (_A[i][j].c == 1) //O
				{
					if (demX >= 5 && _A[i][j - (demX + 1)].c != 1)
						demX = 5;
					else
						demX = 0;
				}
				if (_A[i][j].c == 0)
				{
					if (demX >= 5)
						demX = 5;
					else
						demX = 0;
				}
			}

			if (demO >= 5 || demX >= 5)
			{
				if (demO >= 5)
					win_location[0] = 1;
				else win_location[0] = -1;

				//Hilight giùm trong hàm này nha nhã
				for (int cot = j,temp=1; cot >= j - 4; cot--,temp+=2)
				{
					/*cout << cot << " ";*/
					win_location[temp] = i;//i la dong
					win_location[temp + 1] = cot;
				}

				return true;
			}
		}
	}
}

bool xet_cot()
{
	int demO = 0, demX = 0;
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		demO = 0;
		demX = 0;
		for (int i = 0; i < BOARD_SIZE; i++)

		{
			if (_A[i][j].c == 1) //O
				demO++;
			else
			{
				if (_A[i][j].c == -1) //X
				{
					if (demO >= 5 && _A[i - (demO + 1)][j].c != -1) //X
						demO = 5;
					else
						demO = 0;
				}
				if (_A[i][j].c == 0)
					if (demO >= 5)
						demO = 5;
					else
						demO = 0;
			}

			if (_A[i][j].c == -1) //X
				demX++;
			else
			{
				if (_A[i][j].c == 1) //O
				{
					if (demX >= 5 && _A[i - demX + 1][j].c != 1) //O
						demX = 5;
					else
						demX = 0;

				}
				if (_A[i][j].c == 0)
					if (demX >= 5)
						demX = 5;
					else
						demX = 0;
			}

			if (demO >= 5 || demX >= 5)
			{
				if (demO >= 5)
					win_location[0] = 1;
				else win_location[0] = -1;

				for (int dong = i,temp=1; dong >= i - 4; dong--,temp+=2)
				{
					win_location[temp] = dong;
					win_location[temp+1] = j;
				}
				return true;
			}
		}
	}
}

bool xet_cheo_duoi_phu()
{
	int demO = 0, demX = 0;
	int j = 0, k = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		demO = 0;
		demX = 0;

		k = i;
		j = BOARD_SIZE - 1;
		while (k < BOARD_SIZE && j >= 0)
		{

			if (_A[k][j].c == 1) //O
				demO++;
			else
			{
				if (_A[k][j].c == -1) //X
				{
					if (demO >= 5 && (k - demO >= 0 && j + demO < BOARD_SIZE && _A[k - demO][j + demO].c != -1))
						demO = 5;
					else
						demO = 0;
				}
				if (_A[k][j].c == 0)
					if (demO >= 5)
						demO = 5;
					else
						demO = 0;
			}
			if (_A[k][j].c == -1) //X
				demX++;
			else
			{
				if (_A[k][j].c == 1) //O
				{
					if (demX >= 5 && (k - demX >= 0 && j + demX < BOARD_SIZE && _A[k - demX][j + demX].c == 1))
						demX = 5;
					else
						demX = 0;
				}
				if (_A[k][j].c == 0)
					if (demX >= 5)
						demX = 5;
					else
						demX = 0;
			}

			if (demO >= 5 || demX >= 5)
			{
				if (demO >= 5)
					win_location[0] = 1;
				else win_location[0] = -1;

				for (int dong = k, cot = j,temp=1; dong >= k - 4; dong--, cot++,temp+=2)
				{
					win_location[temp] = dong;
					win_location[temp + 1] = cot;
				}
				return true;
			}

			k++;
			j--;
		}
	}
	return false;
}

bool xet_cheo_tren_phu()
{
	int demO = 0, demX = 0;
	int i = 0, k = 0;
	for (int j = 1; j < BOARD_SIZE; j++)
	{
		demO = 0;
		demX = 0;

		k = j;
		i = 0;
		while (k > 0)
		{
			if (_A[i][k].c == 1) //O
				demO++;
			else
			{
				if (_A[i][k].c == -1) //X
				{
					if (demO >= 5 && _A[i - demO + 1][k + demO + 1].c != -1) //X
						demO = 5;
					else
						demO = 0;
				}
				if (_A[i][k].c == 0)
					if (demO >= 5)
						demO = 5;
					else
						demO = 0;
			}
			if (_A[i][k].c == -1) //X
				demX++;
			else
			{
				if (_A[i][k].c == 1) //O
				{
					if (demX >= 5 && _A[i - demX + 1][k + demX + 1].c != 1)
						demX = 5;
					else
						demX = 0;
				}
				if (_A[i][k].c == 0)
					if (demX >= 5)
						demX = 5;
					else
						demX = 0;
			}

			if (demO >= 5 || demX >= 5)
			{
				if (demO >= 5)
					win_location[0] = 1;
				else win_location[0] = -1;

				for (int dong = i, cot = k,temp=1; dong >= i - 4; dong--, cot++,temp+=2)
				{
					win_location[temp] = dong;
					win_location[temp + 1] = cot;
				}
				return true;
			}

			k--;
			i++;
		}
	}
}

bool xet_chinh()
{
	int demO = 0, demX = 0;
	for (int i = 0; i <= BOARD_SIZE - 5; i++) {
		for (int j = 0; j <= BOARD_SIZE - 5; j++) {
			demO = 0;
			demX = 0;
			for (int k = 0; k < 5; k++) {
				if (_A[i + k][j + k].c == 1) { //O
					demO++;
				}
				else {
					if (_A[i + k][j + k].c == -1) { //X
						if (demO >= 5) {
							demO = 5;
						}
						else {
							demO = 0;
						}
					}
					else { // Chưa đánh dấu
						if (demO >= 5) {
							demO = 5;
						}
						else {
							demO = 0;
						}
					}
				}

				if (_A[i + k][j + k].c == -1) { //X
					demX++;
				}
				else {
					if (_A[i + k][j + k].c == 1) { //O
						if (demX >= 5) {
							demX = 5;
						}
						else {
							demX = 0;
						}
					}
					else { // Chưa đánh dấu
						if (demX >= 5) {
							demX = 5;
						}
						else {
							demX = 0;
						}
					}
				}
			}
			if (demO >= 5 || demX >= 5)
			{
				if (demO >= 5)
					win_location[0] = 1;
				else win_location[0] = -1;

				for (int dong = i, cot = j,temp=1; dong <= i + 4; dong++, cot++,temp+=2)
				{
					win_location[temp] = dong;
					win_location[temp + 1] = cot;
				}
				return true;
			}
		}
	}
	return false;
}

bool xet_cheo_phu_ben_trai_cung()
{
	int demO = 0, demX = 0;
	int i = 0, k = 0;
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		demO = 0;
		demX = 0;

		k = j;
		i = 0;
		while (k >= 0 && i < BOARD_SIZE)
		{
			if (_A[i][k].c == 1) //O
				demO++;
			else
			{
				if (_A[i][k].c == -1) //X
				{
					if (demO >= 5 && (i - demO < 0 || k + demO >= BOARD_SIZE || _A[i - demO][k + demO].c != -1)) //X
						demO = 5;
					else
						demO = 0;
				}
				if (_A[i][k].c == 0)
					if (demO >= 5)
						demO = 5;
					else
						demO = 0;
			}
			if (_A[i][k].c == -1) //X
				demX++;
			else
			{
				if (_A[i][k].c == 1) //O
				{
					if (demX >= 5 && (i - demX < 0 || k + demX >= BOARD_SIZE || _A[i - demX][k + demX].c != 1))
						demX = 5;
					else
						demX = 0;
				}
				if (_A[i][k].c == 0)
					if (demX >= 5)
						demX = 5;
					else
						demX = 0;
			}
			if (demO >= 5 || demX >= 5)
			{
				if (demO >= 5)
					win_location[0] = 1;
				else win_location[0] = -1;

				for (int dong = i, cot = k,temp=1; cot <= k + 4; dong--, cot++,temp+=2)
				{
					win_location[temp] = dong;
					win_location[temp+1] = cot;
				}
				return true;
			}
			k--;
			i++;
		}
	}
	return false;
}

//Hàm kiểm tra thắng
bool WinTest()
{
	if (xet_dong() == true) //roi
		return true;
	if (xet_cot() == true) //roi
		return true;
	if (xet_cheo_duoi_phu() == true) //roi
		return true;
	if (xet_cheo_tren_phu() == true)
		return true;
	if (xet_chinh() == true) //roi
		return true;
	if (xet_cheo_phu_ben_trai_cung() == true) //roi
		return true;
	return false;
}

/*Hàm kiểm ta xem có người thắng/thua/hòa*/
int TestBoard()
{
	if (isfull() == true) return 0; //hòa
	else
	{
		if (WinTest() == true)
			return (_TURN == true ? -1 : 1);
		else
			return 2;
	}
}

/*Hàm hổ trợ xem đang lượt của ai khi enter*/
int CheckBoard(int pX, int pY)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0)
			{
				if (_TURN == true) //Lượt của người 1
					_A[i][j].c = -1; //c= -1 tức là người 1 đánh dấu
				else _A[i][j].c = 1; //c=1 tức là người chơi 2 đánh dấu
				return _A[i][j].c;
			}
		}
	}
	return 0;
}

/*Hàm kiểm tra ô đã đánh chưa*/
int CheckTick(int pX, int pY)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j].x == pX && _A[i][j].y == pY)
				return _A[i][j].c;
		}
	}
}

//Hàm tìm địa chỉ hàng cột của ô với địa chỉ cho trước
int searchAddressOfBoard(int pX, int pY, int& row, int& col) {
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j].x == pX && _A[i][j].y == pY)
			{
				row = i;
				col = j;
				return 1;
			}
		}
	}
	return 0;
}

//Hàm tạo bot chơi với người chơi, lượt của bot là người thứ 2 hay _TURN = false
int Bot(int _X, int _Y, int& pX, int& pY)
{
	srand(time(NULL));

	int row, col;
	searchAddressOfBoard(_X, _Y, row, col);

	//Trường hợp đánh nhiều ô liên tiếp

	//Hàng
	if (_A[row][col - 2].c == -1 && _A[row][col - 1].c == -1 && _A[row][col].c == -1)
	{
		if (col + 1 < BOARD_SIZE)
		{
			if (_A[row][col + 1].c == 0)
			{
				pX = row;
				pY = col + 1;
				_A[row][col + 1].c = 1;
				return 1;
			}
		}
	}
	if (_A[row][col].c == -1 && _A[row][col + 1].c == -1 && _A[row][col + 2].c == -1 && _A[row][col + 3].c == -1)
	{
		if (col - 1 >= 0)
		{
			if (_A[row][col - 1].c == 0)
			{
				pX = row;
				pY = col - 1;
				_A[row][col - 1].c = 1;
				return 1;
			}
		}
	}

	//Cột
	if (_A[row][col].c == -1 && _A[row - 1][col].c == -1 && _A[row - 2][col].c == -1)
	{
		if (row + 1 < BOARD_SIZE)
		{
			if (_A[row + 1][col].c == 0)
			{
				pX = row + 1;
				pY = col;
				_A[row + 1][col].c = 1;
				return 1;
			}
		}
	}
	if (_A[row][col].c == -1 && _A[row + 1][col].c == -1 && _A[row + 2][col].c == -1 && _A[row + 3][col].c == -1)
	{
		if (row - 1 >= 0)
		{
			if (_A[row - 1][col].c == 0)
			{
				pX = row - 1;
				pY = col;
				_A[row - 1][col].c = 1;
				return 1;
			}
		}
	}

	//Chéo

	//chéo trên phụ
	if (_A[row][col].c == -1 && _A[row + 1][col - 1].c == -1 && _A[row + 2][col - 2].c == -1)
	{
		if (row - 1 >= 0 && col + 1 < BOARD_SIZE)
		{
			if (_A[row - 1][col + 1].c == 0)
			{
				pX = row - 1;
				pY = col + 1;
				_A[row - 1][col + 1].c = 1;
				return 1;
			}
		}
	}
	if (_A[row][col].c == -1 && _A[row - 1][col + 1].c == -1 && _A[row - 2][col + 2].c == -1 && _A[row - 3][col + 3].c == -1)
	{
		if (row + 1 < BOARD_SIZE && col - 1 >= 0)
		{
			if (_A[row + 1][col - 1].c == 0)
			{
				pX = row + 1;
				pY = col - 1;
				_A[row + 1][col - 1].c = 1;
				return 1;
			}
		}
	}
	//Chéo trên chính
	if (_A[row][col].c == -1 && _A[row + 1][col + 1].c == -1 && _A[row + 2][col + 2].c == -1)
	{
		if (row - 1 >= 0 && col - 1 >= 0)
		{
			if (_A[row - 1][col - 1].c == 0)
			{
				pX = row - 1;
				pY = col - 1;
				_A[row - 1][col - 1].c = 1;
				return 1;
			}
		}
	}
	if (_A[row][col].c == -1 && _A[row - 1][col - 1].c == -1 && _A[row - 2][col - 2].c == -1 && _A[row - 3][col - 3].c == -1)
	{
		if (row + 1 < BOARD_SIZE && col + 1 < BOARD_SIZE)
		{
			if (_A[row + 1][col + 1].c == 0)
			{
				pX = row + 1;
				pY = col + 1;
				_A[row + 1][col + 1].c = 1;
				return 1;
			}
		}
	}

	//Trường hợp đánh rời rạc
	int bestVal = 0;
	while (bestVal = rand() % 8 + 1)
	{
		switch (bestVal)
		{
		case 1:
		{
			for (int i = col; i < BOARD_SIZE; i++)
			{
				if (_A[row][i].c == 0)
				{
					pX = row;
					pY = i;
					_A[row][i].c = 1;
					return 1;
				}
			}
			break;
		}
		case 2:
		{
			for (int i = col; i >= 0; i--)
			{
				if (_A[row][i].c == 0)
				{
					pX = row;
					pY = i;
					_A[row][i].c = 1;
					return 1;
				}
			}
			break;
		}
		case 3:
		{
			for (int i = row; i < BOARD_SIZE; i++)
			{
				if (_A[i][col].c == 0)
				{
					pX = i;
					pY = col;
					_A[i][col].c = 1;
					return 1;
				}
			}
			break;
		}
		case 4:
		{
			for (int i = row; i >= 0; i--)
			{
				if (_A[i][col].c == 0)
				{
					pX = i;
					pY = col;
					_A[i][col].c = 1;
					return 1;
				}
			}
			break;
		}
		case 5:
		{
			for (int i = row, j = col; i >= 0 && j < BOARD_SIZE; i--, j++)
			{
				if (row <= 1)
					break;
				if (_A[i][j].c == 0)
				{
					pX = i;
					pY = j;
					_A[i][j].c = 1;
					return 1;
				}
			}
			break;
		}
		case 6:
		{
			for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
			{
				if (row <= 1)
					break;
				if (_A[i][j].c == 0)
				{
					pX = i;
					pY = j;
					_A[i][j].c = 1;
					return 1;
				}
			}
			break;
		}
		case 7:
		{
			for (int i = row, j = col; i < BOARD_SIZE && j < BOARD_SIZE; i++, j++)
			{
				if (_A[i][j].c == 0)
				{
					pX = i;
					pY = j;
					_A[i][j].c = 1;
					return 1;
				}
			}
			break;
		}
		case 8:
		{
			for (int i = row, j = col; i < BOARD_SIZE && j >= 0; i++, j--)
			{
				if (_A[i][j].c == 0)
				{
					pX = i;
					pY = j;
					_A[i][j].c = 1;
					return 1;
				}
			}
			break;
		}
		}
	}

	return 0;
}

void InputPvP(int x, int y)
{
	SetColor(15, 0);
	system("cls");
	GotoXY(x, y);
	cout << "Nhap ten Player 1: ";
	char c;
	while (1)
	{
		c = toupper(_getch());
		if (c == 13) break;
		cout << c;
		Player1_name += c;
	}
	GotoXY(x, y + 2);
	cout << "Nhap ten Player 2: ";
	while (1)
	{
		c = toupper(_getch());
		if (c == 13) break;
		cout << c;
		Player2_name += c;
	}
}

void InputPvC(int x, int y)
{
	Player2_name = "COMPUTER";
	SetColor(15, 0);
	system("cls");
	GotoXY(x, y);
	cout << "Nhap ten Player 1: ";
	char c;
	while (1)
	{
		c = toupper(_getch());
		if (c == 13) break;
		cout << c;
		Player1_name += c;
	}
}

void Newgame_opt()
{
	int x = 50;
	int y = 16;
	MENU m = { "PvP","PVC" };
	int opt = 1;
	Draw_newgame_opt(x, y, OPTION_WIDTH, OPTION_HIGH);
	while (opt != 0)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 13)
		{
			switch (opt)
			{
			case 1:
				InputPvP(x - 10, y);
				StartGame();
				PlayPvP();
				return;
			case 2:
				InputPvC(x - 10, y);
				StartGame();
				PlayPvC();
				return;
			}
		}
		else if (_COMMAND == 'B')
		{
			return;
		}
		else if (_COMMAND == 'W')
		{
			if (opt == 1)
				continue;
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15);
			DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, m.opt2);
			MenuUp(opt);
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14);
			DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, m.opt1);
			GotoXY(_X, _Y);
		}
		else if (_COMMAND == 'S')
		{
			if (opt == 2)
				continue;
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15);
			DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, m.opt1);
			MenuDown(opt, 2);
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14);
			DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, m.opt2);
			GotoXY(_X, _Y);
		}
	}
}

void Play()
{
	system("color F0");
	int x = 50, y = 16;
	DrawMenu(x, y, OPTION_WIDTH, OPTION_HIGH, menu);
	_OPTION = 1;
	while (_OPTION != 0)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 13)
		{
			switch (_OPTION)
			{
			case 1:
				Newgame_opt();
				return;
			case 3:
				DrawAbout();
				return;
			case 4:
				system("cls");
				system("color F0");
				exit(0);
			}
		}
		else if (_COMMAND == 'W')
		{
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15);
			switch (_OPTION)
			{
			case 2:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, menu.opt2);
				break;
			case 3:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, menu.opt3);
				break;
			case 4:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, menu.opt4);
				break;
			}
			MenuUp(_OPTION);
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14);
			switch (_OPTION)
			{
			case 1:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, menu.opt1);
				break;
			case 2:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, menu.opt2);
				break;
			case 3:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, menu.opt3);
				break;
			}
			GotoXY(_X, _Y);
		}
		else if (_COMMAND == 'S')
		{
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15);
			switch (_OPTION)
			{
			case 1:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, menu.opt1);
				break;
			case 2:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, menu.opt2);
				break;
			case 3:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 15, 0, menu.opt3);
				break;
			}
			MenuDown(_OPTION, 4);
			HighLight(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14);
			switch (_OPTION)
			{
			case 2:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, menu.opt2);
				break;
			case 3:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, menu.opt3);
				break;
			case 4:
				DrawOption(_X, _Y, OPTION_WIDTH, OPTION_HIGH, 14, 0, menu.opt4);
				break;
			}
			GotoXY(_X, _Y);
		}
	}
}