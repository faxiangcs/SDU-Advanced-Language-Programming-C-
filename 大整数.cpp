#include <iostream>
#include <string>
using namespace std;

class HugeInteger
{
private:
    string integer;               //用string类型储存数值
public:

    HugeInteger(){                //默认为00，其中第一个0为占位符，从第二位开始为数值，输出时可以省略
        integer="00";
    };

    HugeInteger(int a){           //将int类型转化为HugeInteger类型
        string x;
        x=to_string(a);
        integer="0"+x;
    } ;
    HugeInteger(string a){        //将string类型转化为HugeInteger类型
        integer="0"+a;
    };

    HugeInteger operator+(HugeInteger a){       //加法运算符（+）重载，实现HugeInteger类型相加
        HugeInteger INT,max0;
        max0=a;
        INT=*this;

        int Size,Size1,Size2,n=0;

        Size1=INT.integer.size();
        Size2=a.integer.size();

        if (INT.integer.size()>a.integer.size())
        {
            n=1;
            max0=INT;
        };
        if (n==0) Size=Size1;
        else Size=Size2;
        for (int i=0;i<Size;i++)
        {
            int x=0,y=0,z=0;
            x=INT.integer[Size1-i]-'0';     //将那一位对应的字符转换成字符所代表的数字
            y=a.integer[Size2-i]-'0';
            z=x+y;
            x=z%10;
            y=z/10;
            INT.integer[Size1-i]=x+'0';    //将数字转化成字符，再替代此位置字符
            int j=i;
            while(y==1){               //实现加1后连续进位
                if (INT.integer[Size1-j-1]=='9') INT.integer[Size1-j-1] ='0';
                else{
                    INT.integer[Size1-j-1]+=y;
                    y=0;
                }
                j++;
            }
        };

        return INT;
    };
    HugeInteger operator+(int a){     //加法运算符（+）重载，实现HugeInteger类型与int类型相加

        HugeInteger INT(a);           //将int类型转化成HugeInteger类型，再做加法
        INT=*this+INT;
        return INT;
    };
    HugeInteger operator+(string a){  //与上同理
        HugeInteger INT(a);
        INT=*this+INT;
        return INT;
    };

    friend ostream& operator<<(ostream& os,HugeInteger a){      //重载<<符，实现连续输出
        if (a.integer[0]=='0')    //判断首位是否是0，若为0，则从第二位开始输出                        
        {
            int k;
            k=a.integer.size();
            for (int i=1;i<k;i++)
            {
                os<<a.integer[i];
            }
            return os;
        }
        else
        {
            os<<a.integer;
            return os;
        }
    };
};

int main()
{
   HugeInteger n1( 87654321 );
   HugeInteger n2( 78901234 );
   HugeInteger n3( "99999999999999999999999999999" );
   HugeInteger n4( "1" );
   HugeInteger n5;
   cout << "n1 is " << n1 << "\nn2 is " << n2
      << "\nn3 is " << n3 << "\nn4 is " << n4
      << "\nn5 is " << n5 << "\n\n";
   n5 = n1 + n2;
   cout << n1 << " + " << n2 << " = " << n5 << "\n\n";
   cout << n3 << " + " << n4 << "\n= " << ( n3 + n4 ) << "\n\n";
   n5 = n1 + 9;
   cout << n1 << " + " << 9 << " = " << n5 << "\n\n";
   n5 = n2 + "10000";
   cout << n2 << " + " << "10000" << " = " << n5 << endl;

    return 0;
}
