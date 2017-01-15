#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<string.h>
#include<time.h>
#define LEN sizeof(struct Maze)
#define LENL sizeof(struct Load)
#define LENP sizeof(struct position)

/*=============== all the structures ===============*/
typedef struct Maze
{
	int **M;
	int m;
	int n;
}Maze;

typedef struct position
{
	int i;
	int j;
	int p;
	position* next;
}position;
typedef struct Load
{
	int n;
	position* next[100];
}Load;
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

void print(Maze* M,int i,int j)
{
	if(M->M[i][j]==1)printf("■");
	else if(M->M[i][j]==0)printf("□");
	else if(M->M[i][j]==2)printf("※");//终点
	else if(M->M[i][j]==3)printf("●");
	else if(M->M[i][j]==4)printf("◇");//选中标记（路）
	else if(M->M[i][j]==5)printf("◆");//选中标记（墙）
	else if(M->M[i][j]==6)printf("↑");
	else if(M->M[i][j]==7)printf("↓");
	else if(M->M[i][j]==8)printf("←");
	else if(M->M[i][j]==9)printf("→");
}

void drawmaze(Maze* M)//绘制迷宫//定位
{
	int m,n,i,j;
	m=M->m;
	n=M->n;
	printf("\n");
	for(i=0;i<m;i++)
	{
		printf("\n");
		for(j=0;j<n;j++)
		{
			print(M,i,j);
		}
	}
	printf("\n");
}

void drawmaze(Maze* M,int x,int y)//绘制迷宫//定位
{
	int m,n,i,j;
	m=M->m;
	n=M->n;
	for(i=0;i<m;i++)
	{
		SetPos(x,y+i);
		for(j=0;j<n;j++)
		{
			print(M,i,j);
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
	if((f=fopen("index.txt", "r"))==NULL)//打开文件r
	{
		printf("cannot open file\n");
		index=(char **)malloc(sizeof(char*));
		index[0]=fname;n=1;
		writeindex(n,index);//写入默认迷宫名
		return index;
	}
	fscanf(f,"%3d\n",&n);
	index=(char **)malloc(n*sizeof(char*));
	for(int i=0;i<n;++i)
	{
		name=(char *)malloc(20*sizeof(char));
		fscanf(f,"%s\n",&name[0]);
		index[i]=(char *)malloc((strlen(name)+1)*sizeof(char));
		strcpy(index[i],name);
		free(name);
	}
	fclose(f);//close
	return index;
}
Maze *readM(char*fname)//提取迷宫矩阵
{
	int i,j;
	FILE*f;char*name=fname;
	fname=(char*)malloc((strlen(fname)+5)*sizeof(char));
	strcpy(fname,name);strcat(fname,".txt");
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
	FILE*f;char*name=fname;
	fname=(char*)malloc((strlen(fname)+5)*sizeof(char));
	strcpy(fname,name);strcat(fname,".txt");
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


int drawset(int n,char**index,int &c)//返回选择(新建修改删除)
{
	drawselect();
	//选项
	SetPos(4, 7);printf("修    改");
	SetPos(4, 9);printf("删    除");
	SetPos(4, 11);printf("新    建");
	SetPos(4, 13);printf("路    径");
	SetPos(4, 15);printf("返    回");		
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
			{if( k != 7)
				{	
					SetPos(h, k);printf("　");k = k-2;
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
			{if( k != 15 )
				{	
					SetPos(h, k);printf("　");	k = k+2;
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
		case 6 :if(k != 15&&h==2){c=u-7;return (k-5)/2;}
				else if(h==18)	break; 
				else return 0;
		case 5 :return 0;break;
		}	
	}
	return 0;
}

int right(Maze*A,Maze*B,int qi,int qj,int zi,int zj)//qiqj起点//深度搜索
{
	if(qi==zi&&qj==zj) return 1;
	B->M[zi][zj]=1;//drawmaze(B);
	int a,a1=0,a2=0,a3=0,a4=0;
	if(A->M[zi-1][zj]!=1&&B->M[zi-1][zj]==0) 
	{if((a1=right(A,B,qi,qj,zi-1,zj))) return 1;}
	if(A->M[zi+1][zj]!=1&&B->M[zi+1][zj]==0) 
	{if((a2=right(A,B,qi,qj,zi+1,zj))) return 1;}
	if(A->M[zi][zj-1]!=1&&B->M[zi][zj-1]==0) 
	{if((a3=right(A,B,qi,qj,zi,zj-1))) return 1;}
	if(A->M[zi][zj+1]!=1&&B->M[zi][zj+1]==0) 
	{if((a4=right(A,B,qi,qj,zi,zj+1))) return 1;}
	a=a1+a2+a3+a4;
	if(!a) return 0;
}


int yanzheng(Maze*M)
{
	int mi=0,mj=0,q=0,w=0;
	int qi,qj,zi,zj;
	for(mi=0;mi<M->m;++mi)
	{
		for(mj=0;mj<M->n;++mj)
			if(M->M[mi][mj]==3) break;
		if(M->M[mi][mj]==3) break;
	}
	if(mi<M->m) 
	{q=1;qi=mi;qj=mj;SetPos(54, 12);printf("                ");}
	else {SetPos(54, 12);printf("ERROR:  没有起点");}
	for(mi=0;mi<M->m;++mi)
	{
		for(mj=0;mj<M->n;++mj)
			if(M->M[mi][mj]==2) break;
		if(M->M[mi][mj]==2) break;
	}
	if(mi<M->m) {w=1;zi=mi;zj=mj;SetPos(54, 14);printf("                ");}
	else {SetPos(54, 14);printf("ERROR:  没有终点");}
	if(q==1&&w==1) 
	{
		Maze *B=(Maze *)malloc(LEN);
		int i,j;
		B->M =(int **)malloc(M->m*sizeof(int *));
		for(i=0;i<M->m;++i)
			B->M[i]=(int *)malloc(M->n*sizeof(int));
		for(i=0;i<M->m;++i)
			for(j=0;j<M->n;++j)
			{B->M[i][j]=0;}
		B->m=M->m;B->n=M->n;
		if(right(M,B,qi,qj,zi,zj))
			{SetPos(54, 16);printf("                ");return 1;}
		else {SetPos(54, 16);printf("ERROR:  没有路径");}
	}
	return 0;
}

int right2(Maze*A,Maze*B,int qi,int qj,int zi,int zj,int p,Load*L,position * P)//qiqj起点
{
	if(qi==zi&&qj==zj) 
	{
		L->next[L->n]=P;L->n=L->n+1;
		B->M[zi][zj]=0;
		//printf("<%d,%d>%d\n",zi,zj,p);
		return 1;
	}
	B->M[zi][zj]=p+5;//drawmaze(B);
	int n,n1=0,n2=0,n3=0,n4=0;
	if(A->M[zi-1][zj]!=1&&B->M[zi-1][zj]==0) 
	{
		position *Q1=(position *)malloc(LENP);Q1->next=NULL;
		n1=right2(A,B,qi,qj,zi-1,zj,2,L,Q1);
		if(n1) 
		{
			Q1->i=zi;Q1->j=zj;//坐标
			Q1->p=p;Q1->next=P;
			//printf("<%d,%d>%d\n",zi,zj,p);
		}
	}
	if(A->M[zi+1][zj]!=1&&B->M[zi+1][zj]==0) 
	{
		position *Q2=(position *)malloc(LENP);Q2->next=NULL;
		n2=right2(A,B,qi,qj,zi+1,zj,1,L,Q2);
		if(n2) 
		{
			Q2->i=zi;Q2->j=zj;//坐标
			Q2->p=p;Q2->next=P;
			//printf("<%d,%d>%d\n",zi,zj,p);
		}
	}
	if(A->M[zi][zj-1]!=1&&B->M[zi][zj-1]==0) 
	{
		position *Q3=(position *)malloc(LENP);Q3->next=NULL;
		n3=right2(A,B,qi,qj,zi,zj-1,4,L,Q3);
		if(n3) 
		{
			Q3->i=zi;Q3->j=zj;//坐标
			Q3->p=p;Q3->next=P;
			//printf("<%d,%d>%d\n",zi,zj,p);
		}
	}
	if(A->M[zi][zj+1]!=1&&B->M[zi][zj+1]==0) 
	{
		position *Q4=(position *)malloc(LENP);Q4->next=NULL;
		n4=right2(A,B,qi,qj,zi,zj+1,3,L,Q4);
		if(n4) 
		{
			Q4->i=zi;Q4->j=zj;//坐标
			Q4->p=p;Q4->next=P;
			//printf("<%d,%d>%d\n",zi,zj,p);
		}
	}
	n=n1+n2+n3+n4;
	if(n) {B->M[zi][zj]=0;return 1; }
	else{B->M[zi][zj]=0;free(P);return 0;}
}

void load(Maze*M)//全部路径（深度优先遍历）、最优路径
{
	system("cls");
	SetPos(0, 0);
	int mi=0,mj=0,q=0,w=0;
	int qi,qj,zi,zj;
	for(mi=0;mi<M->m;++mi)
	{
		for(mj=0;mj<M->n;++mj)
			if(M->M[mi][mj]==3) break;
		if(M->M[mi][mj]==3) break;
	}
	if(mi<M->m) {q=1;qi=mi;qj=mj;}
	else {printf("ERROR:  没有起点");}
	for(mi=0;mi<M->m;++mi)
	{
		for(mj=0;mj<M->n;++mj)
			if(M->M[mi][mj]==2) break;
		if(M->M[mi][mj]==2) break;
	}
	if(mi<M->m) {w=1;zi=mi;zj=mj;}
	else {printf("ERROR:  没有终点");}
	//
	Maze *B=(Maze *)malloc(LEN);
	int i,j;
	B->M =(int **)malloc(M->m*sizeof(int *));
	for(i=0;i<M->m ;++i)
		B->M[i]=(int *)malloc(M->n*sizeof(int));
	for(i=0;i<M->m ;++i)
		for(j=0;j<M->n ;++j)
		{B->M[i][j]=0;}
	B->m=M->m;B->n=M->n;
	//int qi=6,qj=6,zi=11,zj=11;
	Load*L=(Load *)malloc(LENL);L->n=0;
	if(right2(M,B,qi,qj,zi,zj,0,L,NULL))
	{
		//
		Maze **C=(Maze **)malloc(L->n*LEN);
		//
		printf("全部路径：\n");
		position *q;int *len=(int *)malloc(L->n*sizeof(int));
		for(int k=0;k<L->n;++k)
		{
			//
			C[k]=(Maze *)malloc(LEN);
			int i,j;
			C[k]->M =(int **)malloc(M->m*sizeof(int *));
			for(i=0;i<M->m ;++i)
				C[k]->M[i]=(int *)malloc(M->n*sizeof(int));
			for(i=0;i<M->m ;++i)
				for(j=0;j<M->n ;++j)
				{C[k]->M[i][j]=M->M[i][j];}
			C[k]->m=M->m;C[k]->n=M->n;
			//
			len[k]=0;
			printf("\n路径%d:",k+1);
			for(q=L->next[k] ;q->next!=NULL;q=q->next)
			{
				//
				C[k]->M[q->i][q->j]=q->p+5; 
				//
				printf("<%d,%d>",q->i,q->j);
				len[k]=len[k]+1;
			}
			printf("\n长度：%d\n",len[k]);
			drawmaze(C[k]);
		}
		printf("\n最优路径：\n");
		int l,n;
		for(k=0,l=len[0],n=0;k<L->n;++k)
		{
			if(len[k]<l) {l=len[k];n=k;}
		}
		printf("<%d,%d>",qi,qj);
		for(q=L->next[n] ;q->next!=NULL;q=q->next)
		{printf("<%d,%d>",q->i,q->j);}
		printf("\n长度：%d\n",l);
		drawmaze(C[n]);
	}
	else printf("没有路径\n");
	system("pause");
}

Maze* set(Maze*M)//设定界面
{
	system("cls");
	drawFrame(0, 0, 48, 24, '=', '|');
	drawFrame(49, 0, 79, 4, '-', '|');
	drawFrame(49, 4, 79, 9, '-', '|');
	drawFrame(49, 9, 79, 20, '-', '|');
	drawFrame(49, 20, 79, 24, '-', '|');
	SetPos(54, 12);printf("a加行z减行");
	SetPos(54, 14);printf("s加列x减列");
	SetPos(54, 16);printf("d设起点c设终点");
	SetPos(54, 18);printf("空格转化（墙/路）");
	int mx=18-M->n/2,my=12-M->m/2;int x,i=1,j=1,mi,mj;
	int a=M->M[i][j];M->M[i][j]=a+4;drawmaze(M,mx,my);
	while(1)
	{
		if(x=control())
		{
			switch (x)
			{
			case 32:
				if(a==0) 
				{a=1;M->M[i][j]=a+4;drawmaze(M,mx,my);}
				else if(a==1) 
				{a=0;M->M[i][j]=a+4;drawmaze(M,mx,my);}
				break;
			case 97://加行a
				if(M->m<=15)
				{
					M->m=M->m +1;
					M->M=(int**)realloc(M->M,M->m*sizeof(int));
					M->M[M->m-1]=(int*)malloc(M->n*sizeof(int));
					for(mj=0;mj<M->n;++mj) M->M[M->m-1][mj]=1;
					drawmaze(M,mx,my);
				}
				break;
			case 122://减行z
				if(M->m>5)
				{
					if(i==M->m-2)	--i;
					free(M->M[M->m-2]);
					M->M[M->m-2]= M->M[M->m-1];
					M->m =M->m-1;
					//printf("test\n");//
					//free(M->M[M->m]);
					clearch(1, 1, 47, 23);
					drawmaze(M,mx,my);
				}
				break;
			case 115://加列s
				if(M->n<=15)
				{
					M->n=M->n +1;
					for(mi=0;mi<M->m;++mi)
					{
						M->M[mi]=(int*)realloc(M->M[mi],M->n*sizeof(int));
						M->M[mi][M->n-1]=1;
					}
					drawmaze(M,mx,my);
				}
				break;
			case 120://减列x
				if(M->n>5)
				{
					if(i==M->n-2) --j;
					for(mi=0;mi<M->m;++mi)
					{
						M->M[mi][M->n-2]= M->M[mi][M->n-1];
						M->M[mi]=(int*)realloc(M->M[mi],(M->n-1)*sizeof(int));
					}			
					M->n =M->n -1;
					clearch(1, 1, 47, 23);
					drawmaze(M,mx,my);
				}
				break;
			case 100://设起点d
				for(mi=0;mi<M->m;++mi)
				{
					for(mj=0;mj<M->n;++mj)
						if(M->M[mi][mj]==3) break;
					if(M->M[mi][mj]==3) break;
				}
				if(mi<M->m) M->M[mi][mj]=0;
				a=3;M->M[i][j]=a+4;
				drawmaze(M,mx,my);
				break;
			case 99://设终点c
				for(mi=0;mi<M->m;++mi)
				{
					for(mj=0;mj<M->n;++mj)
						if(M->M[mi][mj]==2) break;
					if(M->M[mi][mj]==2) break;
				}
				if(mi<M->m) M->M[mi][mj]=0;
				a=2;M->M[i][j]=a+4;
				drawmaze(M,mx,my);
				break;
			case 1 :
				if((i-1)!=0)
				{
					M->M[i][j]=a;--i;
					a=M->M[i][j];M->M[i][j]=a+4;
					drawmaze(M,mx,my);
				}
				break;	
			case 2 :
				if((i+1)!=M->m-1)
				{
					M->M[i][j]=a;i++;
					a=M->M[i][j];M->M[i][j]=a+4;
					drawmaze(M,mx,my);
				}
				break;
			case 3 :
				if((j-1)!=0)
				{
					M->M[i][j]=a;j--;
					a=M->M[i][j];M->M[i][j]=a+4;
					drawmaze(M,mx,my);
				}
				break;
			case 4 :
				if((j+1)!=M->n-1)
				{
					M->M[i][j]=a;j++;
					a=M->M[i][j];M->M[i][j]=a+4;
					drawmaze(M,mx,my);
				}
				break;
			case 6 :
				M->M[i][j]=a;
				if(yanzheng(M)==1)
					return M;
				break;
			case 5 :return 0;break;
			}
		}
		else return 0;
	}
	return M;
}

int set(char* fname)//修改
{
	Maze*M;
	M=readM(fname);
	if((M=set(M))==0)return 0;
	writeM(fname,M);
	return 0;
}

int deletMaze(char*fname)//删
{
	char*name=fname;
	fname=(char*)malloc((strlen(fname)+5)*sizeof(char));
	strcpy(fname,name);strcat(fname,".txt");
	if(remove(fname)) return 0;
	else {return 1;}
	return 0;
}
char* addMaze()//返回迷宫名
{
	Maze*M;
	M=MazeM();
	if((M=set(M)) == 0) return 0;
	char *fname=(char*)malloc(20*sizeof(char));
	SetPos(54, 6);printf("fname:");HideCursor(1);
	scanf("%s",&fname[0]);
	HideCursor(0);
	writeM(fname,M);
	return fname;
}

int setmaze()//设定、修改迷宫
{
	Maze *M;
	int x,n=0,c;
	char *fname,**index,**list;
	index=readindex(n);
	x=drawset(n,index,c);//选择新建OR修改
	if(x==0) return 0;//返回
	else if(x!=3&&x!=4&&c==0) 
	{
		clearch(17, 5, 43, 18);
		SetPos(21, 7);printf("ERROR:");
		SetPos(21, 9);printf("默认迷宫无法修改、删除");
		while(1)
		{if((x=control())==5||x==6) return 0;}
		return 0;
	}
	else 
	{
		fname=index[c];//M=readM(fname);
	}
	//增删改
	if(x==1) 
	{
		if((set(fname))==0) return 0;//修改
	}
	else if(x==2) 
	{
		if(deletMaze(fname)==0) return 0;//删除
		else 
		{
			for(int i=c+1;i<n;++i)
			{
				index[i-1]=index[i];
				//printf("%s ",index[i-1]);
			}
			n=n-1;//free(index[n]);
			writeindex(n,index);
		}
	}
	else if(x==3) 
	{
		if((fname=addMaze())==0) return 0;
		n=n+1;index=(char**)realloc(index,n*sizeof(char*));	
		index[n-1]=fname;
		writeindex(n,index);
	}
	else if(x==4) 
	{
		if(!c) M=MazeM();
		else M=readM(fname);
		load(M);
	}
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

int timemode(Maze* M,int T)//-1失败-2成功
{
	drawPlaying();
	SetPos(56, 6);printf("计时模式,空格暂停");
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
	int t0=clock(),t,t1=0,t2=0,p=1;
	while(1)
	{
		if(p)//计时
		{
			t=clock()-t0;t1=T-t/1000;
			if(t1<=0)
			{SetPos(54, 2);printf("    时间到     ");return -1;}
			if(t1!=t2)
			{t2=t1;SetPos(64, 2);printf("%3d",t1);}
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
	if((M=selectmaze())==0) return 0;//选择地图
	if((a=selectmode(M))==0) return 0;//mode//选择模式（自由、计时）
	else if(a==1) s=timemode(M,30);
	else if(a==2) s=freemode(M);
	if(s==0) return 0;
	else if(s==-1) failure();
	else if(s==-2) success();
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
					SetPos(51, 13);printf("　　　　　　　　　　　　      ");
					SetPos(42, 11);printf("开始游戏：                ");
					SetPos(46, 13);printf("选择地图并开始游戏。          ");
				}
				break;
		case 2 :	
				if( j == 11 )
				{	
					SetPos(12, j);printf("　");	j = 13;
					SetPos(12, j);printf(">>");
					SetPos(51, 13);printf("　　　　　　　　　　　　　    ");
					SetPos(42, 11);printf("设置地图：                ");
					SetPos(46, 13);printf("修改/建立/删除地图、路径查找。");
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
	//Maze *M;
	printf("start_test\n");//游戏准备
	HideCursor(0);	//隐藏光标
	int x,t;
	while(1)
	{
		if((x=mainmenu())==0) 
		{system("cls");break;}
		else if(x==1) 
		{
			system("cls");//printf("开始游戏");
			game();
		} 
		else 
		{system("cls");printf("设置地图");setmaze();}//setmaze();
		printf("end_test\n");
	}
}
/*
int startMaze[9][9]={{1,1,1,1,1,1,1,1,1},
					{1,0,0,0,0,0,1,1,1},
					{1,0,1,1,1,0,0,0,1},
					{1,0,1,0,1,1,0,1,1},
					{1,0,1,0,-1,1,0,1,1},
					{1,0,1,0,1,1,0,1,1},
					{1,0,0,0,1,0,0,0,1},
					{1,0,1,0,1,0,1,2,1},
					{1,1,1,1,1,1,1,1,1}};
*/
