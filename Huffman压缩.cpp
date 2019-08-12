#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 257 //2^8+1
typedef struct
{//树节点 元素：权重 父节点 左子节点 右子节点
	unsigned int weight;//权重 无符号整形
	unsigned int parent,lchild,rchild;//父节点 左子节点 右子节点 无符号整形
}HTnode,*Huffmantree;//声明指针*Huffmantree
typedef char* *Huffmancode;//二级指针

void weight(int *w,char *name)
{
	FILE *fp;
	int i,j,q=0;
	for(i=0;i<MAX-1;++i) w[i]=0;//初始化
	if((fp=fopen(name, "rb"))==NULL)//尝试打开文件
		{printf("cannot open file\n");return;}
	for(i=0;!feof(fp)&&fread(&q,1,1,fp);++i)//逐个读取
	{
		*(w+q)=*(w+q)+1;//计数
		if(i>=100000000)
			for(j=0,i=1;j<MAX-1;++j){w[j]=(w[j]+1)/2;i=i+w[j];}
	}
	fclose(fp);
}//计算权重

void select(Huffmantree HT,int n,int *s1,int *s2)
{
	int i,t=100000000;
	for(i=1;i<=n;i++)
		if(HT[i].parent==0&&HT[i].weight<t)
		{t=HT[i].weight;*s1=i;}
	t=100000000;
	for(i=1;i<=n;i++)
		if(HT[i].parent==0&&HT[i].weight<t&&i!=*s1)
		{t=HT[i].weight;*s2=i;}
}//选两个最小的

Huffmantree Huffmantreeing(Huffmantree HT,int *w)
{
	int m,i,p1,q2,*s1,*s2;
	Huffmantree p;
	s1=&p1; s2=&q2;
	m=2*MAX-1; 
	HT=(Huffmantree)malloc((m+1)*sizeof(HTnode));
	for(p=HT+1,i=1;i<=MAX;++i,++p,++w)//赋初始值
	{p->weight=*w;p->parent=0;p->lchild=0;p->rchild=0;}
	for(;i<=m;++i,++p)//赋初始值
	{p->weight=0;p->parent=0;p->lchild=0;p->rchild=0;}
	for(i=MAX+1;i<=m;++i)//链接树的节点
	{
		select(HT,i-1,s1,s2);
		HT[p1].parent=i;HT[q2].parent=i;
		HT[i].lchild=p1;HT[i].rchild=q2;
		HT[i].weight=HT[p1].weight+HT[q2].weight;
	}
	return HT;//节点指针，root为HT[m]
}//huffmantree

Huffmancode Huffmancoding(Huffmantree HT)
{
	Huffmancode HC;
	char *cd;int i,f,start,c;
	HC=(Huffmancode)malloc((MAX+1)*sizeof(char*));
	cd=(char*)malloc(MAX*sizeof(char));cd[MAX-1]='\0';
	for(i=1;i<=MAX;++i)
	{
		start=MAX-1;
		for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent)
			if(HT[f].lchild==c) cd[--start]='0';
			else cd[--start]='1';//逆序遍历得huffmancode
		HC[i-1]=(char*)malloc((MAX-start)*sizeof(char));
		strcpy(HC[i-1],&cd[start]);
	}
	free(cd);
	return HC;
}//得到huffmancode

void infotohuff(Huffmancode HC,char *name_in,char*name_bin,int &in,int &out)
{
	FILE *fp,*fh;
	int p=0,i,j=0,m=0,k=0,h=0;
	if((fp=fopen(name_in, "rb"))==NULL)
		{printf("cannot open file\n");return;}
	if((fh=fopen(name_bin, "wb"))==NULL)
		{printf("cannot open file\n");return;}
	for(i=0;!feof(fp)&&fread(&p,1,1,fp);++i)
	{
		for(j=0;HC[p][j]!='\0';j++)
		{
			h=h*2+(HC[p][j]-'0');m++;//每8个二进制数合一个字节
			if(m==8){fwrite(&h,1,1,fh);h=0;++k;m=0;}
		}
	}
	in=i;//压缩前字节数
	for(j=0;HC[MAX-1][j]!='\0';j++)//添加终止符
	{
		h=h*2+(HC[MAX-1][j]-'0');m++;
		if(m==8){fwrite(&h,1,1,fh);h=0;++k;m=0;	}
	}
	if(m!=0)//补零
	{
		for(i=1;i<=8-m;++i)h=h*2;
		fwrite(&h,1,1,fh);h=0;++k;m=0;
	}
	out=k;//压缩后字节数
	fclose(fp);fclose(fh);
}//压缩

void hufftoinfo(Huffmantree HT,char*name_bin,char *name_out)
{
	FILE *fp,*fh;
	int f=0,p=0,i,j=0,m=0,k=0,h=0,buff[8],buff_h[MAX];;
	if((fh=fopen(name_bin, "rb"))==NULL)
		{printf("cannot open file\n");return;}
	if((fp=fopen(name_out, "wb"))==NULL)
		{printf("cannot open file\n");return;}
	for(i=0;!feof(fh)&&fread(&p,1,1,fh);++i)
	{
		for(j=1;j<=8;++j){buff[8-j]=p%2;p=p/2;}//还原二进制数
		for(j=m;j<m+8;++j)buff_h[j]=buff[j-m];m=m+8;//m-buff_h的缓冲数
		do{
			for(f=2*MAX-1,h=0;f>MAX&&h<m;++h)//沿huffmantree查找
				if(buff_h[h]==1) f=HT[f].rchild;
				else f=HT[f].lchild;
			if(f<=MAX)
			{//若读到终止符关闭文件并结束此子函数，否则写入还原的字节到文件
				if(f==MAX) {fclose(fh);fclose(fp);return;}
				f=f-1;fwrite(&f,1,1,fp);
				for(j=h;j<m;++j) buff_h[j-h]=buff_h[j];m=m-h;
			}else break;//未找到原ASCII值
		}while(f<MAX);
	}
	fclose(fh);fclose(fp);
}//解压

void main()
{
	Huffmantree HT=NULL;
	Huffmancode HC=NULL;//huffmancode
	int len_in=0,len_out=0,w[MAX];
	char name_in[95],*name_bin="info.bin",name_out[100]="out_";
	printf("输入目标文件名:");scanf("%s",&name_in);
	strcat(name_out,name_in);
	printf("开始计算huffmancode\n");
	weight(w,name_in);//计算权重
	w[MAX-1]=1;//终止符
	HT=Huffmantreeing(HT,w);//root为HT[2n-1]
	HC=Huffmancoding(HT);//计算Huffman码
	printf("开始压缩\n");
	infotohuff(HC,name_in,name_bin,len_in,len_out);//压缩
	printf("压缩前：%d字节\n压缩后：%d字节\n",len_in,len_out);
	printf("压缩率：%d%%\n",len_out*100/len_in);printf("压缩完成\n");
	printf("按回车继续。。。");
	fflush(stdin);getchar();printf("开始解压\n");
	hufftoinfo(HT,name_bin,name_out);//解压
	printf("还原到文件：%s\n",name_out);
	printf("解压完成\n");
	printf("end\n");
	getchar();
}
