#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
class Soldier;
class Dragon;
class Ninja;
class Iceman;
class Lion;
class Wolf;

class Headquarter
{
private:
	int color;
	int total_soldier; //��ʿ���� 
	int total_m; //������ֵ
	bool stop;
	int soldier_num[5];//���
	int curNum; //���ڲ�������ʿ��� 
	Soldier* psoldier;
	
public:
	Headquarter(int _color,int _total_m)
	{
		color=_color;
		total_m=_total_m;  
		stop = false;
		total_soldier=0;
		curNum = 0;
		memset(soldier_num,0,sizeof(soldier_num));//��������ʿ�ı�Ŷ�����Ϊ0 
	}
	int making_list[5];
	bool make(int _time);

	friend class Soldier;
	friend class Dragon;
	friend class Ninja;
	friend class Iceman;
	friend class Lion;
	friend class Wolf;
};
class Soldier
{
	friend class Headquarter;
protected:
	Headquarter *pheadquarter;
	int kind;  //0��dragon��1��ninja��2��iceman��3��lion��4��wolf 
public:
	Soldier(Headquarter*p, int m_kind)
	{
		pheadquarter = p;
		kind = m_kind;
	}
	static const char* Name_of_soldier[5]; //���� 
	static int HP[5]; //����ֵ 
	virtual void Print(int _time);
	
};
class Weapon
{
public:
	Weapon(int m_kind):kind(m_kind){}
	int kind;
	static const char*Name_of_Weapon[3];
	
};
class Dragon:public Soldier
{
private: 
    double morale;
    Weapon *pWeapon;
public:
	Dragon(Headquarter*p,int m_kind):Soldier(p,m_kind)
	{
		pWeapon = new Weapon(pheadquarter->total_soldier%3);//���Ϊ n%3 ������
		//˾�ʣ������Ԫ������������dragon���������Ԫ������
		morale = pheadquarter->total_m*1.0 / Soldier::HP[0];
	}
	
	virtual void Print(int _time)
	{
		Soldier::Print(_time);
		printf("It has a %s,and it's morale is %0.2f\n",Weapon::Name_of_Weapon[pWeapon->kind],morale);		
	}
	~Dragon()
	{
		delete pWeapon;
	}

	
};
class Ninja:public Soldier
{
private:
	Weapon*pWeapon1,*pWeapon2;
public:
	Ninja(Headquarter *p,int m_kind):Soldier(p,m_kind)
	{
		pWeapon1 = new Weapon(pheadquarter->total_soldier % 3);
		pWeapon2 = new Weapon((pheadquarter->total_soldier+1) % 3);
	}
	virtual void Print(int _time)
	{
		Soldier::Print(_time);
		cout<<"It has a "<<Weapon::Name_of_Weapon[pWeapon1->kind]<<" and a "<<Weapon::Name_of_Weapon[pWeapon2->kind]<<endl;
	}
	~Ninja()
	{
		delete pWeapon1;
		delete pWeapon2;
	}
};
class Iceman:public Soldier
{
private:
	Weapon *pWeapon;
public:
	Iceman(Headquarter *p,int m_kind):Soldier(p,m_kind)
	{
		pWeapon=new Weapon(pheadquarter->total_soldier % 3);
	}
	virtual void Print(int _time)
	{
		Soldier::Print(_time);
		cout<<"It has a "<<Weapon::Name_of_Weapon[pWeapon->kind]<<endl;
	}
	~Iceman()
	{
		delete pWeapon;
	}
};
class Lion:public Soldier
{
private:
	int loyalty;
public:
	Lion(Headquarter *p,int m_kind):Soldier(p,m_kind)
	{
		loyalty = pheadquarter->total_m;
	}
	virtual void Print(int _time)
	{
		Soldier::Print(_time);
		cout<<"It's loyalty is "<<loyalty<<endl;
	}
	~Lion(){}
	
};
class Wolf:public Soldier
{
public:
	Wolf(Headquarter *p,int m_kind):Soldier(p,m_kind){}
	virtual void Print(int _time)
	{
		Soldier::Print(_time);
	}
	~Wolf(){}
};
void Soldier::Print(int _time)
{
	char Color[10];
	if(pheadquarter->color==0)
	    strcpy(Color,"red");
	else 
	    strcpy(Color,"blue");
	printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",_time,Color,Name_of_soldier[kind],
	pheadquarter->total_soldier,HP[kind],pheadquarter->soldier_num[kind],Name_of_soldier[kind],Color);
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
	int k= making_list[index];
	switch (k) {
		case 0:
			psoldier = new Dragon(this,k);
			break;
		case 1:
			psoldier = new Ninja(this,k);
			break;
		case 2:
			psoldier = new Iceman(this,k);
			break;
		case 3:
			psoldier = new Lion(this,k);
			break;
		case 4:
			psoldier = new Wolf(this,k);
			break;			
	}
	soldier_num[making_list[index]]++;  //��ʾĳ����ʿ�������� 
	psoldier->Print(_time);  
	delete psoldier;
	return true;
}
const char* Soldier::Name_of_soldier[5]={"dragon","ninja","iceman","lion","wolf"};
const char* Weapon::Name_of_Weapon[3]={"sword","bomb","arrow"};
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
