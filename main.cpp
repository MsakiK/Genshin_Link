#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include<algorithm>
using namespace std;

#define png_len 80  //ͼƬ�߳�
#define start_x 150 //x��ʼ��
#define start_y 150 //y��ʼ��
#define mapsize_easy 4	//ͼƬ��ͼ
int png_numbers[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
const int pngname_length = 1000; //ͼƬ��ַ����
const int png_num = 16;	//����ͼƬ����
IMAGE img[png_num];

struct mapnum
{
	int x;
	int y;
	int png_num;//ͼƬ���
	IMAGE* img = NULL;
};
mapnum map[mapsize_easy + 5][mapsize_easy + 5];

//��ʼ������
void init_map()
{
	initgraph(600, 600);
	setorigin(start_x, start_y);
	wchar_t pngname[pngname_length];

	int rands_png[png_num] = { 0 };
	int rands_local[mapsize_easy] = { 0 };
	srand((unsigned)time(NULL));
	for (int i = 0; i < png_num; i++)
	{
		rands_png[i] = (rand() % png_num) + 1;
	}
	for (int i = 0; i < mapsize_easy; i++)
	{
		//for (int j = 0; j < mapsize_easy; j++)
		//{
			rands_local[i] = (rand() % mapsize_easy) + 1;
		//}
	}

	for (int i = 0; i <= mapsize_easy + 1; i++)
	{
		for (int j = 0; j <= mapsize_easy + 1; j++)
		{
			map[i][j].x = png_len * i;
			map[i][j].y = png_len * j;
			if (i > 0 && i <= mapsize_easy && j > 0 && j <= mapsize_easy)
			{
				swprintf_s(pngname, L"D:\\Genshin_Link\\source\\Mond\\%d.png", png_numbers[rands_png[i]]);
				loadimage(&img[png_numbers[rands_png[i]]], pngname, png_len, png_len);
				map[i][j].img = &img[png_numbers[rands_png[i]]];
				map[i + 1][j].img = &img[png_numbers[rands_png[i]]];
			}
		}
	}

}

//��ʾͼƬ
void print_png()
{
	for (int i = 1; i <= mapsize_easy; i++)
	{
		for (int j = 1; j <= mapsize_easy; j++)
		{
			if (map[i][j].img != NULL)
				putimage(map[i][j].x, map[i][j].y, map[i][j].img);
		}
	}
}

int main()
{
	init_map();
	print_png();
	system("pause");
}

//if (i > 0 && i <= mapsize_easy && j > 0 && j <= mapsize_easy)
//{
//	map[i][j].png_num = No_map;
//	map[i][j + 1].png_num = No_map;
//	if (i % 2 == 1)
//	{
//		swprintf_s(pngname, L"D:\\Genshin_Link\\source\\Mond\\%d.png", png_numbers[i]);
//		loadimage(&img[png_numbers[i]], pngname, png_len, png_len);
//		map[i][j].img = &img[png_numbers[No_map]];
//		map[i + 1][j].img = &img[png_numbers[No_map]];
//		No_map = No_map + 1;
//	}
//}