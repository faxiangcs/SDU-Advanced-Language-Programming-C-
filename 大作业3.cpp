#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<vector>
#include<algorithm>
#include<math.h>

#define SOLDIER_NUM 5 //武士种类
#define WEAPON_NUM 3 // 武器种类
//武士代号 
#define DRAGON 0
#define NINJA 1
#define ICEMAN 2
#define LION 3
#define WOLF 4
//红方蓝方司令部代号 
#define RED 0
#define BLUE 1
//武器代号 
#define SWORD 0
#define BOMB 1
#define ARROW 2

using namespace std;
//前置声明 
class Soldier;
class Dragon;
class Ninja;
class Iceman;
class Lion;
class Wolf;

typedef struct{
    Soldier* p1;//红方 
    Soldier* p2;//蓝方 
    int counter;
}City;

class MAP{
public:
    City City_list[25]; //定义城市数组 
    static int City_NUM;
    MAP(int _City_NUM){
        City_NUM = _City_NUM;
        Init(); //初始化界面 
    }
    void Init()
	{
        for(int i=0;i<25;i++){
            City_list[i].p1=NULL;
            City_list[i].p2=NULL;
            City_list[i].counter=0;
        }
    }
    int move(int _time);//0结束，1继续 
    void Attack(int hour);//红蓝武士PK
    void Lion_run(int hour);  //逃跑 
    void Print_Soldier(int hour); 
    void Wolf_rob(int hour); //抢武器 
    void clear(); //清理 
};

class Headquarter
{
    friend class Soldier;
    friend class Dragon;
    friend class Ninja;
    friend class Iceman;
    friend class Lion;
    friend class Wolf;
private:
    int total_soldier;//武士总数
    int color;// 红0蓝1 
    int life;//总生命值
    int curmaking;//当前制造的武士索引号
    int num[5];//各类武士的数量
    bool stop;
    Soldier* psoldier;//指向当前创造的武士
    int location;// 红色是0，中间n个城市，n+1蓝色 
public:
    int making_list[5];//武士制作顺序
    Headquarter(int m_color,int m_life)
	{
        total_soldier = 0;
        color = m_color;
        life = m_life;
        stop = false;
        curmaking = 0;
        if(color == RED) location = 0;
        else location = MAP::City_NUM + 1;
        memset(num,0,sizeof(num));
    }
    bool Make_soldier(int hour,MAP *pmap);//制造一个武士
    void Print_total_life(int hour){
        if(color == RED) printf("%03d:50 %d elements in red headquarter\n",hour,life);
        else printf("%03d:50 %d elements in blue headquarter\n",hour,life);
    }

};

class Weapon
{
public:
    int kind;
    int force;//武器攻击力
    int HP;//生命值 
    static const char* Name_of_weapon[WEAPON_NUM];
    static int HP_of_weapon[WEAPON_NUM];//武器耐久
    Weapon(int _kind):kind(_kind){
        HP = Weapon::HP_of_weapon[kind];
    }
};

bool mysort(const Weapon & A,const Weapon &B)
{//弓箭耐久低的排在前面
    if(A.kind != B.kind) return A.kind < B.kind;
    if(A.kind == ARROW){
        return A.HP < B.HP;
    }
    return true;
}

bool mysort_high(const Weapon & A,const Weapon &B)
{//耐久高的排前面
    if(A.kind != B.kind) return A.kind < B.kind;
    if(A.kind == ARROW){
        return A.HP > B.HP;
    }
    return true;
}

class Soldier
{
protected:
    Headquarter *pHeadquarter;//表明阵营
    int No;//编号
    int kind;//类型 0 for dragon,1 for ninja, 2 for iceman, 3 for lion, 4 for wolf
    bool alive;
    int location;//位置
    int life;//生命值 
    int Weapon_num;//拥有的武器数量
    int curWeapon_index;//当前使用的武器序号
    vector<Weapon> Weapon_list;
public:
    static const char* Name_of_soldier[SOLDIER_NUM];//字符串常量最好使用const char*，否则编译器会报warning
    static int Force_of_soldier[SOLDIER_NUM];
    static int HP_of_soldier[SOLDIER_NUM];
    Soldier(Headquarter* p,int _kind){
        pHeadquarter = p;
        kind = _kind;
        alive = true;
        location = pHeadquarter->location;
        No = pHeadquarter->total_soldier;
        life = Soldier::HP_of_soldier[kind];
    }
    virtual void Print(int hour);
    virtual void Printwin(Soldier *p, int hour,int i);
    int PK(Soldier *p,int hour,int i);//调用者先攻击 在第i个城市 return 0 for red win,1 for blue  win,2 for both alive, 3 for both died
    bool weapon_Zero();
    void attack(Soldier *p);//攻击一下
    void deleteWeapon();
    void sort_weapon();
    void sort_weapon_high();
    void get_weapon(Soldier *p);
    int get_weapon_force(int kind);
    void Print_weapon(int hour);
    bool isalive();
    int return_kind();
    int count_weapon(int kind);
    virtual int return_Lion_loyalty(){}; 
    virtual void yell(int hour,int i){};
    virtual void Lion_Incloyalty(){};
    virtual void Iceman_Inclife(){};
    virtual void rob_weapon(Soldier *p,int hour,int i){};
    friend class Headquarter;//声明友元
    friend class MAP;
    friend class Wolf;
};

class Dragon:public Soldier
{
private:
    //double fmorale;
public:
    Dragon(Headquarter *p,int _kind):Soldier(p,_kind){
        Weapon_list.push_back(Weapon(pHeadquarter->total_soldier % WEAPON_NUM));
        Weapon_num = 1;
        Weapon_list[0].force = get_weapon_force(Weapon_list[0].kind); 
    }
    virtual void Print(int hour){
        Soldier::Print(hour);
    }
    virtual void yell(int hour,int i);
};

class Ninja:public Soldier
{
public:
    Ninja(Headquarter *p,int _kind):Soldier(p,_kind){
        Weapon_list.push_back(Weapon(pHeadquarter->total_soldier % WEAPON_NUM));
        Weapon_list.push_back(((pHeadquarter->total_soldier +1) % WEAPON_NUM));
        Weapon_num = 2;
        Weapon_list[0].force = get_weapon_force(Weapon_list[0].kind);
        Weapon_list[1].force = get_weapon_force(Weapon_list[1].kind);
    }
    virtual void Print(int hour){
        Soldier::Print(hour);
    }
};


class Iceman:public Soldier
{
public:
    Iceman(Headquarter *p,int _kind):Soldier(p,_kind){
        Weapon_list.push_back(Weapon(pHeadquarter->total_soldier % WEAPON_NUM));
        Weapon_num = 1;
        Weapon_list[0].force = get_weapon_force(Weapon_list[0].kind);
    }
    virtual void Print(int hour){
        Soldier::Print(hour);
    }
    virtual void Iceman_Inclife(){
        life -= life / 10;
    }
};

class Lion:public Soldier
{
private:
    int Loyalty;
public:
    static int K;
    Lion(Headquarter *p,int _kind):Soldier(p,_kind){
        Loyalty = pHeadquarter->life;
        Weapon_list.push_back(Weapon(pHeadquarter->total_soldier % WEAPON_NUM));
        Weapon_num = 1;
        Weapon_list[0].force = get_weapon_force(Weapon_list[0].kind);
    }
    virtual void Print(int hour){
        Soldier::Print(hour);
        cout<<"Its loyalty is "<<Loyalty<<endl;
    }
    virtual int return_Lion_loyalty(){
        return Loyalty;
    }
    virtual void Lion_Incloyalty(){
        Loyalty = Loyalty-K;
    }
};

class Wolf:public Soldier
{
public:
    Wolf(Headquarter *p,int _kind):Soldier(p,_kind){
        Weapon_num = 0;
    }
    virtual void Print(int hour){
        Soldier::Print(hour);
    }
    virtual void rob_weapon(Soldier *p,int hour,int i);
};

void Soldier::Print(int hour)
{
    char Color[10];
    if(pHeadquarter->color == 0) strcpy(Color,"red");
    else strcpy(Color,"blue");
    printf("%03d:00 %s %s %d born\n",hour,Color,Name_of_soldier[kind],pHeadquarter->total_soldier);
}

int Soldier::get_weapon_force(int weapon_kind)
{
    if(weapon_kind == SWORD ) return Soldier::Force_of_soldier[kind] / 5;
    if(weapon_kind == BOMB) return Soldier::Force_of_soldier[kind] * 4 / 10;
    else return Soldier::Force_of_soldier[kind] * 3 / 10;
}
void Soldier::Printwin(Soldier *p,int hour,int i)
{
    if(pHeadquarter->color == RED){
        printf("%03d:40 red %s %d killed blue %s %d in city %d remaining %d elements\n"
                ,hour,Soldier::Name_of_soldier[kind],No,Soldier::Name_of_soldier[p->kind],p->No,i,life);
    }
    else{
        printf("%03d:40 blue %s %d killed red %s %d in city %d remaining %d elements\n"
                ,hour,Soldier::Name_of_soldier[kind],No,Soldier::Name_of_soldier[p->kind],p->No,i,life);
    }
}

int Soldier::count_weapon(int kind)
{
    deleteWeapon();
    sort_weapon();
    int counter = 0;
    for(int i = 0;i<Weapon_num;i++){
        if(Weapon_list[i].kind == kind) counter++;
    }
    return counter;
}
void Soldier::attack(Soldier *p)
{
    p->life -= Weapon_list[curWeapon_index].force;
    if(p->life <= 0) p->alive = false;
    if(kind != NINJA && Weapon_list[curWeapon_index].kind == BOMB){
        life -= Weapon_list[curWeapon_index].force/2;
        if(life <= 0) alive = false;
    }
    if(Weapon_list[curWeapon_index].kind != SWORD){
        Weapon_list[curWeapon_index].HP--;
    }
    curWeapon_index++;
    if(curWeapon_index == Weapon_num){
        deleteWeapon();
        sort_weapon();
    }
}

void Soldier::sort_weapon()
{
    curWeapon_index = 0;
    sort(Weapon_list.begin(),Weapon_list.end(),mysort);
}

void Soldier::sort_weapon_high()
{
    curWeapon_index = 0;
    sort(Weapon_list.begin(),Weapon_list.end(),mysort_high);
}

void Soldier::deleteWeapon()
{
    auto it = Weapon_list.begin();
    for(;it != Weapon_list.end();it++){
        if((*it).HP == 0){
            it = Weapon_list.erase(it);
            Weapon_num--;
            if(it == Weapon_list.end()) break;
        }
    }
}

void Soldier::get_weapon(Soldier *p)
{
    p->deleteWeapon();
    p->sort_weapon_high();
    int t = min(10-Weapon_num,p->Weapon_num);
    for(int i=0;i<t;i++){
        Weapon_list.push_back(p->Weapon_list[i]);
        Weapon_list[i+Weapon_num].force = get_weapon_force(Weapon_list[i+Weapon_num].kind);
    }
    Weapon_num += t;
}

bool Soldier::weapon_Zero()
{
    bool flag = true;
    auto it = Weapon_list.begin();
    for(;it != Weapon_list.end(); it++){
        if(!((*it).force == 0 && (*it).kind == SWORD) ){
            flag = false;
            break;
        }
    }
    if(Weapon_num == 0) flag = true;
    return flag;
}

void Soldier::Print_weapon(int hour)
{
    int a = count_weapon(SWORD);
    int b = count_weapon(BOMB);
    int c = count_weapon(ARROW);
    if(pHeadquarter->color == RED) printf("%03d:55 red %s %d has %d sword %d bomb %d arrow and %d elements\n"
                                            ,hour,Soldier::Name_of_soldier[kind],No,a,b,c,life);
    else printf("%03d:55 blue %s %d has %d sword %d bomb %d arrow and %d elements\n"
                                            ,hour,Soldier::Name_of_soldier[kind],No,a,b,c,life);
}

int Soldier::PK(Soldier *p,int hour,int i)
{
    sort_weapon();
    p->sort_weapon();
    while(true){
        if(life > 0 && p->life >0){
            if(!weapon_Zero()){
                attack(p);
            }
        }
        else break;
        if(life > 0 && p->life > 0){
            if(!p->weapon_Zero()){
                p->attack(this);
            }
        }
        else break;
        if(weapon_Zero() && p->weapon_Zero()) break;
    }
    deleteWeapon();
    p->deleteWeapon();
    if(isalive() && !p->isalive() ){
        Printwin(p,hour,i);
        if(kind == DRAGON) yell(hour,i);
        get_weapon(p);
        if(pHeadquarter->color == RED) return 0;
        else return 1;
    }
    if(!isalive() && p->isalive()){
        p->Printwin(this,hour,i);
        if(p->kind == DRAGON) p->yell(hour,i);
        p->get_weapon(this);
        if(pHeadquarter->color == RED) return 1;
        else return 0;
    }
    if(!isalive() && !p->isalive()){
        if(pHeadquarter->color == RED){
            printf("%03d:40 both red %s %d and blue %s %d died in city %d\n"
                ,hour,Soldier::Name_of_soldier[kind],No,Soldier::Name_of_soldier[p->kind],p->No,i);
        }
        else{
            printf("%03d:40 both red %s %d and blue %s %d died in city %d\n"
                ,hour,Soldier::Name_of_soldier[p->kind],p->No,Soldier::Name_of_soldier[kind],No,i);
        }
        return 3;
    }
    if(isalive() && p->isalive()){
        if(pHeadquarter->color == RED){
            printf("%03d:40 both red %s %d and blue %s %d were alive in city %d\n"
                ,hour,Soldier::Name_of_soldier[kind],No,Soldier::Name_of_soldier[p->kind],p->No,i);
        }
        else{
            printf("%03d:40 both red %s %d and blue %s %d were alive in city %d\n"
                ,hour,Soldier::Name_of_soldier[p->kind],p->No,Soldier::Name_of_soldier[kind],No,i);
        }
        if(kind == DRAGON && p->kind != DRAGON) yell(hour,i);
        if(p->kind == DRAGON && kind != DRAGON) p->yell(hour,i);
        if(kind == DRAGON && p->kind == DRAGON){
            if(pHeadquarter->color == RED){
                yell(hour,i);
                p->yell(hour,i);
            }
            else{
                p->yell(hour,i);
                yell(hour,i);
            }
        }
        return 2;
    }
}

void Dragon::yell(int hour,int i)
{
    char Color[10];
    if(pHeadquarter->color == RED) strcpy(Color,"red");
    else strcpy(Color,"blue");
    printf("%03d:40 %s dragon %d yelled in city %d\n",hour,Color,No,i);
}

bool Soldier::isalive()
{
    return alive;
}

int Soldier::return_kind()
{
    return kind;
}

void Wolf::rob_weapon(Soldier *p,int hour,int i)
{
    Soldier::deleteWeapon();
    p->deleteWeapon();
    if(p->return_kind() == WOLF) return;
    p->sort_weapon_high();
    if(p->Weapon_num == 0) return;
    auto it = p->Weapon_list.begin();
    int temp = (*it).kind;
    int counter = 0;
    while((*it).kind == temp && p->Weapon_num > 0 && Weapon_num < 10){
        Weapon_list.push_back((*it));
        Weapon_num++;
        Weapon_list[Weapon_num-1].force = get_weapon_force((*it).kind);
        p->Weapon_num--;
        it = p->Weapon_list.erase(it);
        it = p->Weapon_list.begin();
        counter++;
    }
    if(pHeadquarter->color == RED){
        printf("%03d:35 red wolf %d took %d %s from blue %s %d in city %d\n"
            ,hour,No,counter,Weapon::Name_of_weapon[temp],Soldier::Name_of_soldier[p->kind],p->No,i);
    }
    if(pHeadquarter->color == BLUE){
        printf("%03d:35 blue wolf %d took %d %s from red %s %d in city %d\n"
            ,hour,No,counter,Weapon::Name_of_weapon[temp],Soldier::Name_of_soldier[p->kind],p->No,i);
    }
}

void MAP::Attack(int hour)
{
    for(int i=1;i<=City_NUM;i++){
        if(City_list[i].counter == 2){
            int res;
            if(i % 2 == 1) res = City_list[i].p1->PK(City_list[i].p2,hour,i);
            else res = City_list[i].p2->PK(City_list[i].p1,hour,i);
            //delete and counter
            switch (res)
            {
            case 0:
                delete City_list[i].p2;
                City_list[i].p2 = NULL;
                City_list[i].counter--;
                break;
            case 1:
                delete City_list[i].p1;
                City_list[i].p1 = NULL;
                City_list[i].counter--;
            case 2:
                break;
            case 3:
                delete City_list[i].p1;
                delete City_list[i].p2;
                City_list[i].p1 = NULL;
                City_list[i].p2 = NULL;
                City_list[i].counter = 0;
            }
        }
    }
}

int MAP::move(int hour)
{
    int res = 1;
    for(int i=City_NUM+1;i>=1;i--){
        if(City_list[i-1].p1){
            if(City_list[i-1].p1->kind == LION) City_list[i-1].p1->Lion_Incloyalty();
            if(City_list[i-1].p1->kind == ICEMAN) City_list[i-1].p1->Iceman_Inclife();
            City_list[i].p1 = City_list[i-1].p1;
            City_list[i-1].counter--;
            City_list[i].counter++;
            City_list[i-1].p1 = NULL;
        }
    }
    for(int i=0;i<=City_NUM;i++){
        if(City_list[i+1].p2){
            if(City_list[i+1].p2->kind == LION) City_list[i+1].p2->Lion_Incloyalty();
            if(City_list[i+1].p2->kind == ICEMAN) City_list[i+1].p2->Iceman_Inclife();
            City_list[i].p2 = City_list[i+1].p2;
            City_list[i+1].counter--;
            City_list[i].counter++;
            City_list[i+1].p2 = NULL;
        }
    }
    if(City_list[0].p2 != NULL){
        res = 0;
        printf("%03d:10 blue %s %d reached red headquarter with %d elements and force %d\n"
            ,hour,Soldier::Name_of_soldier[City_list[0].p2->kind],City_list[0].p2->No,City_list[0].p2->life,Soldier::Force_of_soldier[City_list[0].p2->kind]);
        printf("%03d:10 red headquarter was taken\n",hour);
    }
    for(int i=1;i<=City_NUM;i++){
        if(City_list[i].p1 != NULL){
            printf("%03d:10 red %s %d marched to city %d with %d elements and force %d\n"
                    ,hour,Soldier::Name_of_soldier[City_list[i].p1->kind],City_list[i].p1->No,i,City_list[i].p1->life,Soldier::Force_of_soldier[City_list[i].p1->kind]);
        }
        if(City_list[i].p2 != NULL){
            printf("%03d:10 blue %s %d marched to city %d with %d elements and force %d\n"
                    ,hour,Soldier::Name_of_soldier[City_list[i].p2->kind],City_list[i].p2->No,i,City_list[i].p2->life,Soldier::Force_of_soldier[City_list[i].p2->kind]);
        }
    }
    if(City_list[City_NUM+1].p1 != NULL){
        res = 0;
        printf("%03d:10 red %s %d reached blue headquarter with %d elements and force %d\n"
                    ,hour,Soldier::Name_of_soldier[City_list[City_NUM+1].p1->kind],City_list[City_NUM+1].p1->No,City_list[City_NUM+1].p1->life,Soldier::Force_of_soldier[City_list[City_NUM+1].p1->kind]);
                printf("%03d:10 blue headquarter was taken\n",hour);
    }
    return res;
}

void MAP::Print_Soldier(int hour)
{
    for(int i=1;i<=City_NUM;i++){
        if(City_list[i].p1){
            City_list[i].p1->Print_weapon(hour);
        }
        if(City_list[i].p2){
            City_list[i].p2->Print_weapon(hour);
        }
    }
}

void MAP::clear()
{
    for(int i=0;i<25;i++){
        if(City_list[i].p1 != NULL){
            delete City_list[i].p1;
            City_list[i].p1 = NULL;
        }
        if(City_list[i].p2 != NULL){
            delete City_list[i].p2;
            City_list[i].p2 = NULL;
        }
        City_list[i].counter = 0;
    }
}

void MAP::Lion_run(int hour)
{
    for(int i=0;i<=City_NUM+1;i++){
        if(City_list[i].p1 != NULL && City_list[i].p1->kind == LION){
            if(City_list[i].p1->return_Lion_loyalty() <= 0 && i != City_NUM+1){
                printf("%03d:05 red lion %d ran away\n",hour,City_list[i].p1->No);
                City_list[i].counter--;
                delete City_list[i].p1;
                City_list[i].p1 = NULL;
            }
        }
        if(City_list[i].p2 != NULL && City_list[i].p2->kind == LION){
            if(City_list[i].p2->return_Lion_loyalty() <= 0 && i != 0){
                printf("%03d:05 blue lion %d ran away\n",hour,City_list[i].p2->No);
                City_list[i].counter--;
                delete City_list[i].p2;
                City_list[i].p2 = NULL;
            }
        }
    }
}

void MAP::Wolf_rob(int hour)
{
    for(int i=1;i<=City_NUM;i++){
        if(City_list[i].counter == 2 && City_list[i].p1->kind == WOLF){
            City_list[i].p1->rob_weapon(City_list[i].p2,hour,i);
        }
        if(City_list[i].counter == 2 && City_list[i].p2->kind == WOLF){
            City_list[i].p2->rob_weapon(City_list[i].p1,hour,i);
        }
    }
}

bool Headquarter::Make_soldier(int hour,MAP* pmap)
{
    if(stop) return false;
    int index = curmaking;
    if(Soldier::HP_of_soldier[making_list[index]] > life){
        stop = true;
        return false;
    }
    life -= Soldier::HP_of_soldier[making_list[index]];
    curmaking = (index+1) % SOLDIER_NUM;
    total_soldier++;
    int k = making_list[index];
    switch (k)
    {
    case DRAGON:
        psoldier = new Dragon(this,k);
        break;
    case NINJA:
        psoldier = new Ninja(this,k);
        break;
    case ICEMAN:
        psoldier = new Iceman(this,k);
        break;
    case LION:
        psoldier = new Lion(this,k);
        break;
    case WOLF:
        psoldier = new Wolf(this,k);
        break;
    }
    num[making_list[index]]++;
    psoldier->Print(hour);
    if(color == 0) {
    	pmap->City_list[0].p1 = psoldier;
    	pmap->City_list[0].counter++;
	}
    else {
    	pmap->City_list[MAP::City_NUM+1].p2 = psoldier;
    	pmap->City_list[MAP::City_NUM+1].counter++;
	}
    return true;
}

const char* Soldier::Name_of_soldier[SOLDIER_NUM]={"dragon","ninja","iceman","lion","wolf"};
int Soldier::Force_of_soldier[SOLDIER_NUM];
const char* Weapon::Name_of_weapon[WEAPON_NUM]={"sword","bomb","arrow"};
int Weapon::HP_of_weapon[WEAPON_NUM]={-1,1,2};//-1 for forever
int Soldier::HP_of_soldier[SOLDIER_NUM];
int MAP::City_NUM;
int Lion::K;

int main()
{
    int t;
    int M;
    int N;
    int K;
    int T;
    scanf("%d",&t);
    int caseNo = 1;
    while(t--){
        printf("Case %d:\n",caseNo);
        caseNo++;
        scanf("%d %d %d %d",&M,&N,&K,&T);
        Headquarter redhead(RED,M);
        Headquarter bluehead(BLUE,M);
        MAP map = MAP(N);
        MAP* pmap = &map;
        MAP::City_NUM = N;
        Lion::K = K;
        for(int i=0;i<SOLDIER_NUM;i++){
            scanf("%d",&Soldier::HP_of_soldier[i]);
        }
        for(int i=0;i<SOLDIER_NUM;i++){
            scanf("%d",&Soldier::Force_of_soldier[i]);
        }
        int hour = 0;
        int minute = 0;
        redhead.making_list[0]=2;
        redhead.making_list[1]=3;
        redhead.making_list[2]=4;
        redhead.making_list[3]=1;
        redhead.making_list[4]=0;
        bluehead.making_list[0]=3;
        bluehead.making_list[1]=0;
        bluehead.making_list[2]=1;
        bluehead.making_list[3]=2;
        bluehead.making_list[4]=4;
        while(1){
            if(hour * 60 + minute <= T){
                redhead.Make_soldier(hour,pmap);
                bluehead.Make_soldier(hour,pmap);
                minute += 5;
            }
            else break;
            if(hour * 60 + minute <= T){
                pmap->Lion_run(hour);
                minute += 5;
            }
            else break;
            if(hour * 60 + minute <= T){
                int res = pmap->move(hour);
                if(res == 0) break;
                minute += 25;
            }
            else break;
            if(hour * 60 + minute <= T){
                pmap->Wolf_rob(hour);
                minute += 5;
            }
            if(hour * 60 + minute <= T){
                pmap->Attack(hour);
                minute += 10;
            }
            else break;
            if(hour * 60 + minute <= T){
                redhead.Print_total_life(hour);
                bluehead.Print_total_life(hour);
                minute += 5;
            }
            else break;
            if(hour * 60 + minute <= T){
                pmap->Print_Soldier(hour);
                minute = 0;
            }
            else break;
            hour++;
        }
        pmap->clear();
    }
    return 0;
}
