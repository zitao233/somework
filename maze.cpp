#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<string.h>

//ÉèÖÃ¹â±êÎ»ÖÃ
void SetPos(COORD a)// set cursor 
{
	HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);
}
void SetPos(int i, int j)// set cursor
{
	COORD pos={i, j};
	SetPos(pos);
}

void HideCursor(int i)//Òþ²Ø£¨0£©/ÏÔÊ¾£¨1£©¹â±ê
{
	CONSOLE_CURSOR_INFO cursor_info = {1, i}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//°ÑµÚyÐÐ£¬[x1, x2) Ö®¼äµÄ×ø±êÌî³äÎª ch
void drawRow(int y, int x1, int x2, char ch)
{
	SetPos(x1,y);
	for(int i = 0; i <= (x2-x1); i++)
		printf("%c",ch);
}
//ÔÚa, b ×Ý×ø±êÏàÍ¬µÄÇ°ÌáÏÂ£¬°Ñ×ø±ê [a, b] Ö®¼äÌî³äÎª ch
void drawRow(COORD a, COORD b, char ch)
{
	if(a.Y == b.Y)
		drawRow(a.Y, a.X, b.X, ch);
	else
	{
		SetPos(0, 25);
		printf("error code 01£ºÎÞ·¨Ìî³äÐÐ£¬ÒòÎªÁ½¸ö×ø±êµÄ×Ý×ø±ê(x)²»ÏàµÈ");
		system("pause");
	}
}

//°ÑµÚxÁÐ£¬[y1, y2] Ö®¼äµÄ×ø±êÌî³äÎª ch
void drawCol(int x, int y1, int y2, char ch)
{
	int y=y1;
	while(y<y2+1)
	{
		SetPos(x, y);printf("%c",ch);
		y++;
	}
}
//ÔÚa, b ºá×ø±êÏàÍ¬µÄÇ°ÌáÏÂ£¬°Ñ×ø±ê [a, b] Ö®¼äÌî³äÎª ch
void drawCol(COORD a, COORD b, char ch)
{
	if(a.X == b.X)
		drawCol(a.X, a.Y, b.Y, ch);
	else
	{
		SetPos(0, 25);
		printf("error code 02£ºÎÞ·¨Ìî³äÁÐ£¬ÒòÎªÁ½¸ö×ø±êµÄºá×ø±ê(y)²»ÏàµÈ");
		system("pause");
	}
}

int control()//¿ØÖÆº¯Êý£¬·µ»ØÉÏÏÂ×óÓÒ¡¢ESC
{
		int ch;
		if( (ch=getch())==0x1B ) return 5;/* Press ESC to quit... */
		if( ch==13 ) return 6;
		switch(ch)
		{
			case 0xE0:
			switch(ch=getch())
			{
				case 72:  return 1;break;//UP
				case 80:  return 2;break;//DOWN
				case 75:  return 3;break;//LEFT
				case 77:  return 4;break;//RIGHT
				default: break;
				}break;
			default: return ch;break;
		}
	return 0;
}

void drawmaze(int** M,int x,int y)//»æÖÆÃÔ¹¬
{
	int m,n,i,j;
	for(i=0;i<m;i++)
	{
		SetPos(x,y+i);
		for(j=0;j<n;j++)
			if(M[i][j]==1)printf("¡ö");
			else if(M[i][j]==0)printf("¡õ");
			else if(M[i][j]==-1)printf("¡ñ");
			else if(M[i][j]==2)printf("¡ù");
			
	}
}

void drawmenu()//²Ëµ¥±ß¿ò
{
	//mainmenu
	system("cls");
	SetPos(30, 1);printf("M A Z E     G A M E");
	drawRow(3, 0, 79, '-');drawRow(5, 0, 79, '-');
	SetPos(15, 4);printf("·½Ïò¼ü ÉÏ ºÍ ÏÂ Ñ¡Ôñ£¬ »Ø³µ È·¶¨£¬ ESC ½áÊø/·µ»Ø");
	drawRow(20, 0, 79, '-');drawRow(22, 0, 79, '-');
	SetPos(24, 21);printf("ÖÆ×÷:    ±ß×ÓÌÎ    bztup@qq.com");
}

void drawPlaying()//ÓÎÏ·½çÃæ
{}

void readM(char*fname,int** M)//ÌáÈ¡ÃÔ¹¬¾ØÕó
{
	//´ò¿ªÎÄ¼þr
	//
}

void writeM(char*fname,int** M)//½«ÃÔ¹¬¾ØÕóÐ´ÈëÎÄ¼þ
{
	//´ò¿ªÎÄ¼þw
	//
}

void drawselect()//Ñ¡Ôñ½çÃæ
{}

int** selectmaze()//´ÓÒÑÓÐÑ¡ÔñÃÔ¹¬²¢´«»ØÃÔ¹¬Êý×é
{
	drawmenu();
	int** M;
	//readM();
	//printfÑ¡Ôñ½çÃæ
	//Ä¬ÈÏÃÔ¹¬¡¢ÃÔ¹¬´æµµ
	//reprint Ñ¡Ôñ>> ÃÔ¹¬Ô¤ÀÀdrawmaze(int** M,int x,int y)
	//control()//ÉÏÏÂÑ¡Ôñ£¬»Ø³µÈ·ÈÏ
	return M;
	return 0;
}

void drawset()//ÐÂ½¨½çÃæ
{
	//Éè¶¨½çÃæ(·½Ïò¼üÑ¡Ôñ>>¿Õ¸ñ±äµØÐÎ¡¢×óÊý×ÖÐÐÑ¡Ïî(É¾³ý/ÐÂ½¨ÐÐ)¡¢ÉÏÊý×ÖÁÐÑ¡Ïî(É¾³ý/ÐÂ½¨ÁÐ)>>±ê¼ÇÆðµãA¡¢±ê¼ÇÖÕµãB»Ø³µÈ·ÈÏ)
}

int setmaze(int k)//Éè¶¨¡¢ÐÞ¸ÄÃÔ¹¬
{
	drawmenu();
	//readM();
	//printfÑ¡Ôñ½çÃædrawmaze(int** M,int x,int y)
	//Ñ¡Ïî
	//(ÐÂ½¨µØÍ¼>>»Ø³µ>>Éè¶¨½çÃæ)
	//(µØÍ¼Ñ¡Ïî>>»Ø³µÑ¡ÖÐ>>È·ÈÏÐÞ¸Ä£¿>>»Ø³µ>>Éè¶¨½çÃæ)
	//Ä¬ÈÏÃÔ¹¬¡¢ÃÔ¹¬´æµµ
	//reprint Ñ¡Ôñ>> ÃÔ¹¬Ô¤ÀÀ
	//control();//ÉÏÏÂ×óÓÒÑ¡Ôñ£¬»Ø³µÈ·ÈÏ
	//return M;
	return 0;
}

int game()
{
	char a;
	int x,y;
	int** M;
	//Ñ¡ÔñµØÍ¼
	//Ñ¡ÔñÄ£Ê½£¨×ÔÓÉ¡¢¼ÆÊ±£©
	drawmaze(M,x,y);//»æÖÆµØÍ¼¡¢»æÖÆÀÏÊó
	//control();
	return 0;
}

int load()//È«²¿Â·¾¶¡¢×îÓÅÂ·¾¶£¨¹ã¶ÈÓÅÏÈ±éÀú£©
{return 0;}

int mainmenu()
{
	drawmenu();
	SetPos(15, 11);printf("¿ªÊ¼ÓÎÏ·");
	SetPos(15, 13);printf("ÉèÖÃµØÍ¼");
	SetPos(42, 11);printf("¿ªÊ¼ÓÎÏ·£º");
	SetPos(46, 13);printf("Ñ¡ÔñµØÍ¼²¢¿ªÊ¼ÓÎÏ·¡£");
	int j=11,x;
	SetPos(12, j);printf(">>");	
	while(x=control())
	{
		switch (x)
		{
		case 1 :	
				if( j == 13)
				{	
					SetPos(12, j);printf("¡¡");j = 11;
					SetPos(12, j);printf(">>");
					SetPos(51, 13);printf("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡  ");
					SetPos(42, 11);printf("¿ªÊ¼ÓÎÏ·£º                ");
					SetPos(46, 13);printf("Ñ¡ÔñµØÍ¼²¢¿ªÊ¼ÓÎÏ·¡£      ");
				}
				break;
		case 2 :	
				if( j == 11 )
				{	
					SetPos(12, j);printf("¡¡");	j = 13;
					SetPos(12, j);printf(">>");
					SetPos(51, 13);printf("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡");
					SetPos(42, 11);printf("ÉèÖÃµØÍ¼£º                ");
					SetPos(46, 13);printf("½¨Á¢ÐÂµØÍ¼»ò¸ü¸ÄµØÍ¼´æµµ¡£");
				}
				break;
		case 6 :if (j == 11)	return 1;else return 2;
		case 5 :return 0;break;
		}	
	}
	return 0;
}


void main()
{
	printf("start_test\n");//ÓÎÏ·×¼±¸
	HideCursor(0);	//Òþ²Ø¹â±ê
	int **M,x;
	x=mainmenu();
	if(x==1) 
	{system("cls");printf("¿ªÊ¼ÓÎÏ·");//game();
	} 
	else 
		printf("ÉèÖÃµØÍ¼");//setmaze();




	//system("cls");
	//test
	//for(int i=1;i<=10;i++)
	//{
	//	x=control();
	//	SetPos(2*i, 2*i);
	//	printf("%d\n",x);
	//}
	printf("end_test\n");
}
