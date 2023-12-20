#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
class Soldier;//提前声明 
class Headquarter
{
public:
	int making_list[5];
	Headquarter(int _color,int _total_m) 
	{
		color=_color;
		total_soldier=0;
		total_m=_total_m;
		stop= false;
		curNum=0;
		memset(soldier_num,0,sizeof(soldier_num));
	}
	bool make(int _time);
	friend Soldier;
	
private:
	int color;  //0是红军 1是蓝军 
	int total_soldier;  //武士的总数
	int total_m; //生命值 
	bool stop; //是否停止制造武士
	int curNum;  //当前武士的编号 
	int soldier_num[5]; //各类武士的数量 
	Soldier* psoldier;  //当前的士兵 
	
};
class Soldier
{
	friend Headquarter;//声明友元 
public:
	Soldier(Headquarter* p,int _kind)
	{
	    pHeadquarter = p;
	    kind = _kind;
	}
	int num;//编号 
	static const char* Name_of_soldier[5];
	static int HP[5];//生命值 
	int AP;//攻击力 
	int kind;//种类的编号0 for dragon,1 for ninja, 2 for iceman, 3 for lion, 4 for wolf 
	Headquarter *pHeadquarter;//表明阵营
	void Print(int _time);  //输出每一行 
	
};

void Soldier::Print(int _time)
{
	char Color[10];
	if(pHeadquarter->color==0)
	    strcpy(Color,"red");
	else 
	    strcpy(Color,"blue");
	printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",_time,Color,Name_of_soldier[kind],
	pHeadquarter->total_soldier,HP[kind],pHeadquarter->soldier_num[kind],Name_of_soldier[kind],Color);
}
bool Headquarter::make(int _time)
{
	if(stop) return false;
	int index = curNum;
	int counter = 0;
	while(Soldier::HP[making_list[index]] > total_m && counter<5)
	{
		index =(index+1)%5;
		counter++;
	}
	if(counter == 5)
	{
		stop=true;
		if(color==0)
		{
			printf("%03d red headquarter stops making warriors\n",_time);
			
		}
		else 
		    printf("%03d blue headquarter stops making warriors\n",_time);
	return false;
	}
	total_m -= Soldier::HP[making_list[index]]; //司令部生命源减少 
	curNum = (index+1)%5;  
	total_soldier++;
	psoldier = new Soldier(this,making_list[index]);
	soldier_num[making_list[index]]++;  //表示某种武士数量增加 
	psoldier->Print(_time);  
	delete psoldier;
	return true;
}

const char* Soldier::Name_of_soldier[5]={"dragon","ninja","iceman","lion","wolf"};
int Soldier::HP[5];

int main()
{
	int type;//测试组数 
	int m;//生命值
	int no=1;
	cin>>type;
	while(type>0)
	{
		cin>>m;
		//输入每个武士生命值 
		Headquarter red(0,m);
		Headquarter blue(1,m);
		for(int i=0;i<5;i++)
		{
			cin>>Soldier::HP[i];
		}
		cout<<"Case: "<<no<<endl;
		no++;
		int time=0;
		//红方司令部按照iceman、lion、wolf、ninja、dragon的顺序循环制造武士
		//蓝方司令部按照lion、dragon、ninja、iceman、wolf的顺序循环制造武士 
		red.making_list[0]=2;
		red.making_list[1]=3;
		red.making_list[2]=4;
		red.making_list[3]=1;
		red.making_list[4]=0;
		blue.making_list[0]=3;
		blue.making_list[1]=0;
		blue.making_list[2]=1;
		blue.making_list[3]=2;
		blue.making_list[4]=4;
		while(1)
		{
			bool b1 =red.make(time);
			bool b2 = blue.make(time);
			if(b1==true && b2==false) break;
			time++;
		}
		type--;
	}
	return 0;
}
