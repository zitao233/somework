//基于链表的学生成绩管理系统
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#define LEN sizeof(struct xsnode)

struct xsnode
{
	int xh;
	char xm[15];
	int gs;
	int yy;
	int wl;
	float pj;
	int mc;
	struct xsnode *next; 
};

int n=0;

struct xsnode*hd()
{
	struct xsnode*head;
	head=(struct xsnode*)malloc(LEN);
	head->next=NULL;
	return head;
}//头指针

void fr(struct xsnode*head)
{
	struct xsnode*p;
	while(head->next!=NULL)
	{
		p=head->next;
		head->next=p->next;
		free(p);
	}
}//释放链表内存空间

void shuru(struct xsnode*p)
{
	printf("学号:");
	while(scanf("%d",&p->xh)!=1||p->xh>=10000)
	{fflush(stdin);printf("输入格式错误，请输入不超过4位整数:");}fflush(stdin);
	printf("姓名:");scanf("%s",p->xm);fflush(stdin);
	printf("高数成绩:");
	while(scanf("%d",&p->gs)!=1||p->gs>=1000)
	{fflush(stdin);printf("输入格式错误，请输入不超过3位整数:");}fflush(stdin);
	printf("英语成绩:");
	while(scanf("%d",&p->yy)!=1||p->yy>=1000)
	{fflush(stdin);printf("输入格式错误，请输入不超过3位整数:");}fflush(stdin);
	printf("物理成绩:");
	while(scanf("%d",&p->wl)!=1||p->wl>=1000)
	{fflush(stdin);printf("输入格式错误，请输入不超过3位整数:");}fflush(stdin);
	p->pj=(float)(p->gs+p->yy+p->wl)/3.0;
	p->mc=0;
}//输入数据

struct xsnode*chazhao(struct xsnode*head,int xh)
{
	struct xsnode*p=head->next;
	while(p!=NULL)
	{
		if(p->xh==xh)break;
		p=p->next;
	}
	return p;
}//返回指向目标学号节点的指针

void create(struct xsnode*head)
{
	int i;
	struct xsnode*p,*end=head;
	fr(head);
	printf("***************************************************\n");
	printf("*                     创建链表                    *\n");
	printf("***************************************************\n");
	printf("\n请输入学生个数：");
	while(scanf("%d",&n)!=1||n>=10000)
	{fflush(stdin);printf("输入格式错误，请输入不超过4位整数:");}fflush(stdin);
	for(i=1;i<=n;i++)
	{
		printf("第%d个学生（共%d个）：\n",i,n);
		p=(struct xsnode*)malloc(LEN);
		shuru(p);
		while(chazhao(head,p->xh)!=NULL)
		{printf("学号重复，请重新输入：\n");shuru(p);}
		end->next=p;
		end=p;
		end->next=NULL;
	}
	printf("***************************************************\n");
	printf("*                     录入完毕！                  *\n");
	printf("*                 请按任意键继续。。。            *\n");
	printf("***************************************************\n");
	getch();
}//建立链表，输入成绩。

void save(struct xsnode*head)
{
	if(head->next!=NULL)
	{
		struct xsnode*p=head->next;
		char filename[10];
		int i;
		FILE*fp;
		printf("***************************************************\n");
		printf("*                     保存文件                    *\n");
		printf("***************************************************\n");
		printf("输入文件名：");scanf("%s",filename);fflush(stdin);
		if((fp=fopen(filename,"w"))==NULL)
		{
			printf("***************************************************\n");
			printf("*                 无法打开此文件！                *\n");
			printf("*                 请按任意键继续。。。            *\n");
			printf("***************************************************\n");
			getch();return;
		}
		fprintf(fp,"%3d\n",n);
		for(i=0;i<n;i++)
		{
			fprintf(fp,"%4d%15s%6d%6d%6d%8.2f%6d\n",p->xh,p->xm,p->gs,p->yy,p->wl,p->pj,p->mc);
			p=p->next;
		}
		fclose(fp);
		printf("***************************************************\n");
		printf("*                     保存成功！                  *\n");
		printf("*                 请按任意键继续。。。            *\n");
		printf("***************************************************\n");
		getch();
	}
	else
	{
		printf("***************************************************\n");
		printf("*                   当前链表为空                  *\n");
		printf("*                请先录入或读取文件               *\n");
		printf("*                  请按任意键继续。。。           *\n");
		printf("***************************************************\n");
		getch();
	}
}//保存文件

void read(struct xsnode*head)
{
	int i;
	char filename[10];
	struct xsnode*p,*end=head;
	FILE*fp;
	fr(head);
	printf("***************************************************\n");
	printf("*                     读取文件                    *\n");
	printf("***************************************************\n");
	printf("输入文件名：");scanf("%s",filename);fflush(stdin);
	if((fp=fopen(filename,"r"))==NULL)
	{
		printf("***************************************************\n");
		printf("*                 无法打开此文件！                *\n");
		printf("*                 请按任意键继续。。。            *\n");
		printf("***************************************************\n");
		getch();return;
	}
	fscanf(fp,"%3d\n",&n);
	for(i=0;i<n;i++)
	{
		p=(struct xsnode*)malloc(LEN);
		fscanf(fp,"%4d%15s%6d%6d%6d%f%d\n",&p->xh,p->xm,&p->gs,&p->yy,&p->wl,&p->pj,&p->mc);
		end->next=p;
		end=p;
		end->next=NULL;
	}
	fclose(fp);
	printf("***************************************************\n");
	printf("*                    读取成功！                   *\n");
	printf("*        可在主菜单选择显示数据等其他操作         *\n");
	printf("*                 请按任意键继续。。。            *\n");
	printf("***************************************************\n");
	getch();
}//读取文件。

void print(struct xsnode*head)
{
	struct xsnode*p=head->next;
	if(p!=NULL)
	{
		printf("***************************************************\n");
		printf("学号          姓名   高数  英语  物理  平均分  名次\n");
		printf("***************************************************\n");
		while(p!=NULL)
		{
			printf("%4d%15s%6d%6d%6d%8.2f%6d\n",p->xh,p->xm,p->gs,p->yy,p->wl,p->pj,p->mc);
			p=p->next;
		}
		printf("***************************************************\n");
		printf("*           显示完成，请按任意键继续。。。        *\n");
		printf("***************************************************\n");
		getch();
	}
	else
	{
		printf("***************************************************\n");
		printf("*                   当前链表为空                  *\n");
		printf("*                请先录入或读取文件               *\n");
		printf("*                  请按任意键继续。。。           *\n");
		printf("***************************************************\n");
		getch();
	}
}//数据显示


void swap(struct xsnode*p,struct xsnode*q)
{
	struct xsnode*u=(struct xsnode*)malloc(LEN);
	u->xh=p->xh;strcpy(u->xm,p->xm);
	u->gs=p->gs;u->yy=p->yy;u->wl=p->wl;
	u->pj=p->pj;u->mc=p->mc;

	p->xh=q->xh;strcpy(p->xm,q->xm);
	p->gs=q->gs;p->yy=q->yy;p->wl=q->wl;
	p->pj=q->pj;p->mc=q->mc;

	q->xh=u->xh;strcpy(q->xm,u->xm);
	q->gs=u->gs;q->yy=u->yy;q->wl=u->wl;
	q->pj=u->pj;q->mc=u->mc;
	free(u);
}//交换两节点中的数据

struct xsnode*GetPartion(struct xsnode* phead,struct xsnode* pend)  
{  
    float pj=phead->pj;  
    struct xsnode* p=phead;  
    struct xsnode* q=p->next;  
    while(q!=pend)  
    {
        if(q->pj>pj)  
        {
            p=p->next;  
            swap(p,q);  
        }
        q=q->next;  
    }  
    swap(p,phead);
    return p;  
}//快速排序的kay位置（降序）

void quicksort(struct xsnode*phead,struct xsnode*pend)
{

	if(phead!=pend)
	{
		struct xsnode*partion=GetPartion(phead,pend);
		quicksort(phead,partion);
		if(partion->next!=pend)
			quicksort(partion->next,pend);
	}
}//用于链表的快速排序

void px(struct xsnode*head)
{
	struct xsnode*p=head->next;
	int i=1;
	quicksort(p,NULL);
	if(p!=NULL)
	{
		printf("***************************************************\n");
		printf("*                     名次排序                    *\n");
		printf("***************************************************\n");
		printf("学号          姓名   高数  英语  物理  平均分  名次\n");
		printf("***************************************************\n");
		while(p!=NULL)
		{
			p->mc=i;
			printf("%4d%15s%6d%6d%6d%8.2f%6d\n",p->xh,p->xm,p->gs,p->yy,p->wl,p->pj,p->mc);
			p=p->next;
			i++;
		}
		printf("***************************************************\n");
		printf("*                 按名次排序完成                  *\n");
		printf("*                 请按任意键继续。。。            *\n");
		printf("***************************************************\n");
		getch();
	}
	else
	{
		printf("***************************************************\n");
		printf("*                   当前链表为空                  *\n");
		printf("*                请先录入或读取文件               *\n");
		printf("*                  请按任意键继续。。。           *\n");
		printf("***************************************************\n");
		getch();
	}
}//名次排序

void cz(struct xsnode*head)
{
	struct xsnode*p=head->next;
	int xh=0;
	if(p!=NULL)
	{
		printf("***************************************************\n");
		printf("*                     数据查找                    *\n");
		printf("***************************************************\n");
		printf("请输入要查询学生的学号：");
		while(scanf("%d",&xh)!=1||xh>=10000)
		{fflush(stdin);printf("输入格式错误，请输入不超过4位整数:");}fflush(stdin);
		p=chazhao(head,xh);
		if(p==NULL)
		{
			printf("***************************************************\n");
			printf("*           查找失败，未找到此学号的学生          *\n");
			printf("*                请按任意键继续。。。             *\n");
			printf("***************************************************\n");
			getch();
		}
		else
		{
			printf("***************************************************\n");
			printf("学号          姓名   高数  英语  物理  平均分  名次\n");
			printf("***************************************************\n");
			printf("%4d%15s%6d%6d%6d%8.2f%6d\n",p->xh,p->xm,p->gs,p->yy,p->wl,p->pj,p->mc);
			printf("***************************************************\n");
			printf("*            查找完成，请按任意键继续。。。       *\n");
			printf("***************************************************\n");
			getch();
		}
	}
	else
	{
		printf("***************************************************\n");
		printf("*                   当前链表为空                  *\n");
		printf("*                请先录入或读取文件               *\n");
		printf("*                  请按任意键继续。。。           *\n");
		printf("***************************************************\n");
		getch();
	}
}//数据查找

void insert(struct xsnode*head)
{
	int m=0,i;
	struct xsnode*p=(struct xsnode*)malloc(LEN);
	printf("***************************************************\n");
	printf("*                 增加数个学生数据                *\n");
	printf("***************************************************\n");
	printf("\n请输入增加学生个数：");
	while(scanf("%d",&m)!=1||m>=10000-n)
	{fflush(stdin);printf("输入格式错误，请输入不超过%d的整数:",9999-n);}fflush(stdin);
	for(i=1;i<=m;i++)
	{
		printf("第%d个学生（共%d个）：\n",i,m);
		p=(struct xsnode*)malloc(LEN);
		shuru(p);
		while(chazhao(head,p->xh)!=NULL)
		{printf("学号重复，请重新输入：\n");shuru(p);}
		p->next=head->next;
		head->next=p;
	}
	n=n+m;
	printf("***************************************************\n");
	printf("*                    添加成功！                   *\n");
	printf("*                请按任意键继续。。。             *\n");
	printf("***************************************************\n");
	getch();
}//增加节点

void Delete(struct xsnode*head)
{
	struct xsnode*p=head->next;
	int xh=0;
	printf("***************************************************\n");
	printf("*                删除一个学生数据                 *\n");
	printf("***************************************************\n");
	printf("请输入要删除学生的学号：");
	while(scanf("%d",&xh)!=1||xh>=10000)
	{fflush(stdin);printf("输入格式错误，请输入不超过4位整数:");}fflush(stdin);
	while(p!=NULL)
	{
		if(p->xh==xh)break;
		head=p;
		p=p->next;
	}
	if(p==NULL)
	{
		printf("***************************************************\n");
		printf("*               未找到此学号的学生                *\n");
		printf("*                请按任意键继续。。。             *\n");
		printf("***************************************************\n");
		getch();
	}
	else
	{
		head->next=p->next;
		free(p);
		n--;
		printf("***************************************************\n");
		printf("*                    删除成功！                   *\n");
		printf("*                请按任意键继续。。。             *\n");
		printf("***************************************************\n");
		getch();
	}
}//删除节点

void dat_xg(struct xsnode*head)
{
	struct xsnode*p;
	int xh=0;
	printf("***************************************************\n");
	printf("*                 修改一个学生数据                *\n");
	printf("***************************************************\n");
	printf("请输入要修改学生的学号：");
	while(scanf("%d",&xh)!=1||xh>=10000)
	{fflush(stdin);printf("输入格式错误，请输入不超过4位整数:");}fflush(stdin);
	p=chazhao(head,xh);
	if(p==NULL)
		{
			printf("***************************************************\n");
			printf("*               未找到此学号的学生                *\n");
			printf("*                请按任意键继续。。。             *\n");
			printf("***************************************************\n");
			getch();
		}
		else
		{
			system("cls");
			printf("***************************************************\n");
			printf("学号          姓名   高数  英语  物理  平均分  名次\n");
			printf("***************************************************\n");
			printf("%4d%15s%6d%6d%6d%8.2f%6d\n",p->xh,p->xm,p->gs,p->yy,p->wl,p->pj,p->mc);
			printf("***************************************************\n");
			shuru(p);
			while(chazhao(chazhao(head,p->xh),p->xh)!=NULL)
			{printf("学号重复，请重新输入：\n");shuru(p);}
			printf("名次：");
			while(scanf("%d",&p->mc)!=1||p->mc>=10000)
			{fflush(stdin);printf("输入格式错误，请输入不超过4位整数！\n");}fflush(stdin);
			printf("***************************************************\n");
			printf("*                      修改完成                   *\n");
			printf("***************************************************\n");
			printf("学号          姓名   高数  英语  物理  平均分  名次\n");
			printf("***************************************************\n");
			printf("%4d%15s%6d%6d%6d%8.2f%6d\n",p->xh,p->xm,p->gs,p->yy,p->wl,p->pj,p->mc);
			printf("***************************************************\n");
			printf("*                 请按任意键继续。。。            *\n");
			printf("***************************************************\n");
			getch();
		}
}//节点数据修改

void xg(struct xsnode*head)
{
	struct xsnode*p=head->next;
	if(p!=NULL)
	{
		int xz=9;
		while(xz!=0)
		{
			system("cls");
			printf("***************************************************\n");
			printf("*                     数据修改                    *\n");
			printf("***************************************************\n");
			printf("*               1—增加数个学生数据               *\n");
			printf("*               2—删除一个学生数据               *\n");
			printf("*               3—修改一个学生数据               *\n");
			printf("*               0—返回上一级菜单                 *\n");
			printf("***************************************************\n");
			printf("*******************   请选择  *********************\n");
			xz=getch()-48;
			system("cls");
			switch(xz)
			{
			case 1:insert(head);break;
			case 2:Delete(head);break;
			case 3:dat_xg(head);break;
			case 0:break;
			}
		}
	}
	else
	{
		printf("***************************************************\n");
		printf("*                   当前链表为空                  *\n");
		printf("*                请先录入或读取文件               *\n");
		printf("*                  请按任意键继续。。。           *\n");
		printf("***************************************************\n");
		getch();
	}
}//数据修改

void menu()
{
	system("cls");
	printf("***************************************************\n");
	printf("*           基于链表的学生成绩管理系统            *\n");
	printf("***************************************************\n");
	printf("*        计算B152   边子涛   201509014226         *\n");
	printf("***************************************************\n");
	printf("*                  1—创建链表                    *\n");
	printf("*                  2—保存文件                    *\n");
	printf("*                  3—读取文件                    *\n");
	printf("*                  4—数据显示                    *\n");
	printf("*                  5—名次排序                    *\n");
	printf("*                  6—数据查找                    *\n");
	printf("*                  7—数据修改                    *\n");
	printf("*                  0—退出系统                    *\n");
	printf("***************************************************\n");
	printf("*******************   请选择  *********************\n");
}//主菜单

void main()
{
	struct xsnode*head;
	int xz=9;
	head=hd();
	while(xz!=0)
	{
		menu();
		xz=getch()-48;
		system("cls");
		switch(xz)
		{
			case 1:create(head);break;
			case 2:save(head);break;
			case 3:read(head);break;
			case 4:print(head);break;
			case 5:px(head);break;
			case 6:cz(head);break;
			case 7:xg(head);break;
			case 0:break;
		}
	}
	fr(head);
	free(head);
}
