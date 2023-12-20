#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
class Soldier;//��ǰ���� 
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
	int color;  //0�Ǻ�� 1������ 
	int total_soldier;  //��ʿ������
	int total_m; //����ֵ 
	bool stop; //�Ƿ�ֹͣ������ʿ
	int curNum;  //��ǰ��ʿ�ı�� 
	int soldier_num[5]; //������ʿ������ 
	Soldier* psoldier;  //��ǰ��ʿ�� 
	
};
class Soldier
{
	friend Headquarter;//������Ԫ 
public:
	Soldier(Headquarter* p,int _kind)
	{
	    pHeadquarter = p;
	    kind = _kind;
	}
	int num;//��� 
	static const char* Name_of_soldier[5];
	static int HP[5];//����ֵ 
	int AP;//������ 
	int kind;//����ı��0 for dragon,1 for ninja, 2 for iceman, 3 for lion, 4 for wolf 
	Headquarter *pHeadquarter;//������Ӫ
	void Print(int _time);  //���ÿһ�� 
	
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
	total_m -= Soldier::HP[making_list[index]]; //˾�����Դ���� 
	curNum = (index+1)%5;  
	total_soldier++;
	psoldier = new Soldier(this,making_list[index]);
	soldier_num[making_list[index]]++;  //��ʾĳ����ʿ�������� 
	psoldier->Print(_time);  
	delete psoldier;
	return true;
}

const char* Soldier::Name_of_soldier[5]={"dragon","ninja","iceman","lion","wolf"};
int Soldier::HP[5];

int main()
{
	int type;//�������� 
	int m;//����ֵ
	int no=1;
	cin>>type;
	while(type>0)
	{
		cin>>m;
		//����ÿ����ʿ����ֵ 
		Headquarter red(0,m);
		Headquarter blue(1,m);
		for(int i=0;i<5;i++)
		{
			cin>>Soldier::HP[i];
		}
		cout<<"Case: "<<no<<endl;
		no++;
		int time=0;
		//�췽˾�����iceman��lion��wolf��ninja��dragon��˳��ѭ��������ʿ
		//����˾�����lion��dragon��ninja��iceman��wolf��˳��ѭ��������ʿ 
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
