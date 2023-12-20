#include <iostream>
#include <string>
using namespace std;

class HugeInteger
{
private:
    string integer;               //��string���ʹ�����ֵ
public:

    HugeInteger(){                //Ĭ��Ϊ00�����е�һ��0Ϊռλ�����ӵڶ�λ��ʼΪ��ֵ�����ʱ����ʡ��
        integer="00";
    };

    HugeInteger(int a){           //��int����ת��ΪHugeInteger����
        string x;
        x=to_string(a);
        integer="0"+x;
    } ;
    HugeInteger(string a){        //��string����ת��ΪHugeInteger����
        integer="0"+a;
    };

    HugeInteger operator+(HugeInteger a){       //�ӷ��������+�����أ�ʵ��HugeInteger�������
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
            x=INT.integer[Size1-i]-'0';     //����һλ��Ӧ���ַ�ת�����ַ������������
            y=a.integer[Size2-i]-'0';
            z=x+y;
            x=z%10;
            y=z/10;
            INT.integer[Size1-i]=x+'0';    //������ת�����ַ����������λ���ַ�
            int j=i;
            while(y==1){               //ʵ�ּ�1��������λ
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
    HugeInteger operator+(int a){     //�ӷ��������+�����أ�ʵ��HugeInteger������int�������

        HugeInteger INT(a);           //��int����ת����HugeInteger���ͣ������ӷ�
        INT=*this+INT;
        return INT;
    };
    HugeInteger operator+(string a){  //����ͬ��
        HugeInteger INT(a);
        INT=*this+INT;
        return INT;
    };

    friend ostream& operator<<(ostream& os,HugeInteger a){      //����<<����ʵ���������
        if (a.integer[0]=='0')    //�ж���λ�Ƿ���0����Ϊ0����ӵڶ�λ��ʼ���                        
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
