#include <iostream>
#include <time.h>
#include <graphics.h>
#include <algorithm>
#include <string>
#include <fstream>
#include <windows.h>
#include <Mmsystem.h>
#include <cstdlib>
#include <stack>
#pragma comment(lib,"winmm.lib")
using namespace std;

typedef struct Player {
	string Name;
	double Rank;
	double Time;
};
/*��ͼ��������*/
#define Width 18
#define Height 12
/*��ϷͼƬ�����ֵ*/
#define Blocks 160
#define PicWidth 72
#define PicHeight 60
#define WinWidth 1280 
#define WinHeight 720
#define SmallButton 50
#define BigButton 180
#define ButtonWidth 500
#define ButtonHeight 100
/*��Ϸ���������ֵ*/
#define BGX 0
#define BGY 0
#define LogoX 160
#define LogoY 50
/*��Ϸ���������ֵ*/
#define SaveButtonInterval 30
#define SaveButtonWidth 215
/*ʱ�����*/
time_t GameBegin, GameEnd;
double GameTime = 0;
double GameTimeTotal = 0;
int TimeTemp = 0;
int CutDownTime = 180;

IMAGE img[70];
/*����ͼƬ*/
IMAGE BGGame; IMAGE BGWelcome; IMAGE BGSave; IMAGE BGGameOver;
IMAGE BGGameWin; IMAGE BGGamePass; IMAGE BGRank; IMAGE LLKLogo;
IMAGE BGGuide;
/*��ťͼƬ*/
IMAGE Exit; IMAGE Pause; IMAGE BackWelcome; IMAGE BeginGame;
IMAGE ContinueGame; IMAGE ExitGame; IMAGE LoadData; IMAGE LoadRank;
IMAGE RePlay; IMAGE WriteRankB; IMAGE Click; IMAGE NotSaved; IMAGE Saved; 
IMAGE Back; IMAGE Guide; IMAGE CountDown; IMAGE Time;
/*�����*/
int    MouseClickIndex = 0;
COORD  TargetPos[2] = { -1, -1 };
/*��Ϸ��ؿ��Ʊ���*/
int Map[Height][Width] = { 0 };	//��ͼ����ṹ
int Players = 0;				//���а��¼�����Ŀ
int GameScore = Blocks;			//������Ϸ����
int GameRank = 0;				//��Ϸ�ܵ÷�
int PicCount = 10;				//ͼƬʹ�ô���������ԽС�Ѷ�Խ��
int SaveDataCount = 0;			//�浵������
bool WriteOrRead = false;		//���ֱ������
int FlushCount = 0;				//�Ʒֹ�ʽ���ֶ�ˢ�´���
int SuggestCount = 0;			//�Ʒֹ�ʽ���ֶ���ʾ����
/*��Ϸ���ܺ���*/
void InitGame();				//��Ϸ��ʼ��
void LoadImage();				//ͼƬ����
void ShowPic();					//��ӡͼƬ
void UpdateGame();				//��Ϸ����
void WriteRank();				//д�����а�
void SortRank();				//���а�����
bool cmp(Player arr, Player arrs);
void FlushMap();				//ˢ�µ�ͼ
void SuggestMap();				//��Ϸ��ʾ
void DrawPng(int  X, int Y, IMAGE* picture);		//����͸����PNG�ز�
bool NoCornerPath(COORD pos1, COORD pos2);			//���ε����X��Y����һ��
bool HaveOneCornerPath(COORD pos1, COORD pos2);		//·����һ��ת��
bool HaveTwoCornerPath(COORD pos1, COORD pos2);		//·����������ת��
bool HavePath(COORD targetPos[]);					//ͨ·�ж�
/*���滯�������غ���*/
void SaveData(int num);						//������Ϸ����
void SaveDataUI();							//������Ϸ���ݽ���
void PutLoadDataPic();						//��ԭ��ͼ����
bool LoadSaveData(int num);					//���ر�������
void LoadSaveUI();							//���ر������ݽ���
void UpdateSaveUI();						//���½���ͼƬ
void DeleteData(int num);					//ɾ���浵
/*��Ϸ������غ���*/
void WelcomeGameUI();						//��ҳ��
void ShowRank();							//���а�
void GamePass();							//ͨ��С��
void GameOver();							//��Ϸʧ��
void GameWin();								//��Ϸʤ��
void GameGuide();							//��Ϸָ��

int main()
{
	initgraph(WinWidth, WinHeight);
	srand((unsigned int)time(NULL));
	while (true)
	{
		WelcomeGameUI();
		break;
	}
	while (true)
	{
		GameBegin = clock();
		UpdateGame();
		GameEnd = clock();
		GameTime = double(GameEnd - GameBegin) / CLOCKS_PER_SEC; 
		GameTimeTotal += GameTime;
		loadimage(&Time, "./Resources/Pictures/Button/Time.png", 645 - (35 + GameTimeTotal * 3), 35);
		ShowPic();
		if (GameScore == 0)
			GamePass();
	}
}

void InitGame()
{
	int PicNum;
	PicNum = (rand() % 50) + 1;
	int TempPicCount = 0;
	int TempMap[Height - 2][Width - 2] = { 0 };
	for (int i = 0; i < Height - 2; ++i)
	{
		for (int j = 0; j < Width - 2; ++j)
		{
			TempMap[i][j] = PicNum;
			TempPicCount++;
			if (TempPicCount == PicCount)
			{
				PicNum++;
				TempPicCount = 0;
			}
		}
	}
	random_shuffle((int*)TempMap, (int*)TempMap + (Height - 2) * (Width - 2));
	for (size_t i = 1; i < Width - 1; i++)
	{
		for (size_t j = 1; j < Height - 1; j++)
		{
			Map[j][i] = TempMap[j - 1][i - 1];
		}
	}
}

void LoadImage()
{
	loadimage(&BGGame, "./Resources/Pictures/BG/Game.png", WinWidth, WinHeight);
	loadimage(&Back, "./Resources/Pictures/Button/Back.png", SmallButton, SmallButton);
	loadimage(&Exit, "./Resources/Pictures/Button/Exit.png", SmallButton, SmallButton);
	loadimage(&Pause, "./Resources/Pictures/Button/Pause.png", SmallButton, SmallButton);
	loadimage(&CountDown, "./Resources/Pictures/Button/CountDown.png", 715, 50);
	for (size_t i = 0; i < _countof(img); i++)
	{
		string file = "./Resources/Blocks/" + to_string(i + 1) + ".png";
		loadimage(&img[i], file.c_str());
	}
}

void ShowPic()
{
	BeginBatchDraw();
	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			if (Map[j][i] == 0)
				continue;
			putimage(i * PicWidth, j * PicHeight, &img[Map[j][i] - 1]);
		}
	}
	DrawPng(285, 10, &CountDown);
	DrawPng(390, 15, &Time);
	DrawPng(1220, 665, &Exit);
	DrawPng(1160, 665, &Back);
	DrawPng(1100, 665, &Pause);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 3);
	if (MouseClickIndex > 0)
	{
		rectangle(TargetPos[0].X * PicWidth + PicWidth,
			TargetPos[0].Y * PicHeight + PicHeight,
			TargetPos[0].X * PicWidth, TargetPos[0].Y * PicHeight);
	}
	EndBatchDraw();
}

bool NoCornerPath(COORD pos1, COORD pos2)
{
	if (pos1.X != pos2.X && pos2.Y != pos1.Y)
		return false;
	int min = 0, max = 0;
	if (pos1.X == pos2.X)
	{
		min = pos1.Y < pos2.Y ? pos1.Y : pos2.Y;
		max = pos1.Y > pos2.Y ? pos1.Y : pos2.Y;
		for (min++; min < max; min++)
		{
			if (Map[min][pos1.X] != 0)
				return false;
		}
	}
	if (pos1.Y == pos2.Y)
	{
		min = pos1.X < pos2.X ? pos1.X : pos2.X;
		max = pos1.X > pos2.X ? pos1.X : pos2.X;
		for (min++; min < max; min++)
		{
			if (Map[pos1.Y][min] != 0)
				return false;
		}
	}
	return true;
}

bool HaveOneCornerPath(COORD pos1, COORD pos2)
{
	if (pos1.X == pos2.X || pos1.Y == pos2.Y)
		return false;
	COORD temp[2];
	temp[0].X = pos1.X;
	temp[0].Y = pos2.Y;
	temp[1].X = pos2.X;
	temp[1].Y = pos1.Y;
	if (Map[temp[0].Y][temp[0].X] == 0)
	{
		if (NoCornerPath(pos1, temp[0]) == true && NoCornerPath(pos2, temp[0]) == true)
		{
			line(pos1.X * PicWidth + PicWidth / 2,
				pos1.Y * PicHeight + PicHeight / 2,
				temp[0].X * PicWidth + PicWidth / 2,
				temp[0].Y * PicHeight + PicHeight / 2);
			line(pos2.X * PicWidth + PicWidth / 2,
				pos2.Y * PicHeight + PicHeight / 2,
				temp[0].X * PicWidth + PicWidth / 2,
				temp[0].Y * PicHeight + PicHeight / 2);
			return true;
		}
	}
	if (Map[temp[1].Y][temp[1].X] == 0)
	{
		if (NoCornerPath(pos1, temp[1]) == true && NoCornerPath(pos2, temp[1]) == true)
		{
			line(pos1.X * PicWidth + PicWidth / 2,
				pos1.Y * PicHeight + PicHeight / 2,
				temp[1].X * PicWidth + PicWidth / 2,
				temp[1].Y * PicHeight + PicHeight / 2);
			line(pos2.X * PicWidth + PicWidth / 2,
				pos2.Y * PicHeight + PicHeight / 2,
				temp[1].X * PicWidth + PicWidth / 2,
				temp[1].Y * PicHeight + PicHeight / 2);
			return true;
		}
	}
	return false;
}

bool HaveTwoCornerPath(COORD pos1, COORD pos2)
{
	COORD temp;
	for (temp.Y = pos1.Y - 1, temp.X = pos1.X; temp.Y >= 0; temp.Y--)
	{
		if (Map[temp.Y][temp.X] != 0)
			break;
		if (HaveOneCornerPath(pos2, temp))
		{
			line(pos1.X * PicWidth + PicWidth / 2, pos1.Y * PicHeight + PicHeight / 2,
				temp.X * PicWidth + PicWidth / 2, temp.Y * PicHeight + PicHeight / 2);
			return true;
		}
	}
	for (temp.Y = pos1.Y + 1, temp.X = pos1.X; temp.Y < Height; temp.Y++)
	{
		if (Map[temp.Y][temp.X] != 0)
			break;
		if (HaveOneCornerPath(pos2, temp))
		{
			line(pos1.X * PicWidth + PicWidth / 2, pos1.Y * PicHeight + PicHeight / 2,
				temp.X * PicWidth + PicWidth / 2, temp.Y * PicHeight + PicHeight / 2);
			return true;
		}
	}
	for (temp.X = pos1.X - 1, temp.Y = pos1.Y; temp.X >= 0; temp.X--)
	{
		if (Map[temp.Y][temp.X] != 0)
			break;
		if (HaveOneCornerPath(pos2, temp))
		{
			line(pos1.X * PicWidth + PicWidth / 2, pos1.Y * PicHeight + PicHeight / 2,
				temp.X * PicWidth + PicWidth / 2, temp.Y * PicHeight + PicHeight / 2);
			return true;
		}
	}
	for (temp.X = pos1.X + 1, temp.Y = pos1.Y; temp.X < Width; temp.X++)
	{
		if (Map[temp.Y][temp.X] != 0)
			break;
		if (HaveOneCornerPath(pos2, temp))
		{
			line(pos1.X * PicWidth + PicWidth / 2, pos1.Y * PicHeight + PicHeight / 2,
				temp.X * PicWidth + PicWidth / 2, temp.Y * PicHeight + PicHeight / 2);
			return true;
		}
	}
	return false;
}

bool HavePath(COORD targetPos[])
{
	if (NoCornerPath(targetPos[0], targetPos[1]) == true)
	{
		line(TargetPos[0].X * PicWidth + PicWidth / 2,
			TargetPos[0].Y * PicHeight + PicHeight / 2,
			TargetPos[1].X * PicWidth + PicWidth / 2,
			TargetPos[1].Y * PicHeight + PicHeight / 2);
		return true;
	}
	if (HaveOneCornerPath(targetPos[0], targetPos[1]) == true)
		return true;
	if (HaveTwoCornerPath(targetPos[0], targetPos[1]) == true)
		return true;
	return false;
}

void UpdateGame()
{
	if (GameTimeTotal >= CutDownTime)
		GameOver();
	MOUSEMSG msg = GetMouseMsg();
	int row = msg.y / PicHeight;
	int col = msg.x / PicWidth;
	switch (msg.uMsg)
	{
	case WM_MBUTTONDOWN:
		SuggestMap();
		SuggestCount++;
	case WM_LBUTTONDOWN:
		if ((msg.x > 1220 && msg.x < 1280) && (msg.y > 665 && msg.y < 720))
		{
			int n = MessageBox(GetHWnd(), "�Ƿ�Ҫ�˳���Ϸ��", "Genshin_Link", MB_OKCANCEL);
			if (n == IDOK)
			{
				n = MessageBox(GetHWnd(), "�Ƿ�Ҫ���д浵��", "Genshin_Link", MB_OKCANCEL);
				if (n == IDOK)
				{
					WriteOrRead = true;
					SaveDataUI();
					exit(EXIT_SUCCESS);
				}
				else if (n == IDCANCEL)
				{
					closegraph();
					exit(EXIT_SUCCESS);
				}
			}
			else if (n == IDCANCEL)
				return;
		}
		if ((msg.x > 1160 && msg.x < 1220) && (msg.y > 665 && msg.y < 720))
		{
			int n = MessageBox(GetHWnd(), "�Ƿ�Ҫ���������棿", "Genshin_Link", MB_OKCANCEL);
			if (n == IDOK)
			{
				GameScore = Blocks;
				GameTime = GameTimeTotal = 0;
				PicCount = 10;
				WelcomeGameUI();
				return;
			}
			else if (n == IDCANCEL)
				return;
		}
		if ((msg.x > 1100 && msg.x < 1160) && (msg.y > 665 && msg.y < 720))
		{
			GameEnd = clock();
			GameTime = double(GameEnd - GameBegin) / CLOCKS_PER_SEC;
			GameTimeTotal += GameTime;
			GameTime = 0;
			int n = MessageBox(GetHWnd(), "��Ϸ����ͣ�����ȷ�ϼ�����Ϸ��", "Genshin_Link", MB_OK);
			if (n == IDOK)
			{
				GameBegin = clock();
				return;
			}
			return;
		}
		if (Map[row][col] == 0)
			break;
		TargetPos[MouseClickIndex].X = col;
		TargetPos[MouseClickIndex].Y = row;
		if (++MouseClickIndex > 1)
		{
			MouseClickIndex = 0;
			if (TargetPos[0].X == TargetPos[1].X &&
				TargetPos[0].Y == TargetPos[1].Y)
				return;
			if (Map[TargetPos[0].Y][TargetPos[0].X] !=
				Map[TargetPos[1].Y][TargetPos[1].X])
			{
				MouseClickIndex = 1;
				TargetPos[0] = TargetPos[1];
				return;
			}
			if (HavePath(TargetPos) == true)
			{
				rectangle(TargetPos[1].X * PicWidth + PicWidth,
					TargetPos[1].Y * PicHeight + PicHeight,
					TargetPos[1].X * PicWidth, TargetPos[1].Y * PicHeight);
				Sleep(100);

				DrawPng(390, 10, &Time);
				putimage(BGX, BGY, &BGGame);
				for (int i = 0; i < 2; i++)
					Map[TargetPos[i].Y][TargetPos[i].X] = 0;
				GameScore -= 2;
			}
			else
			{
				TargetPos[0] = TargetPos[1];
				MouseClickIndex = 1;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		FlushMap();
		FlushCount++;
		ShowPic();
	}
}

void GamePass()
{
	if (GameScore == 0)
	{
		if (PicCount == 8)
		{
			GameWin();
			return;
		}
		MOUSEMSG msg;
		Sleep(500);
		setlinecolor(GREEN);
		loadimage(&BGGamePass, "./Resources/Pictures/BG/GamePass.png", WinWidth, WinHeight);
		putimage(BGX, BGY, &BGGamePass);

		loadimage(&ContinueGame, "./Resources/Pictures/Button/ContinueGame.png", ButtonWidth, ButtonHeight);
		loadimage(&ExitGame, "./Resources/Pictures/Button/ExitGame.png", ButtonWidth, ButtonHeight);

		DrawPng(395, 205, &ContinueGame);
		DrawPng(395, 530, &ExitGame);

		loadimage(&Click, "./Resources/Pictures/Button/Click.png", SmallButton, SmallButton);
		while (true)
		{
			msg = GetMouseMsg();
			switch (msg.uMsg)
			{
			case WM_LBUTTONDOWN:
				if ((msg.x > 395 && msg.x < ButtonWidth + 395) && (msg.y > 205 && msg.y < ButtonHeight + 205))
				{
					DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
					Sleep(500);
					PicCount -= 2;
					GameScore = Blocks;
					GameRank += Blocks;
					GameTimeTotal += GameTime;
					TimeTemp = GameTimeTotal;
					GameTimeTotal = 0; GameTime = 0;
					InitGame();
					ShowPic();
					putimage(BGX, BGY, &BGGame);
					return;
				}
				if ((msg.x > 395 && msg.x < ButtonWidth + 395) && (msg.y > 530 && msg.y < ButtonHeight + 530))
				{
					DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
					Sleep(500);
					int n = MessageBox(GetHWnd(), "�Ƿ�Ҫд�����а�", "Genshin_Link", MB_OKCANCEL);
					if (n == IDOK)
					{
						GameRank = GameRank + Blocks - (FlushCount * 2) - (SuggestCount * 5);
						WriteRank();
						exit(EXIT_SUCCESS);
					}
					else if (n == IDCANCEL)
						exit(EXIT_SUCCESS);
				}
			}
		}
	}
}

void SortRank()
{
	Player* p;
	p = new Player[Players];

	ifstream InSortRankFile("./Resources/Documents/Rank/Rank.txt");
	for (int i = 0; i < Players; i++)
		InSortRankFile >> p[i].Name >> p[i].Rank >> p[i].Time;
	InSortRankFile.close();

	sort(p, p + Players, cmp);

	ofstream OutSortRankFile("./Resources/Documents/Rank/Rank.txt");
	for (int i = 0; i < Players; i++)
		OutSortRankFile << p[i].Name << " " << p[i].Rank << " " << p[i].Time << endl;
	OutSortRankFile.close();
}

bool cmp(Player arr, Player arrs)
{
	if (arr.Rank != arrs.Rank)
		return arr.Rank > arrs.Rank;
	else
	{
		if (arr.Rank == arrs.Rank)
			return arr.Time < arrs.Time;
		if (arr.Time == arrs.Time)
			return arr.Name < arrs.Name;
	}
}

void WriteRank()
{
	ifstream InWriteRankData("./Resources/Documents/Rank/Players.txt");
	InWriteRankData >> Players;
	InWriteRankData.close();
	ofstream OutWriteRankData("./Resources/Documents/Rank/Players.txt");
	Players++;
	OutWriteRankData << Players;
	OutWriteRankData.close();

	TCHAR player[10];
	ofstream OutWriteRankFile("./Resources/Documents/Rank/Rank.txt", ios::app);
	InputBox(player, 10, _T("������������֣�10�ַ�����\n"));
	string str(player);
	GameTimeTotal += TimeTemp;
	OutWriteRankFile << player << " " << (double)GameRank << " " << (double)GameTimeTotal << endl;
	OutWriteRankFile.close();
	SortRank();
	MessageBox(GetHWnd(), "������д��", "Vectory", MB_OK);
}

void FlushMap()
{
	int TempMap[(Height - 2) * (Width - 2)] = { 0 };
	int count = 0;
	for (int i = 0; i < Height - 1; i++)
	{
		for (int j = 0; j < Width - 1; j++)
		{
			if (Map[i][j] == 0)
				continue;
			else
			{
				TempMap[count] = Map[i][j];
				Map[i][j] = -1;
				count++;
			}
		}
	}
	random_shuffle(TempMap, TempMap + count);
	count = 0;
	for (int i = 0; i < Height - 1; i++)
	{
		for (int j = 0; j < Width - 1; j++)
		{
			if (Map[i][j] == -1 && TempMap[count] != 0)
			{
				Map[i][j] = TempMap[count];
				count++;
			}
		}
	}
}

void WelcomeGameUI()
{
	loadimage(&BGWelcome, "./Resources/Pictures/BG/Welcome.png", WinWidth, WinHeight);
	loadimage(&LLKLogo, "./Resources/Pictures/BG/LLKLogo.png", 400, 300);

	loadimage(&Click, "./Resources/Pictures/Button/Click.png", SmallButton, SmallButton);
	loadimage(&Guide, "./Resources/Pictures/Button/Guide.png", SmallButton, SmallButton);
	loadimage(&BeginGame, "./Resources/Pictures/Button/BeginGame.png", ButtonWidth, ButtonHeight);
	loadimage(&ContinueGame, "./Resources/Pictures/Button/ContinueGame.png", ButtonWidth, ButtonHeight);
	loadimage(&LoadRank, "./Resources/Pictures/Button/LoadRank.png", ButtonWidth, ButtonHeight);

	putimage(BGX, BGY, &BGWelcome);
	DrawPng(LogoX, LogoY, &LLKLogo);
	DrawPng(0, 665, &Guide);
	DrawPng(100, 350, &BeginGame);
	DrawPng(100, 450, &ContinueGame);
	DrawPng(100, 550, &LoadRank);
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((msg.x > 100 && msg.x < ButtonWidth + 100) && (msg.y > 350 && msg.y < ButtonHeight + 350))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				InitGame();
				LoadImage();
				putimage(BGX, BGY, &BGGame);
				return;
			}
			if ((msg.x > 100 && msg.x < ButtonWidth + 100) && (msg.y > 450 && msg.y < ButtonHeight + 450))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				SaveDataUI();
				return;
			}
			if ((msg.x > 100 && msg.x < ButtonWidth + 100) && (msg.y > 550 && msg.y < ButtonHeight + 550))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				ShowRank();
				return;
			}
			if ((msg.x > 0 && msg.x < SmallButton) && (msg.y > 665 && msg.y < 720))
			{
				GameGuide();
				return;
			}
		default:
			break;
		}
	}
}

void DrawPng(int X, int Y, IMAGE* picture)
{
	//������ʼ��
	DWORD* dst = GetImageBuffer();				//GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ��
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);		//��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth();	//��ȡpicture�Ŀ��
	int picture_height = picture->getheight();	//��ȡpicture�ĸ߶�
	int graphWidth = getwidth();				//��ȡ��ͼ���Ŀ��
	int graphHeight = getheight();				//��ȡ��ͼ���ĸ߶�
	int dstX = 0;								//���Դ������صĽǱ�
	//ʵ��͸����ͼ 
	//��ʽ�� Cp=��p*FP+(1-��p)*BP,��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width;			//���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24);	//0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16);	//��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);		//G
			int sb = src[srcX] & 0xff;					//B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + X) + (iy + Y) * graphWidth;		//���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)	//��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)			//��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);					//��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void SaveData(int num)
{
	int Temp[10] = { 0 };
	ifstream InSaveDataFile("./Resources/Documents/Save/SaveDataCount.txt");
	for (int i = 0; i < 6; i++)
		InSaveDataFile >> Temp[i];
	InSaveDataFile.close();
	SaveDataCount = Temp[0];
	if (SaveDataCount < 5)
	{
		SaveDataCount++;
		for (int i = 1; i <= 5; i++)
		{
			if (i == num && Temp[i] == 0)
			{
				wofstream OutSaveFile("./Resources/Documents/Save/data" +
					to_string(num) + ".txt");
				wofstream OutSaveData("./Resources/Documents/Save/SaveDataCount.txt");
				wofstream OutPlayData("./Resources/Documents/Save/PlayData" +
					to_string(num) + ".txt");
				if (OutSaveFile)
				{
					for (int i = 0; i < Height; i++)
					{
						for (int j = 0; j < Width; j++)
							OutSaveFile << Map[i][j] << " ";
						OutSaveFile << endl;
					}
					Temp[i] = 1;
				}
				OutSaveData << SaveDataCount << " " << Temp[1] << " " << Temp[2] << " " << Temp[3]
					<< " " << Temp[4] << " " << Temp[5];
				OutPlayData << GameScore << " " << GameTimeTotal << " " << PicCount;
				OutSaveFile.close();
				OutSaveData.close();
				OutPlayData.close();
				MessageBox(GetHWnd(), "�浵��д��", "Genshin_Link", MB_OK);
				exit(EXIT_SUCCESS);
			}
		}
	}
	else
	{
		MessageBox(GetHWnd(), "�浵�������޷��浵", "Genshin_Link", MB_OK);
		return;
	}
	return;
}

void LoadSaveUI()
{
	loadimage(&BGSave, "./Resources/Pictures/BG/Save.png", WinWidth, WinHeight);
	loadimage(&NotSaved, "./Resources/Pictures/Button/NotSaved.png", BigButton, BigButton);
	loadimage(&Saved, "./Resources/Pictures/Button/Saved.png", BigButton, BigButton);
	loadimage(&Back, "./Resources/Pictures/Button/Back.png", SmallButton, SmallButton);
	putimage(BGX, BGY, &BGSave);
	DrawPng(1220, 665, &Back);
}

void UpdateSaveUI()
{
	int Temp[10] = { 0 };
	ifstream InSaveDataFile("./Resources/Documents/Save/SaveDataCount.txt");
	for (int i = 0; i < 6; i++)
		InSaveDataFile >> Temp[i];
	InSaveDataFile.close();
	SaveDataCount = Temp[0];
	if (SaveDataCount == 0)
		MessageBox(GetHWnd(), "���ػ�û�д浵�ļ��޷����ж���", "Genshin_Link", MB_OK);
	LoadSaveUI();
	int ButtonX[6] = { 0,45,290,545,795,1050 };
	for (int i = 1; i <= 5; i++)
	{
		if (Temp[i] == 1)
		{
			DrawPng(ButtonX[i], 290, &Saved);
			continue;
		}
		else
		{
			DrawPng(ButtonX[i], 290, &NotSaved);
			continue;
		}
	}
}

bool LoadSaveData(int num)
{
	ifstream InLoadSaveFile("./Resources/Documents/Save/data" +
		to_string(num) + ".txt");
	if (!InLoadSaveFile)
	{
		MessageBox(GetHWnd(), "���ػ�û�д浵�ļ��޷����ж���", "Genshin_Link", MB_OK);
		return false;
	}
	else
	{
		for (int j = 0; j < Height; j++)
			for (int i = 0; i < Width; i++)
				InLoadSaveFile >> Map[j][i];
		InLoadSaveFile.close();
		ifstream InLoadPlayData("./Resources/Documents/Save/PlayData" +
			to_string(num) + ".txt");
		InLoadPlayData >> GameScore >> GameTime >> PicCount;
		InLoadPlayData.close();
	}
	return true;
}

void PutLoadDataPic()
{
	LoadImage();
	putimage(BGX, BGY, &BGGame);
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (Map[i][j] == 0)
				continue;
			else
				putimage(j * PicWidth, i * PicHeight, &img[Map[i][j]]);
		}
	}
	return;
}

void SaveButtonChoose(int num)
{
	switch (num)
	{
	case 1: {
		rectangle(SaveButtonInterval, 215, (SaveButtonWidth * 1) +
			SaveButtonInterval, 500);
		Sleep(500);
		if (WriteOrRead)
			SaveData(num);
		else
		{
			if (LoadSaveData(num))
			{
				PutLoadDataPic();
				break;
			}
			else
			{
				SaveDataUI();
				return;
			}
		}
	}
	case 2: {
		rectangle(SaveButtonInterval * 2 + SaveButtonWidth, 215,
			(SaveButtonWidth + SaveButtonInterval) * 2, 500);
		Sleep(500);
		if (WriteOrRead)
			SaveData(num);
		else
		{
			if (LoadSaveData(num))
			{
				PutLoadDataPic();
				break;
			}
			else
			{
				SaveDataUI();
				return;
			}
		}
	}
	case 3: {
		rectangle((SaveButtonInterval * 3) + (SaveButtonWidth * 2) + 5, 215,
			(SaveButtonWidth + SaveButtonInterval) * 3 + 5, 500);
		Sleep(500);
		if (WriteOrRead)
			SaveData(num);
		else
		{
			if (LoadSaveData(num))
			{
				PutLoadDataPic();
				break;
			}
			else
			{
				SaveDataUI();
				return;
			}
		}
	}
	case 4: {
		rectangle((SaveButtonInterval * 4) + (SaveButtonWidth * 3) + 12, 215,
			(SaveButtonWidth + SaveButtonInterval) * 4 + 10, 500);
		Sleep(500);
		if (WriteOrRead)
			SaveData(num);
		else
		{
			if (LoadSaveData(num))
			{
				PutLoadDataPic();
				break;
			}
			else
			{
				SaveDataUI();
				return;
			}
		}
	}
	case 5: {
		rectangle((SaveButtonInterval * 5) + (SaveButtonWidth * 4) + 25, 215,
			(SaveButtonWidth + SaveButtonInterval) * 5 + 23, 500);
		Sleep(500);
		if (WriteOrRead)
			SaveData(num);
		else
		{
			if (LoadSaveData(num))
			{
				PutLoadDataPic();
				break;
			}
			else
			{
				SaveDataUI();
				return;
			}
		}
	}
	}
}

void SaveDataUI()
{
	int num = 0;
	MOUSEMSG msg;
	UpdateSaveUI();
	while (true)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((msg.x > 1220 && msg.x < 1280) && (msg.y > 660 && msg.y < 720)) {
				WelcomeGameUI();
				return;
			}
			if ((msg.x > SaveButtonInterval * 1 && msg.x < SaveButtonWidth) && (msg.y > 215 && msg.y < 500)) {
				SaveButtonChoose(1);
				return;
			}
			if ((msg.x > SaveButtonInterval * 2 + SaveButtonWidth && msg.x < (SaveButtonWidth + SaveButtonInterval) * 2)
				&& (msg.y > 215 && msg.y < 500)) {
				SaveButtonChoose(2);
				return;
			}
			if ((msg.x > (SaveButtonInterval * 3) + (SaveButtonWidth * 2) && msg.x < (SaveButtonWidth + SaveButtonInterval) * 3)
				&& (msg.y > 215 && msg.y < 500)) {
				SaveButtonChoose(3);
				return;
			}
			if ((msg.x > (SaveButtonInterval * 4) + (SaveButtonWidth * 3) && msg.x < (SaveButtonWidth + SaveButtonInterval) * 4)
				&& (msg.y > 215 && msg.y < 500)) {
				SaveButtonChoose(4);
				return;
			}
			if ((msg.x > (SaveButtonInterval * 5) + (SaveButtonWidth * 4) && msg.x < (SaveButtonWidth + SaveButtonInterval) * 5)
				&& (msg.y > 215 && msg.y < 500)) {
				SaveButtonChoose(5);
				return;
			}
		case WM_RBUTTONDOWN:
			if ((msg.x > 1220 && msg.x < 1280) && (msg.y > 660 && msg.y < 720)) {
				WelcomeGameUI();
				return;
			}
			if ((msg.x > SaveButtonInterval * 1 && msg.x < SaveButtonWidth) && (msg.y > 215 && msg.y < 500)) {
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				DeleteData(1);
				return;
			}
			if ((msg.x > SaveButtonInterval * 2 + SaveButtonWidth && msg.x < (SaveButtonWidth + SaveButtonInterval) * 2)
				&& (msg.y > 215 && msg.y < 500)) {
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				DeleteData(2);
				return;
			}
			if ((msg.x > (SaveButtonInterval * 3) + (SaveButtonWidth * 2) && msg.x < (SaveButtonWidth + SaveButtonInterval) * 3)
				&& (msg.y > 215 && msg.y < 500)) {
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				DeleteData(3);
				return;
			}
			if ((msg.x > (SaveButtonInterval * 4) + (SaveButtonWidth * 3) && msg.x < (SaveButtonWidth + SaveButtonInterval) * 4)
				&& (msg.y > 215 && msg.y < 500)) {
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				DeleteData(4);
				return;
			}
			if ((msg.x > (SaveButtonInterval * 5) + (SaveButtonWidth * 4) && msg.x < (SaveButtonWidth + SaveButtonInterval) * 5)
				&& (msg.y > 215 && msg.y < 500)) {
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				DeleteData(5);
				return;
			}
		default:
			break;
		}
	}
}

void ShowRank()
{
	setbkmode(TRANSPARENT);
	loadimage(&BGRank, "./Resources/Pictures/BG/Rank.png", WinWidth, WinHeight);
	loadimage(&Back, "./Resources/Pictures/Button/Back.png", SmallButton, SmallButton);
	putimage(BGX, BGY, &BGRank);
	DrawPng(1220, 665, &Back);
	ifstream InShowRankData("./Resources/Documents/Rank/Players.txt");
	InShowRankData >> Players;
	InShowRankData.close();

	TCHAR Name[10];
	TCHAR Rank[10];
	TCHAR Time[10];

	LOGFONT Front;
	gettextstyle(&Front);
	Front.lfHeight = 50;
	settextcolor(BLACK);
	_tcscpy_s(Front.lfFaceName, _T("���ķ���"));
	Front.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&Front);

	MOUSEMSG msg;
	ifstream InShowRankFile("./Resources/Documents/Rank/Rank.txt");
	for (int i = 1; i <= Players; i++)
	{
		if (i == 12)
			break;

		InShowRankFile >> Name >> Rank >> Time;
		int NameXL = 50, NameXR = 650;
		int TimeXL = 250, TimeXR = 870;
		int RankXL = 490, RankXR = 1095;

		if (i % 2 != 0)
		{
			outtextxy(NameXL, 205 + ((i - 1) * 25), Name);
			outtextxy(TimeXL, 205 + ((i - 1) * 25), Time);
			outtextxy(RankXL, 205 + ((i - 1) * 25), Rank);
			continue;
		}
		else
		{
			outtextxy(NameXR, 205 + ((i - 2) * 25), Name);
			outtextxy(TimeXR, 205 + ((i - 2) * 25), Time);
			outtextxy(RankXR, 205 + ((i - 2) * 25), Rank);
			continue;
		}
	}
	InShowRankFile.close();

	while (true)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((msg.x > 1220 && msg.x < 1280) && (msg.y > 660 && msg.y < 720)) {
				WelcomeGameUI();
				return;
			}
		}
	}
}

void GameOver()
{
	MOUSEMSG msg;
	Sleep(500);
	loadimage(&BGGameOver, "./Resources/Pictures/BG/GameOver.png", WinWidth, WinHeight);
	putimage(BGX, BGY, &BGGameOver);

	loadimage(&RePlay, "./Resources/Pictures/Button/RePlay.png", ButtonWidth, ButtonHeight);
	loadimage(&LoadData, "./Resources/Pictures/Button/LoadData.png", ButtonWidth, ButtonHeight);
	loadimage(&LoadRank, "./Resources/Pictures/Button/LoadRank.png", ButtonWidth, ButtonHeight);
	loadimage(&ExitGame, "./Resources/Pictures/Button/ExitGame.png", ButtonWidth, ButtonHeight);
	DrawPng(540, 60, &RePlay);
	DrawPng(540, 235, &LoadData);
	DrawPng(540, 405, &LoadRank);
	DrawPng(540, 580, &ExitGame);

	loadimage(&Click, "./Resources/Pictures/Button/Click.png", SmallButton, SmallButton);
	while (true)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((msg.x > 540 && msg.x < 540 + ButtonWidth) && (msg.y > 60 && msg.y < ButtonHeight + 60))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				GameScore = Blocks;
				GameTimeTotal = 0; GameTime = 0; FlushCount = 0; SuggestCount = 0;
				InitGame();
				ShowPic();
				putimage(BGX, BGY, &BGGame);
				return;
			}
			if ((msg.x > 540 && msg.x < 540 + ButtonWidth) && (msg.y > 235 && msg.y < ButtonHeight + 235))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				SaveDataUI();
				return;
			}
			if ((msg.x > 540 && msg.x < 540 + ButtonWidth) && (msg.y > 405 && msg.y < ButtonHeight + 405))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				ShowRank();
				return;
			}
			if ((msg.x > 540 && msg.x < 540 + ButtonWidth) && (msg.y > 580 && msg.y < ButtonHeight + 580))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				exit(EXIT_SUCCESS);
			}
		}
	}
}

void GameWin()
{
	MOUSEMSG msg;
	Sleep(500);
	loadimage(&BGGameWin, "./Resources/Pictures/BG/GameWin.png", WinWidth, WinHeight);
	putimage(BGX, BGY, &BGGameWin);

	loadimage(&BackWelcome, "./Resources/Pictures/Button/BackWelcome.png", ButtonWidth, ButtonHeight);
	loadimage(&ExitGame, "./Resources/Pictures/Button/ExitGame.png", ButtonWidth, ButtonHeight);
	DrawPng(535, 150, &BackWelcome);
	DrawPng(535, 440, &ExitGame);
	GameTimeTotal += GameTime;
	GameRank = GameRank + Blocks - (FlushCount * 2) - (SuggestCount * 5);
	FlushCount = SuggestCount = 0;
	WriteRank();
	loadimage(&Click, "./Resources/Pictures/Button/Click.png", SmallButton, SmallButton);
	while (true)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((msg.x > 535 && msg.x < ButtonWidth + 535) && (msg.y > 150 && msg.y < ButtonHeight + 150))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				PicCount = 10;
				GameScore = Blocks;
				GameTimeTotal = GameTime = 0;
				WelcomeGameUI();
				return;
			}
			if ((msg.x > 535 && msg.x < ButtonWidth + 535) && (msg.y > 440 && msg.y < ButtonHeight + 440))
			{
				DrawPng(msg.x - (SmallButton / 2), msg.y - (SmallButton / 2), &Click);
				Sleep(500);
				exit(EXIT_SUCCESS);
			}
		}
	}
	return;
}

void SuggestMap()
{
	COORD Temp[2] = { -1,-1 };
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			Temp[0].X = j;
			Temp[0].Y = i;
			for (int x = 0; x < Height; x++)
			{
				for (int y = 0; y < Width; y++)
				{
					Temp[1].X = y;
					Temp[1].Y = x;
					if (!(i == x || y == j)
						&& Map[i][j] == Map[x][y] && Map[i][j] != 0 && HavePath(Temp))
					{
						setlinecolor(WHITE);
						rectangle(y * PicWidth + PicWidth,
							x * PicHeight + PicHeight,
							y * PicWidth, x * PicHeight);
						rectangle(j * PicWidth + PicWidth,
							i * PicHeight + PicHeight,
							j * PicWidth, i * PicHeight);
						Sleep(500);
						return;
					}
				}
			}
		}
	}
	return;
}

void DeleteData(int num)
{
	int n = MessageBox(GetHWnd(), "�Ƿ�Ҫɾ���ô浵��", "Genshin_Link", MB_OKCANCEL);
	if (n == IDOK)
	{
		string DeleteData = "./Resources/Documents/Save/data" +
			to_string(num) + ".txt";
		LPCSTR str = DeleteData.c_str();
		if (DeleteFile(str) != 0)
		{
			string DeletePlay = "./Resources/Documents/Save/PlayData" +
				to_string(num) + ".txt";
			LPCSTR str1 = DeletePlay.c_str();
			DeleteFile(str1);
			int Temp[10] = { 0 };
			ifstream InSaveDataFile("./Resources/Documents/Save/SaveDataCount.txt");
			for (int i = 0; i < 6; i++)
				InSaveDataFile >> Temp[i];
			InSaveDataFile.close();
			SaveDataCount = Temp[0];
			SaveDataCount--;
			Temp[num] = 0;
			for (int i = 1; i <= 5; i++)
			{
				wofstream OutSaveData("./Resources/Documents/Save/SaveDataCount.txt");
				OutSaveData << SaveDataCount << " " << Temp[1] << " " << Temp[2] << " " << Temp[3]
					<< " " << Temp[4] << " " << Temp[5];
				OutSaveData.close();
				MessageBox(GetHWnd(), "�浵��ɾ��", "Genshin_Link", MB_OK);
				SaveDataUI();
				return;
			}
		}
		else
		{
			MessageBox(GetHWnd(), "�浵�����ڣ��޷�ɾ��", "Genshin_Link", MB_OK);
			SaveDataUI();
		}
		return;
	}
	else if (n == IDCANCEL)
		return;
	return;
}

void GameGuide()
{
	loadimage(&BGGuide, "./Resources/Pictures/BG/Guide.png", WinWidth, WinHeight);
	putimage(BGX, BGY, &BGGuide);
	loadimage(&Back, "./Resources/Pictures/Button/Back.png", SmallButton, SmallButton);
	DrawPng(1220, 665, &Back);
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((msg.x > 1220 && msg.x < 1280) && (msg.y > 660 && msg.y < 720)) {
				WelcomeGameUI();
				return;
			}
		}
	}
	return;
}