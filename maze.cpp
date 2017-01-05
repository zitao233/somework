#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<string.h>
#include<time.h>
#define LEN sizeof(struct Maze)

/*=============== all the structures ===============*/
typedef struct Maze
{
	int **M;
	int m;
	int n;
}Maze;
/*=============== all the functions ===============*/

//设置光标位置
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

void HideCursor(int i)//隐藏（0）/显示（1）光标
{
	CONSOLE_CURSOR_INFO cursor_info = {1, i}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//把第y行，[x1, x2) 之间的坐标填充为 ch
void drawRow(int y, int x1, int x2, char ch)
{
	SetPos(x1,y);
	for(int i = 0; i <= (x2-x1); i++)
		printf("%c",ch);
}
//在a, b 纵坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawRow(COORD a, COORD b, char ch)
{
	if(a.Y == b.Y)
		drawRow(a.Y, a.X, b.X, ch);
	else
	{
		SetPos(0, 25);
		printf("error code 01：无法填充行，因为两个坐标的纵坐标(x)不相等");
		system("pause");
	}
}

//把第x列，[y1, y2] 之间的坐标填充为 ch
void drawCol(int x, int y1, int y2, char ch)
{
	int y=y1;
	while(y<y2+1)
	{
		SetPos(x, y);printf("%c",ch);
		y++;
	}
}
//在a, b 横坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawCol(COORD a, COORD b, char ch)
{
	if(a.X == b.X)
		drawCol(a.X, a.Y, b.Y, ch);
	else
	{
		SetPos(0, 25);
		printf("error code 02：无法填充列，因为两个坐标的横坐标(y)不相等");
		system("pause");
	}
}

//左上角坐标、右下角坐标、用row填充行、用col填充列
void drawFrame(COORD a, COORD  b, char row, char col)
{
	drawRow(a.Y, a.X+1, b.X-1, row);
	drawRow(b.Y, a.X+1, b.X-1, row);
	drawCol(a.X, a.Y+1, b.Y-1, col);
	drawCol(b.X, a.Y+1, b.Y-1, col);
}

void drawFrame(int x1, int y1, int x2, int y2, char row, char col)
{
	COORD a={x1, y1};
	COORD b={x2, y2};
	drawFrame(a, b, row, col);
}

//部分清空
void clearch(int x1, int y1, int x2, int y2)
{
	int x,y;
	for(y=y1;y<=y2;++y)
	{
		SetPos(x1,y);
		for(x=x1;x<=x2;++x)
			printf(" ");
	}
}

int control()//控制函数，返回上下左右、回车、ESC
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

void drawmaze(Maze* M,int x,int y)//绘制迷宫
{
	int m,n,i,j;
	m=M->m;
	n=M->n;
	for(i=0;i<m;i++)
	{
		SetPos(x,y+i);
		for(j=0;j<n;j++)
		{
			if(M->M[i][j]==1)printf("■");
			else if(M->M[i][j]==0)printf("□");
			else if(M->M[i][j]==3)printf("●");
			else if(M->M[i][j]==2)printf("※");	
		}
	}
}

void drawmenu(int j,int i)//菜单边框//start j=3,i=20
{
	//mainmenu
	system("cls");
	drawRow(j, 0, 79, '-');drawRow(j+2, 0, 79, '-');
	SetPos(15, j+1);printf("方向键 上 和 下 选择， 回车 确定， ESC 结束/返回");
	drawRow(i, 0, 79, '-');drawRow(i+2, 0, 79, '-');
	SetPos(24, i+1);printf("制作:    边子涛    bztup@qq.com");
}

Maze* MazeM()//返回指向默认迷宫矩阵的指针
{
	Maze *M=(Maze *)malloc(LEN);
	int i,j,N=13;
	M->M =(int **)malloc(N*sizeof(int *));
	for(i=0;i<N;++i)
		M->M[i]=(int *)malloc(N*sizeof(int));
	int maze[13][13]={{1,1,1,1,1,1,1,1,1,1,1,1,1},
					{1,0,0,0,0,0,1,0,1,0,0,0,1},
					{1,0,1,1,1,0,0,0,1,0,1,0,1},
					{1,0,1,0,1,1,0,1,1,1,0,0,1},
					{1,0,1,0,1,0,0,0,0,0,0,1,1},
					{1,0,1,0,1,1,1,1,1,0,1,1,1},
					{1,0,0,0,0,0,3,0,1,0,1,1,1},
					{1,1,1,0,1,0,0,0,1,0,0,0,1},
					{1,0,0,0,1,0,0,0,1,1,1,0,1},
					{1,0,1,1,1,0,1,1,1,0,1,0,1},
					{1,0,0,0,1,0,0,0,1,0,1,0,1},
					{1,0,1,0,1,0,1,0,0,0,1,2,1},
					{1,1,1,1,1,1,1,1,1,1,1,1,1}};
	for(i=0;i<N;++i)
		for(j=0;j<N;++j)
			M->M[i][j]=maze[i][j];
	M->m=N;M->n=N;
	return M;
}

//读取/写入文件
void writeindex(int n,char**index)//将目录文件信息写入文件
{
	FILE*f;
	if((f=fopen("index.txt", "w"))==NULL)//打开文件w
		{printf("cannot open file\n");return;}
	fprintf(f,"%3d\n",n);//写入
	for(int i=0;i<n;++i)
		fprintf(f,"%s\n",index[i]);//写入目录文件信息
	fclose(f);//close
}
char** readindex(int &n)//提取目录文件信息
{
	FILE*f;
	char **index,*fname="默认迷宫",*name;
	index=(char **)malloc(100*sizeof(char*));
	if((f=fopen("index.txt", "r"))==NULL)//打开文件r
	{
		printf("cannot open file\n");
		index[0]=fname;
		writeindex(1,index);//写入默认迷宫名
		return 0;
	}
	fscanf(f,"%3d\n",&n);
	for(int i=0;i<n;++i)
	{
		name=(char *)malloc(15*sizeof(char));
		fscanf(f,"%s\n",&name[0]);
		index[i]=(char *)malloc((strlen(name))*sizeof(char));
		strcpy(index[i],&name[0]);
		free(name);
	}
	fclose(f);//close
	return index;
}
Maze *readM(char*fname)//提取迷宫矩阵
{
	int i,j;
	FILE*f;strcat(fname,".txt");
	if((f=fopen(fname, "r"))==NULL)//打开文件r
		{printf("cannot open file\n");return 0;}
	Maze *M=(Maze *)malloc(LEN);
	fscanf(f,"%3d%3d\n",&M->m,&M->n);
	M->M=(int **)malloc(M->m*sizeof(int *));
	for(i=0;i<M->m;++i)
	{
		M->M[i]=(int *)malloc(M->n*sizeof(int));
		for(j=0;j<M->n;++j)
			fscanf(f,"%2d",&M->M[i][j]);//提取迷宫矩阵
	}
	fclose(f);//close
	return M;
}
void writeM(char*fname,Maze* M)//将迷宫写入文件
{
	FILE*f;strcat(fname,".txt");
	if((f=fopen(fname, "w"))==NULL)//打开文件w
		{printf("cannot open file\n");return;}
	fprintf(f,"%3d%3d\n",M->m,M->n);
	int i,j;
	for(i=0;i<M->m;++i)
		for(j=0;j<M->n;++j)
			fprintf(f,"%2d",M->M[i][j]);//写入迷宫
	fclose(f);//close
}

void drawselect()
{
	drawmenu(1,20);
	drawFrame(0, 4, 15, 19, '=', '|');
	drawFrame(16, 4, 44, 19, '=', '|');
	drawFrame(45, 3, 79, 20, '-', '|');
	SetPos(5, 5);printf("选  项");
	SetPos(28, 5);printf("列  表");
}

int select(int n,char**index)//选择界面,返回选择
{
	drawselect();
	//选项
	SetPos(4, 7);printf("确认选择");
	SetPos(4, 9);printf("返    回");
	//列表
	for(int i=0;i<n;++i)
	{SetPos(21, 7+i);printf("%s",index[i]);}
	int k=7,h=2,u=7,x,last;last=u+n-1;
	Maze *M;int mx,my;
	M=MazeM();
	mx=56-M->n/2;my=11-M->m/2;
	drawmaze(M,mx,my);//reprint 选择>> 迷宫预览
	SetPos(h, k);printf(">>");	
	while(x=control())//	control();
	{
		switch (x)
		{
		case 1 :
			if( h == 2)
			{if( k == 9)
				{	
					SetPos(h, k);printf("　");k = 7;
					SetPos(h, k);printf(">>");
				}
			}
			else
			{if( u != 7)
				{
					SetPos(h, u);printf("　");	u = u-1;
					SetPos(h, u);printf(">>");
					if(u-7==0) M=MazeM();
					else M=readM(index[u-7]);
					mx=56-M->n/2;my=11-M->m/2;
					clearch(46, 4, 78, 19);drawmaze(M,mx,my);
				}
			}

				break;
		case 2 :
			if( h == 2)
			{if( k == 7 )
				{	
					SetPos(h, k);printf("　");	k = 9;
					SetPos(h, k);printf(">>");
				}
			}
			else
			{if( u != last)
				{
					SetPos(h, u);printf("　");	u = u+1;
					SetPos(h, u);printf(">>");
					M=readM(index[u-7]);
					mx=56-M->n/2;my=11-M->m/2;
					clearch(46, 4, 78, 19);drawmaze(M,mx,my);
				}
			}	
				break;
		case 3 :if( h == 18 )
				{
					SetPos(h, u);printf("　");	h = 2;
					SetPos(h, k);printf(">>");
				}
				break;
		case 4 :if( h == 2 )
				{
					SetPos(h, k);printf("　");	h = 18;
					SetPos(h, u);printf(">>");
				}
				break;
		case 6 :if (k == 7&&h==2)	return u-7;
				else if(h==18)	break; 
				else return -1;
		case 5 :return -1;break;
		}	
	}
	return 0;
}

Maze* selectmaze()//从已有选择迷宫并传回迷宫
{
	//选择、读取迷宫矩阵
	Maze *M;
	int x,n=0;
	char *fname,**index;
	index=readindex(n);
	x=select(n,index);//选择界面
	if(x==-1) return 0;
	else if(x==0) M=MazeM();//默认迷宫、迷宫存档
	else {fname=index[x];M=readM(fname);}
	return M;
}

Maze* set(Maze* M)//设定界面,返回新建、修改后矩阵
{
	drawmenu(1,20);
	int x,y;
	//设定界面(方向键选择>>空格变地形、左数字行选项(删除/新建行)、上数字列选项(删除/新建列)>>标记起点A、标记终点B回车确认)
	//SetPos(x,y);
	drawmaze(M,x,y);
	return 0;
}

int drawset(Maze* M)//设定界面,返回选择
{
	drawmenu(1,20);
	int x,y;
	//设定界面(方向键选择>>空格变地形、左数字行选项(删除/新建行)、上数字列选项(删除/新建列)>>标记起点A、标记终点B回车确认)
	//SetPos(x,y);
	drawmaze(M,x,y);
	return 0;
}

int setmaze()//设定、修改迷宫
{
	//选择新建OR修改
	//选择、读取迷宫矩阵
	Maze *M;
	int x,n=0;
	char *fname,**index;
	index=readindex(n);
	x=select(n,index);
	if(x==0) M=MazeM();
	else {fname=index[x];M=readM(fname);}
	//选项
	//(新建地图>>回车>>设定界面)
	//(地图选项>>回车选中>>确认修改？>>回车>>设定界面)
	//默认迷宫、迷宫存档
	//reprint 选择>> 迷宫预览
	//control();//上下左右选择，回车确认
	//return M;
	return 0;
}

void drawPlaying()//游戏界面
{
	system("cls");
	drawFrame(0, 0, 48, 24, '=', '|');//
	drawFrame(49, 0, 79, 4, '-', '|');//
	drawFrame(49, 4, 79, 9, '-', '|');//
	drawFrame(49, 9, 79, 20, '-', '|');//
	drawFrame(49, 20, 79, 24, '-', '|');//
}

void drawmode(Maze *M)
{
	drawmenu(1,20);
	drawFrame(0, 4, 15, 19, '=', '|');
	drawFrame(16, 4, 44, 19, '=', '|');
	drawFrame(45, 3, 79, 20, '-', '|');
	int mx=56-M->n/2,my=11-M->m/2;
	drawmaze(M,mx,my);
}

void print(int i)
{
	if(i==7)
	{
		SetPos(24, 11);printf("　　　　　　　　　");
		SetPos(21, 9);printf("计时模式：");
		SetPos(24, 11);printf("在限时内完成游戏  ");
	}
	else if(i==9)
	{
		SetPos(24, 11);printf("　　　　　　　　　");
		SetPos(21, 9);printf("自由模式：");
		SetPos(24, 11);printf("不限时完成游戏    ");
	}
	else clearch(17, 5, 43, 18);
}


int selectmode(Maze*M)
{
	drawmode(M);
	SetPos(4, 5);printf("选择模式");
	SetPos(4, 7);printf("计时模式");
	SetPos(4, 9);printf("自由模式");
	SetPos(4, 11);printf("返    回");
	int j=7,x;
	print(j);
	SetPos(2, j);printf(">>");	
	while(x=control())
	{
		switch (x)
		{
		case 1 :	
				if( j != 7)
				{	
					SetPos(2, j);printf("　");j = j-2;
					SetPos(2, j);printf(">>");
					print(j);
				}
				break;
		case 2 :	
				if( j != 11 )
				{	
					SetPos(2, j);printf("　");	j = j+2;
					SetPos(2, j);printf(">>");
					print(j);
				}
				break;
		case 6 :if (j == 7)	return 1;else if(j==9) return 2;
		case 5 :return 0;break;
		}	
	}
	return 0;
}

/*
int ptime(int p)
{
	int t0=clock()/1000+p,t;
		t=clock()/1000-t0;
	return t;
}
*/
int timemode(Maze* M)//-1失败-2成功
{
	drawPlaying();
	SetPos(54, 2);printf("剩余时间：    s");
	int mx=18-M->n/2,my=12-M->m/2;
	drawmaze(M,mx,my);
	int x,i,j;
	for(i=0;i<M->m;++i)
	{
		for(j=0;j<M->n;++j)
			if(M->M[i][j]==3) break;
		if(M->M[i][j]==3) break;
	}
	int t0=clock(),t,t1=0,p=1;
	while(1)
	{
		if(p)//计时
		{
			t=clock()-t0;t1=30-t/1000;
			if(t1<=0)
			{printf("时间到");return -1;}
			if(t%1000==0)
			{SetPos(64, 2);printf("%3d",t1);}
		}
		if( kbhit() )
		{
			if(x=control())
			{
				switch (x)
				{
				case 32:
					if(p)
					{
						p=0;
						SetPos(54, 2);printf("    暂停中     ");
						continue;
					}
					else 
					{
						SetPos(54, 2);printf("               ");
						SetPos(54, 2);printf("剩余时间：    s");
						SetPos(64, 2);printf("%3d",t1);
						p=1;t0=clock()-t;
					}
					break;
				case 1 :
					if(M->M[i-1][j]==0&&p)
					{
						M->M[i][j]=0;i--;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i-1][j]==2&&p)
					{M->M[i][j]=0;i--;drawmaze(M,mx,my);}
					break;	
				case 2 :
					if(M->M[i+1][j]==0&&p)
					{
						M->M[i][j]=0;i++;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i+1][j]==2&&p)
					{M->M[i][j]=0;i++;drawmaze(M,mx,my);}
					break;
				case 3 :
					if(M->M[i][j-1]==0&&p)
					{
						M->M[i][j]=0;j--;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i][j-1]==2&&p)
					{M->M[i][j]=0;j--;drawmaze(M,mx,my);}
					break;
				case 4 :
					if(M->M[i][j+1]==0&&p)
					{
						M->M[i][j]=0;j++;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i][j+1]==2&&p)
					{M->M[i][j]=0;j++;drawmaze(M,mx,my);}
					break;
				//case 6 :break;
				case 5 :return 0;break;
				}
				if(M->M[i][j]==2) return -2;
			}
			else return 0;
		}
	}
	return 0;
}
int freemode(Maze *M)
{
	drawPlaying();
	SetPos(54, 2);printf("游戏中");
	int mx=18-M->n/2,my=12-M->m/2;
	drawmaze(M,mx,my);
	int x,i,j;
	for(i=0;i<M->m;++i)
	{
		for(j=0;j<M->n;++j)
			if(M->M[i][j]==3) break;
		if(M->M[i][j]==3) break;
	}
	while(1)
	{
		if( kbhit() )
		{
			if(x=control())
			{
				switch (x)
				{
				case 1 :
					if(M->M[i-1][j]==0)
					{
						M->M[i][j]=0;i--;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i-1][j]==2)
					{M->M[i][j]=0;i--;drawmaze(M,mx,my);}
					break;	
				case 2 :
					if(M->M[i+1][j]==0)
					{
						M->M[i][j]=0;i++;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i+1][j]==2)
					{M->M[i][j]=0;i++;drawmaze(M,mx,my);}
					break;
				case 3 :
					if(M->M[i][j-1]==0)
					{
						M->M[i][j]=0;j--;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i][j-1]==2)
					{M->M[i][j]=0;j--;drawmaze(M,mx,my);}
					break;
				case 4 :
					if(M->M[i][j+1]==0)
					{
						M->M[i][j]=0;j++;
						M->M[i][j]=3;
						drawmaze(M,mx,my);
					}
					else if(M->M[i][j+1]==2)
					{M->M[i][j]=0;j++;drawmaze(M,mx,my);}
					break;
				//case 6 :break;
				case 5 :return 0;break;
				}
				if(M->M[i][j]==2) return -2;
			}
			else return 0;
		}
	}
	return 0;
}

int failure()
{
	clearch(1, 1, 47, 23);
	SetPos(16, 12);printf("失败:-(");
	int x;
	while(1)
	{if((x=control())==5||x==6) return 0;}
}
int success()
{
	clearch(1, 1, 47, 23);
	SetPos(20, 12);printf("成功!!");
	int x;
	while(1)
	{if((x=control())==5||x==6) return 0;}
}
int game()
{
	Maze *M;
	char a,s=0;
	int x,y;x=30;y=5;
	if((M=selectmaze())==0) return 0;//选择地图
	if((a=selectmode(M))==0) return 0;//mode//选择模式（自由、计时）
	else if(a==1) s=timemode(M);
	else if(a==2) s=freemode(M);
	if(s==0) return 0;
	else if(s==-1) failure();
	else if(s==-2) success();
	return 0;
}

int** load(Maze*M)//全部路径、最优路径（广度优先遍历）,返回最优路径
{
	return 0;
}

int mainmenu()
{
	drawmenu(3,20);
	SetPos(30, 1);printf("M A Z E     G A M E");
	SetPos(15, 11);printf("开始游戏");
	SetPos(15, 13);printf("设置地图");
	SetPos(42, 11);printf("开始游戏：");
	SetPos(46, 13);printf("选择地图并开始游戏。");
	int j=11,x;
	SetPos(12, j);printf(">>");	
	while(x=control())
	{
		switch (x)
		{
		case 1 :	
				if( j == 13)
				{	
					SetPos(12, j);printf("　");j = 11;
					SetPos(12, j);printf(">>");
					SetPos(51, 13);printf("　　　　　　　　　　　　  ");
					SetPos(42, 11);printf("开始游戏：                ");
					SetPos(46, 13);printf("选择地图并开始游戏。      ");
				}
				break;
		case 2 :	
				if( j == 11 )
				{	
					SetPos(12, j);printf("　");	j = 13;
					SetPos(12, j);printf(">>");
					SetPos(51, 13);printf("　　　　　　　　　　　　　");
					SetPos(42, 11);printf("设置地图：                ");
					SetPos(46, 13);printf("建立新地图或更改地图存档。");
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
	Maze *M;
	printf("start_test\n");//游戏准备
	HideCursor(0);	//隐藏光标
	int x,t;
	while(1)
	{
		if((x=mainmenu())==0) 
		{system("cls");break;}
		else if(x==1) 
		{
			system("cls");printf("开始游戏");
			game();
		} 
		else 
		{printf("设置地图");}//setmaze();
		printf("end_test\n");
	}
}
