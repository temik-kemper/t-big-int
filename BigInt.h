/** TODO:
 * �������� bin to bin-dec              done
 * ������� bin-dec to bin               done
 * ����� ������� ������ � �������� ���� later
 * setsign ��� ������ � size==0 �������� ��������� ����
**/
#ifndef BIGINT_H
#define BIGINT_H

#include<string>

union U_tran
{
    bool                U_bool;
    unsigned char       U_Uchar[8];
    unsigned char       U_UUchar;
    char                U_char;
    int                 U_int;
    unsigned int        U_Uint;
    long                U_long;
    unsigned long       U_Ulong;
    long long           U_longlong;
    unsigned long long  U_Ulonglong;

};

union U_summ
{
    unsigned short U_Ushort;
    unsigned char U_Uchar[2];
};



char BinToHex(unsigned char UC);

class TBigNum
{
private:

    unsigned char       *f_pnum;
    unsigned long long   f_size; //� ������� ���, �� ��� �������� �����������
    bool                 f_sign; //���� �����

public:

    void SetSize(unsigned long long setsize);
    // � ����� � ���**��� ������ ����� �������������� �*��� �������, ������ ��� � ���� ������� ��� ������ ���� � ����

    //����������, ���� �� ���������� ����������� �������� ���������� � cpp ???
    template<typename T>
    void SetNumb(T input)
    {
        U_tran Utran;    //������ ���� ����� - �������� ������ � ��������� �����
        if(input < 0)
        {
            f_sign = true;
            input *= (-1);
        }else{
            f_sign = false;
        }
        Utran.U_Ulonglong = input;
        SetSize(9);
        for (unsigned long long i = 0; i<f_size-1; i++)
        {
            f_pnum[i] = Utran.U_Uchar[i];
        }
        if(f_sign)
        {
            f_pnum[f_size-1] |= 1<<7;
        }else{
            f_pnum[f_size-1] &= 127;
        }

        MemoryCleanUp();
    }



    //

    unsigned long long GetSize(){return f_size;}
    bool GetSign(){return f_sign;}
    unsigned char GetData(unsigned long long index){return f_pnum[index];}
    void SetData(unsigned char data ,unsigned long long index){f_pnum[index] = data;}
    void SetSign(bool nsign){f_sign = nsign; if(nsign)f_pnum[f_size-1]|=1<<7;else f_pnum[f_size-1] &= 127;}


    TBigNum();
    TBigNum(unsigned long long ssize);
    TBigNum(TBigNum &TBN);
    TBigNum(std::string input);
    ~TBigNum();

    void PrintBinary();

    TBigNum & operator=(const TBigNum & TBN);


    // ������ �� ����� �� ������ ������� � �������������� ���� ??
    //������ ����� ������ ������� � �������������� � ������� � ������, �������, ������
    //� ���� ������� ��� ��������

    void Tcomplement(); //������� � �������������� ���
    void UnTcomplement(); // �� ���� ���� ����� ��� ����� �� ������, �� ��������� �� ������ ���� � ��������� ����
    //������ ��� ������� ������ �������������� ��������

    bool MemoryCleanUp(); //������ �������� ������

    void zdvig();

    TBigNum operator+(const TBigNum &TBN2)const;
    TBigNum operator-(const TBigNum &TBN2);


    void PrintHex();


/**
* ���� ��������
* ������� ������
**/

    void BinToBindec(){}

    void PrintBinDec(bool debug);

    bool StrToBin(std::string input);


    void test();


};



#endif // BIGINT_H
