#include<iostream>
#include<cstring>
#include<typeinfo>
#include<cstdio>
#include<iomanip>
using namespace std;
 
int soldier_HP[5];
char soldier_name[5][20] = { "iceman", "lion", "wolf", "ninja", "dragon" };
char weapon_name[3][20] = { "sword", "bomb", "arrow" };
int soldier_force[5];
int city_number, time_limited;
int R;
 
//武器类
class weapon
{
public:
	int num, force;//数量， 攻击力
	weapon() :num(0), force(0) {}
};
 
class csoldier
{
public:
	int HP, force, id, type;
	weapon sweapon[3];
	csoldier() {}
	virtual ~csoldier() {}
 
 
	friend class battlefield;
};
 
 
class dragon :public csoldier
{
public:
	friend class battlefield;
	double morale;
	//constructor
	dragon(int n, int color, double m) :csoldier()
	{
		type = 4;
		morale = m;
		printf("Its morale is %.2f\n", morale);
		force = soldier_force[4];
		HP = soldier_HP[4];
		id = n;
		if (n % 3 == 0)
		{
		    if(force / 5 > 0)
            {
                sweapon[0].force = force / 5;
                sweapon[0].num = 1;
            }
		}
		else if (n % 3 == 1)
		{
			sweapon[n % 3].force = 1e9;
			sweapon[n % 3].num = 1;
		}
		else
		{
			sweapon[n % 3].force = R;
			sweapon[n % 3].num = 3;
		}
	}
	//destructor
	~dragon() {}
};
 
class ninja :public csoldier
{
public:
	friend class battlefield;
	ninja(int n, int color) :csoldier()
	{
		type = 3;
		force = soldier_force[3];
		HP = soldier_HP[3];
		id = n;
		if (n % 3 == 0)
		{
		    if(force / 5 > 0)
            {
                sweapon[0].force = force / 5;
                sweapon[0].num = 1;
            }
			sweapon[1].force = 1e9;
			sweapon[1].num = 1;
		}
		else if (n % 3 == 1)
		{
			sweapon[1].force = 1e9;
			sweapon[1].num = 1;
			sweapon[2].force = R;
			sweapon[2].num = 3;
		}
		else
		{
			sweapon[n % 3].force = R;
			sweapon[n % 3].num = 3;
		    if(force / 5 > 0)
            {
                sweapon[0].force = force / 5;
                sweapon[0].num = 1;
            }
		}
	}
	~ninja() {}
};
 
class iceman :public csoldier
{
private:
	friend class battlefield;
	iceman(int n, int color) :csoldier()
	{
		type = 0;
		force = soldier_force[0];
		HP = soldier_HP[0];
		id = n;
		if (n % 3 == 0)
		{
		    if(force / 5 > 0)
            {
                sweapon[0].force = force / 5;
                sweapon[0].num = 1;
            }
		}
		else if (n % 3 == 1)
		{
			sweapon[n % 3].force = 1e9;
			sweapon[n % 3].num = 1;
		}
		else
		{
			sweapon[n % 3].force = R;
			sweapon[n % 3].num = 3;
		}
	}
	~iceman() {}
};
 
class lion :public csoldier
{
private:
	friend class battlefield;
	int loyalty;
	lion(int n, int color, int hp) :csoldier(), loyalty(hp)
	{
		type = 1;
		printf("Its loyalty is %d\n", loyalty);
		force = soldier_force[1];
		HP = soldier_HP[1];
		id = n;
	}
public:
	static int K;
};
int lion::K = 0;
 
class wolf :public csoldier
{
public:
	wolf(int n, int color) :csoldier()
	{
		type = 2;
		force = soldier_force[2];
		HP = soldier_HP[2];
		id = n;
	}
 
private:
	friend class battlefield;
};
 
 
class battlefield
{
private:
	int hour, minute, soldier_total[2], cur[2];
	int HP[2];
	char name[2][10];
	csoldier ***head;
	int soldier[5];
	int *flag, *last_winner, *city_hp;
	bool enemy_appear[2];
public:
	const int arr[2][5] = { { 0, 1, 2, 3, 4 } ,{ 1, 4, 3, 0, 2 } };
 
 
	battlefield(int n) :HP{ n, n }, hour(0), minute(0), name{ "red", "blue" }, enemy_appear{ false, false }
	{
		soldier_total[0] = soldier_total[1] = 0;
		cur[0] = cur[1] = 0;
		memset(soldier, 0, sizeof(soldier));
		flag = new int[city_number + 2];
		last_winner = new int[city_number + 2];
		city_hp = new int[city_number + 2];
		memset(city_hp, 0, sizeof(int) * (city_number + 2));
		memset(last_winner, -1, sizeof(int) * (city_number + 2));
		memset(flag, -1, sizeof(int) * (city_number + 2));
		head = new csoldier**[city_number + 2];
		for (int i = 0; i < city_number + 2; ++i)
		{
			head[i] = new csoldier*[2];
			memset(head[i], NULL, sizeof(csoldier *) * 2);
		}
	}
 
 
	~battlefield()
	{
		delete[]flag;
		delete[]last_winner;
		delete[]city_hp;
		for (int i = 0; i < city_number + 2; ++i)
		{
			delete[]head[i];
		}
		delete[]head;
	}
 
	//输出当前时间
	void out_time()
	{
		cout << setfill('0') << setw(3) << hour << ':' << setfill('0') << setw(2) << minute;
		return;
	}
 
	//1) 武士降生
	//base produce soldiers. if base produce soldier sucessfully, return true;if else return false;
	bool produce(int color)
	{
		int t = arr[color][cur[color]];
		if (HP[color] >= soldier_HP[t])
		{
			HP[color] -= soldier_HP[t];
			soldier_total[color]++;
			out_time();
			cout << ' ' << name[color] << ' ' << soldier_name[t] << ' ' << soldier_total[color] << " born" << endl;
			int pos = ((color == 0) ? 0 : city_number + 1);
			switch (t)
			{
			case 0: head[pos][color] = new iceman(soldier_total[color], color); break;
			case 1: head[pos][color] = new lion(soldier_total[color], color, HP[color]); break;
			case 2: head[pos][color] = new wolf(soldier_total[color], color); break;
			case 3: head[pos][color] = new ninja(soldier_total[color], color); break;
			case 4: head[pos][color] = new dragon(soldier_total[color], color, (1.0 * HP[color] / soldier_HP[t])); break;
			}
			cur[color] = (cur[color] + 1) % 5;
			return true;
		}
		else
			return false;
	}
 
	void out_reach(csoldier *s, int city, int color)
	{
 
		if (city != 0 && city != city_number + 1)
			{
			    out_time();
			    printf(" %s %s %d marched to city %d with %d elements and force %d\n", name[color], soldier_name[s->type], s->id, city, s->HP, s->force);
			}
		else if((city == 0 && m[0]) || (city == city_number + 1 && m[1]))
			{
			    out_time();
			    printf(" %s %s %d reached %s headquarter with %d elements and force %d\n", name[color], soldier_name[s->type], s->id, name[1 ^ color], s->HP, s->force);
			    if((base[0] && city == 0) || (base[1] && city == city_number + 1))
                {
                    out_time();
                    printf(" %s headquarter was taken\n", name[((city == 0)? 0 : 1)]);
                }
			}
	}
 
	//3)武士前进到某一城市
	bool m[2], ans;
	bool base[2];
	bool march()
	{
 
		ans = false;
		base[0] = base[1] = false;
		m[0] = m[1] = false;
		if (head[city_number][0] != NULL)
		{
			if (enemy_appear[1] == false)
				{
				    enemy_appear[1] = true;
				    m[1] = true;
				}
			else
			{
			    m[1] = true;
				base[1] = true;
				ans = true;
			}
		}
		for (int i = city_number; i >= 0; --i)
		{
			if (i == city_number && head[i][0] == NULL)
				continue;
			if (head[i][0] != NULL && typeid(*head[i][0]) == typeid(iceman) && i % 2 == 1)
			{
				head[i][0]->force += 20;
				if (head[i][0]->HP > 9)
					head[i][0]->HP -= 9;
				else
					head[i][0]->HP = 1;
			}
			head[i + 1][0] = head[i][0];
		}
 
		if (head[1][1] != NULL)
		{
			if (enemy_appear[0] == false)
				{
				    enemy_appear[0] = true;
				    m[0] = true;
				}
			else
			{
			    m[0] = true;
				base[0] = true;
				ans = true;
			}
		}
		for (int i = 0; i <= city_number; ++i)
		{
			if (i == 0 && head[i + 1][1] == NULL)
				continue;
			if (head[i + 1][1] != NULL && typeid(*head[i + 1][1]) == typeid(iceman) && (city_number - i) % 2 == 1)
			{
				head[i + 1][1]->force += 20;
				if (head[i + 1][1]->HP > 9)
					head[i + 1][1]->HP -= 9;
				else
					head[i + 1][1]->HP = 1;
			}
			head[i][1] = head[i + 1][1];
		}
		head[0][0] = head[city_number + 1][1] = NULL;
		for (int i = 0; i <= city_number + 1; ++i)
		{
			if (head[i][0] != NULL)
				out_reach(head[i][0], i, 0);
			if (head[i][1] != NULL)
				out_reach(head[i][1], i, 1);
		}
		return !ans;
	}
 
	//2) lion逃跑
	void run_away()
	{
		for (int i = 0; i <= city_number + 1; ++i)
		{
			for (int t = 0; t < 2; ++t)
			{
				if ((i == 0 && t == 1) || (i == city_number + 1 && t == 0))
					continue;
				if (head[i][t] != NULL && typeid(*head[i][t]) == typeid(lion))
				{
					auto p = (lion *)head[i][t];
					if (p->loyalty <= 0)
					{
						out_time();
						cout << ' ' << name[t];
						cout << " lion " << head[i][t]->id << " ran away" << endl;
						delete head[i][t];
						head[i][t] = NULL;
						continue;
					}
				}
			}
 
		}
		return;
	}
 
	//20分钟时，每个城市产生10个生命元
	void produce_hp()
	{
		for (int i = 1; i <= city_number; ++i)
			city_hp[i] += 10;
		return;
	}
 
	//30分钟时， 武士取走每个城市的生命元
	void fetch_hp()
	{
		for (int i = 1; i <= city_number; ++i)
		{
			if (head[i][0] != NULL && head[i][1] == NULL && city_hp[i] > 0)
			{
				HP[0] += city_hp[i];
				out_time();
				printf(" %s %s %d earned %d elements for his headquarter\n", name[0], soldier_name[head[i][0]->type], head[i][0]->id, city_hp[i]);
				city_hp[i] = 0;
			}
			else if (head[i][1] != NULL && head[i][0] == NULL && city_hp[i] > 0)
			{
				HP[1] += city_hp[i];
				out_time();
				printf(" %s %s %d earned %d elements for his headquarter\n", name[1], soldier_name[head[i][1]->type], head[i][1]->id, city_hp[i]);
				city_hp[i] = 0;
			}
		}
		return;
	}
 
	//弓箭手准备放箭！
	void out_shoot(csoldier *p, csoldier*q, int color)
	{
		if (q->HP <= R)
		{
			q->HP = 0;
			p->sweapon[2].num--;
			out_time();
			printf(" %s %s %d shot and killed %s %s %d\n", name[color], soldier_name[p->type], p->id, name[1 ^ color], soldier_name[q->type], q->id);
		}
		else
		{
			q->HP -= R;
			p->sweapon[2].num--;
			out_time();
			printf(" %s %s %d shot\n", name[color], soldier_name[p->type], p->id);
		}
	}
	void shoot_arrow()
	{
		for (int i = 1; i <= city_number; ++i)
		{
			if (head[i][0] != NULL && head[i][0]->sweapon[2].num > 0 && head[i + 1][1] != NULL)
			{
				out_shoot(head[i][0], head[i + 1][1], 0);
			}
			if (head[i][1] != NULL && head[i][1]->sweapon[2].num > 0 && head[i - 1][0] != NULL)
			{
				out_shoot(head[i][1], head[i - 1][0], 1);
			}
		}
		return;
	}
 
	//评估是否使用bomb
	void use_bomb()
	{
		for (int i = 1; i <= city_number; ++i)
		{
			if (head[i][0] == NULL || head[i][1] == NULL || head[i][0]->HP <= 0 || head[i][1]->HP <= 0)
				continue;
			if (head[i][0]->sweapon[1].num == 0 && head[i][1]->sweapon[1].num == 0)
				continue;
			int color = ((flag[i] != -1) ? flag[i] : ((i % 2 == 1) ? 0 : 1));
			if (head[i][0]->sweapon[1].num != 0)
			{
				if (color == 0)
				{
					if (typeid(*head[i][1]) != typeid(ninja) && head[i][0]->sweapon[0].force + head[i][0]->force < head[i][1]->HP && head[i][1]->sweapon[0].force + head[i][1]->force / 2 >= head[i][0]->HP)
					{
						out_time();
						printf(" %s %s %d used a bomb and killed %s %s %d\n", name[0], soldier_name[head[i][0]->type], head[i][0]->id, name[1], soldier_name[head[i][1]->type], head[i][1]->id);
						delete head[i][0];
						delete head[i][1];
						head[i][0] = head[i][1] = NULL;
						continue;
					}
				}
				else
				{
					if (head[i][1]->sweapon[0].force + head[i][1]->force >= head[i][0]->HP)
					{
						out_time();
						printf(" %s %s %d used a bomb and killed %s %s %d\n", name[0], soldier_name[head[i][0]->type], head[i][0]->id, name[1], soldier_name[head[i][1]->type], head[i][1]->id);
						delete head[i][0];
						delete head[i][1];
						head[i][0] = head[i][1] = NULL;
						continue;
					}
				}
			}
			if (head[i][1]->sweapon[1].num != 0)
			{
				if (color == 1)
				{
					if (typeid(*head[i][0]) != typeid(ninja) && head[i][1]->sweapon[0].force + head[i][1]->force < head[i][0]->HP && head[i][0]->sweapon[0].force + head[i][0]->force / 2 >= head[i][1]->HP)
					{
						out_time();
						printf(" %s %s %d used a bomb and killed %s %s %d\n", name[1], soldier_name[head[i][1]->type], head[i][1]->id, name[0], soldier_name[head[i][0]->type], head[i][0]->id);
						delete head[i][0];
						delete head[i][1];
						head[i][0] = head[i][1] = NULL;
						continue;
					}
				}
				else
				{
					if (head[i][0]->sweapon[0].force + head[i][0]->force >= head[i][1]->HP)
					{
						out_time();
						printf(" %s %s %d used a bomb and killed %s %s %d\n", name[1], soldier_name[head[i][1]->type], head[i][1]->id, name[0], soldier_name[head[i][0]->type], head[i][0]->id);
						delete head[i][0];
						delete head[i][1];
						head[i][0] = head[i][1] = NULL;
						continue;
					}
				}
			}
 
		}
		return;
	}
 
	//loser died
	void wolf_snatch(csoldier *winner, csoldier *loser)
	{
		//wolf缴获武器
		if (typeid(*winner) == typeid(wolf))
		{
			for (int i = 0; i < 3; ++i)
			{
				if (winner->sweapon[i].num == 0)
					winner->sweapon[i] = loser->sweapon[i];
			}
		}
	}
 
	void dragon_cheer(csoldier *winner, int color, int city)
	{
		if (typeid(*winner) == typeid(dragon))
		{
			dragon *p = (dragon *)winner;
			p->morale += 0.2;
			if (p->morale > 0.8)
			{
				out_time();
				printf(" %s %s %d yelled in city %d\n", name[color], soldier_name[winner->type], winner->id, city);
			}
 
		}
		return;
	}
 
	void b_fight(csoldier* &att, csoldier* &rev, int color, int city)
	{
		out_time();
		int hurt = 0;
		printf(" %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", name[color], soldier_name[att->type], att->id, name[1 ^ color], soldier_name[rev->type], rev->id, city, att->HP, att->force);
		if (att->sweapon[0].num != 0)
		{
			hurt = att->sweapon[0].force + att->force;
			att->sweapon[0].force = att->sweapon[0].force * 4 / 5;
			if (att->sweapon[0].force == 0)
				att->sweapon[0].num = 0;
		}
		else
			hurt = att->force;
		if (hurt >= rev->HP)
		{
			if (typeid(*rev) == typeid(lion))
			{
				att->HP += rev->HP;
			}
 
			wolf_snatch(att, rev);
			out_time();
			printf(" %s %s %d was killed in city %d\n", name[1 ^ color], soldier_name[rev->type], rev->id, city);
			temp[city] = true;
            if (typeid(*att) == typeid(dragon))
			{
				dragon_cheer(att, color, city);
			}
			delete rev;
			rev = NULL;
		}
		else
		{
			rev->HP -= hurt;
			if (typeid(*rev) != typeid(ninja))
			{
				out_time();
				hurt = 0;
				printf(" %s %s %d fought back against %s %s %d in city %d\n", name[color ^ 1], soldier_name[rev->type], rev->id, name[color], soldier_name[att->type], att->id, city);
				if (rev->sweapon[0].num != 0)
				{
					hurt = rev->sweapon[0].force + rev->force / 2;
					rev->sweapon[0].force = rev->sweapon[0].force * 4 / 5;
					if (rev->sweapon[0].force == 0)
						rev->sweapon[0].num = 0;
				}
				else
					hurt = rev->force / 2;
				if (hurt >= att->HP)
				{
					if (typeid(*att) == typeid(lion))
					{
						rev->HP += att->HP;
					}
					if (typeid(*rev) == typeid(dragon))
					{
						((dragon *)rev)->morale += 0.2;
					}
					wolf_snatch(rev, att);
					out_time();
					printf(" %s %s %d was killed in city %d\n", name[color], soldier_name[att->type], att->id, city);
					temp[city] = true;
					delete att;
					att = NULL;
				}
				else
				{
					att->HP -= hurt;
					if (typeid(*rev) == typeid(dragon))
					{
						((dragon *)rev)->morale -= 0.2;
					}
					if (typeid(*att) == typeid(dragon))
					{
						((dragon *)att)->morale -= 0.2;
						if(((dragon *)att)->morale > 0.8)
                        {
                            out_time();
                            printf(" %s %s %d yelled in city %d\n", name[color], "dragon", att->id, city);
                        }
					}
					if (typeid(*att) == typeid(lion))
					{
						((lion *)att)->loyalty -= lion::K;
					}
					if (typeid(*rev) == typeid(lion))
					{
						((lion *)rev)->loyalty -= lion::K;
					}
					last_winner[city] = -1;
				}
			}
			else
			{
				if (typeid(*att) == typeid(dragon))
				{
					((dragon *)att)->morale -= 0.2;
                    if(((dragon *)att)->morale > 0.8)
                    {
                        out_time();
                        printf(" %s %s %d yelled in city %d\n", name[color], "dragon", att->id, city);
                    }
				}
				if (typeid(*att) == typeid(lion))
				{
					((lion *)att)->loyalty -= lion::K;
				}
				last_winner[city] = -1;
			}
		}
		return;
	}
 
	//战斗！
	bool *temp;
	void fight()
	{
		temp = new bool[city_number + 2];
		memset(temp, false, sizeof(bool) * (city_number + 2));
		for (int i = 1; i <= city_number; ++i)
		{
			int color = ((flag[i] != -1) ? flag[i] : ((i % 2 == 1) ? 0 : 1));
			csoldier* &r = head[i][0], * &b = head[i][1];
			if (r == NULL || b == NULL)
				continue;
			if (r->HP <= 0 && b->HP <= 0)
			{
				delete r;
				delete b;
				r = NULL;
				b = NULL;
				continue;
			}
			if (r->HP <= 0)
			{
				temp[i] = true;
				wolf_snatch(b, r);
				if (color == 1)
					dragon_cheer(b, 1, i);
				out_time();
				printf(" %s %s %d earned %d elements for his headquarter\n", name[1], soldier_name[head[i][1]->type], head[i][1]->id, city_hp[i]);
				delete r;
				r = NULL;
				if (last_winner[i] == 1 && flag[i] != 1)
				{
					out_time();
					printf(" blue flag raised in city %d\n", i);
					flag[i] = 1;
				}
				else
				{
					last_winner[i] = 1;
				}
				continue;
			}
			else if (b->HP <= 0)
			{
				temp[i] = true;
				wolf_snatch(r, b);
				if (color == 0)
					dragon_cheer(r, 0, i);
				out_time();
				printf(" %s %s %d earned %d elements for his headquarter\n", name[0], soldier_name[head[i][0]->type], head[i][0]->id, city_hp[i]);
				delete b;
				b = NULL;
				if (last_winner[i] == 0 && flag[i] != 0)
				{
					out_time();
					printf(" red flag raised in city %d\n", i);
					flag[i] = 0;
				}
				else
				{
					last_winner[i] = 0;
				}
				continue;
			}
			else
			{
				b_fight(head[i][color], head[i][1 ^ color], color, i);
 
				if (temp[i])
				{
 
					int w;
					if (head[i][0] != NULL)
						w = 0;
					else
						w = 1;
					out_time();
					printf(" %s %s %d earned %d elements for his headquarter\n", name[w], soldier_name[head[i][w]->type], head[i][w]->id, city_hp[i]);
					if (last_winner[i] == w && flag[i] != w)
					{
						out_time();
						printf(" %s flag raised in city %d\n", name[w], i);
						flag[i] = w;
					}
					else
					{
						last_winner[i] = w;
					}
				}
				continue;
			}
		}
		for (int i = 1; i <= city_number; ++i)
		{
			if (temp[i] && head[i][1] != NULL)
			{
				if (HP[1] >= 8)
				{
					HP[1] -= 8;
					head[i][1]->HP += 8;
				}
			}
		}
		for (int i = city_number; i >= 1; --i)
		{
			if (temp[i] && head[i][0] != NULL)
			{
				if (HP[0] >= 8)
				{
					HP[0] -= 8;
					head[i][0]->HP += 8;
				}
			}
		}
		for (int i = 1; i <= city_number; ++i)
		{
			if (temp[i])
			{
				if (head[i][0] != NULL)
				{
					HP[0] += city_hp[i];
					city_hp[i] = 0;
				}
				else
				{
					HP[1] += city_hp[i];
					city_hp[i] = 0;
				}
 
			}
		}
		delete temp;
	}
 
	//检查时间
	bool check_time()
	{
		if (hour * 60 + minute > time_limited)
			return false;
		else
			return true;
	}
 
	//基地报告
	void base_report()
	{
		out_time();
		printf(" %d elements in red headquarter\n", HP[0]);
		out_time();
		printf(" %d elements in blue headquarter\n", HP[1]);
		return;
	}
 
	void soldier_report()
	{
        for(int i = 0; i <= city_number + 1; ++i)
        {
            for(int t = 0; t < 2; ++t)
            {
                if(head[i][t] != NULL && head[i][t]->HP <= 0)
                {
                    delete head[i][t];
                    head[i][t] = NULL;
                }
            }
        }
        for (int t = 0; t < 2; ++t)
		for (int i = 0; i <= city_number + 1; ++i)
		{
				if (head[i][t] != NULL)
				{
					out_time();
					printf(" %s %s %d has ", name[t], soldier_name[head[i][t]->type], head[i][t]->id);
					bool f = true;
					if (head[i][t]->sweapon[2].num != 0)
					{
						printf("arrow(%d)", head[i][t]->sweapon[2].num);
						f = false;
					}
					if (head[i][t]->sweapon[1].num != 0)
					{
						if (!f)
							printf(",");
						printf("bomb");
						f = false;
					}
					if (head[i][t]->sweapon[0].num != 0)
					{
						if (!f)
							printf(",");
						printf("sword(%d)", head[i][t]->sweapon[0].force);
						f = false;
					}
					if (f)
						printf("no weapon");
					printf("\n");
				}
		}
		return;
	}
 
	bool run()
	{
		minute = 0;
		if (!check_time()) return false;
 
		produce(0);
		produce(1);
 
		minute = 5;
		if (!check_time()) return false;
 
		run_away();
 
		minute = 10;
		if (!check_time()) return false;
 
		if (!march())
			return false;
 
		minute = 20;
		if (!check_time()) return false;
		produce_hp();
 
		minute = 30;
		if (!check_time()) return false;
		fetch_hp();
 
		minute = 35;
		if (!check_time()) return false;
		shoot_arrow();
 
		minute = 38;
		if (!check_time()) return false;
		use_bomb();
 
		minute = 40;
		if (!check_time()) return false;
 
		fight();
 
		minute = 50;
		if (!check_time()) return false;
		base_report();
 
		minute = 55;
		if (!check_time()) return false;
		soldier_report();
 
		hour++;
		return true;
	}
 
};
 
int main()
{
	int n, hp, cnt = 0;
	cin >> n;
	while (n--)
	{
		cout << "Case " << ++cnt << ':' << endl;
		cin >> hp >> city_number >> R >> lion::K >> time_limited;
		battlefield Battle(hp);
		cin >> soldier_HP[4] >> soldier_HP[3] >> soldier_HP[0] >> soldier_HP[1] >> soldier_HP[2];
		cin >> soldier_force[4] >> soldier_force[3] >> soldier_force[0] >> soldier_force[1] >> soldier_force[2];
		while (Battle.run());
 
	}
	return 0;
}
