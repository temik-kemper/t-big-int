#include "BigInt.h"
#include <iostream>

using namespace std;

//запись x &= 0<<7 для зануления последнего бита сломала мне жизнь и вызывала баги
//да здравствует x &= 127;

void TBigNum::SetSize(unsigned long long setsize)
{
    if(setsize != f_size)
    {
        unsigned char *trans = new unsigned char[setsize];
        if ((f_sign))
        {

            if(f_size > 0) f_pnum[f_size-1] &=127;

            for (unsigned long long i=0; ((i<f_size)&&(i<setsize)); i++)
            {
                trans[i] = f_pnum[i];
            }
            for (unsigned long long i=f_size; i<setsize; i++)
            {
                trans[i] = 0;
            }
            trans[setsize-1] |=1<<7;

        }else{

            for (unsigned long long i=0; ((i<f_size)&&(i<setsize)); i++)
            {
                trans[i] = f_pnum[i];
            }
            for (unsigned long long i=f_size; i<setsize; i++)
            {
                trans[i] = 0;
            }
            trans[setsize-1] &=127;

        }//if f_size != setsize

        delete [] f_pnum;
        f_size = setsize;
        f_pnum = trans;
    }

}






TBigNum::TBigNum()
{
    f_pnum = NULL;
    f_size = 0;
    f_sign = 0;
}

TBigNum::TBigNum(string input)
{
    f_pnum = NULL;
    f_size = 0;
    f_sign = 0;
    StrToBin(input);
}

TBigNum::TBigNum(unsigned long long ssize)
{
    f_pnum = NULL;
    f_size = 0;
    f_sign = 0;
    SetSize(ssize);
}

TBigNum::TBigNum(TBigNum &TBN)
{
    f_pnum = NULL;
    f_size = 0;
    f_sign = false;
    if(TBN.GetSize() > 0)
    {
        SetSize(TBN.GetSize());
        this->f_sign = TBN.GetSign();
        for(unsigned long long i =0; i<f_size; i++)
        {
            this->f_pnum[i] = TBN.GetData(i);
        }
    }
}

TBigNum::~TBigNum()
{
    delete [] f_pnum;
}

void TBigNum::PrintBinary()
{
    for(unsigned long long i = (f_size -1); i > 0; i--)
    {
        for(int j=7; j>=0; j--)
        {
            cout << ((f_pnum[i] >> j) & 1);
        }
        cout << " " ;
    }
    for(int j=7; j>=0; j--)
        {
            cout << ((f_pnum[0] >> j) & 1);
        }
        cout << " " ;
}

/// да он написан отвратительно
/// да я вспомнил об этом поздно
/// нет мне нормально, пускай ещё так полежит
TBigNum & TBigNum::operator=(const TBigNum & TBN)
{
    if(this->f_pnum != NULL) delete [] this->f_pnum;

    if(TBN.f_size == 0)
    {
        this->f_size = 0;
        this->f_pnum = NULL;
        this->f_sign = 0;
    }else{
        this->f_pnum = new unsigned char[TBN.f_size];
        this->f_size = TBN.f_size;
        this->f_sign = TBN.f_sign;
        for(unsigned long long i=0; i<this->f_size; i++)
        {
            this->f_pnum[i] = TBN.f_pnum[i];
        }
    }
}

void TBigNum::Tcomplement()
{
    if (f_sign != 0)
    {
        f_pnum[f_size-1] &= 127;

        for (unsigned long long i = 0; i<f_size; i++)
        {
            f_pnum[i] = ~(f_pnum[i]);
        }
        U_summ sum;
        sum.U_Ushort = f_pnum[0];
        sum.U_Ushort += 1;
        f_pnum[0] = sum.U_Uchar[0];
        unsigned long long counter = 1;
        while (sum.U_Uchar[1])
        {
            sum.U_Ushort=0;
            sum.U_Ushort=f_pnum[counter];
            counter++;
            sum.U_Ushort += 1;
            f_pnum[counter] = sum.U_Uchar[0];
        }
    }
}

void TBigNum::UnTcomplement()
{
    if (f_pnum[f_size-1] & 1<<7)
    {

        for (unsigned long long i = 0; i<f_size; i++)
        {
            f_pnum[i] = ~(f_pnum[i]);
        }
        U_summ sum;
        sum.U_Ushort = f_pnum[0];
        sum.U_Ushort += 1;
        f_pnum[0] = sum.U_Uchar[0];
        unsigned long long counter = 1;
        while (sum.U_Uchar[1])
        {
            sum.U_Ushort=0;
            sum.U_Ushort=f_pnum[counter];
            counter++;
            sum.U_Ushort += 1;
            f_pnum[counter] = sum.U_Uchar[0];
        }
        f_sign = true;
        f_pnum[f_size-1] |= 1<<7;
    }else{
        f_sign = false;
    }

}

// трубочистим ненужные биты по 8 штук
bool TBigNum::MemoryCleanUp()
{
    unsigned char six_shooter; //я честно беспонятия зачем мне были нужны эти переменные и почему у них такие имена
    unsigned char one_per_seven; //но мне это нравится и оно тут остаётся как часть истории, которую я уже не вспомню
    while ((f_size > 1)&&((f_pnum[f_size-1]&127) ==0)&&((f_pnum[f_size-2] & 1<<7) == 0))
    {
        SetSize(f_size-1);
    }
}


TBigNum TBigNum::operator+(const TBigNum &TBN2)const
{
    TBigNum PTBN1, PTBN2;

    PTBN1.SetSize(f_size);
    PTBN1.SetSign(f_sign);
    for(unsigned long long i = 0; i<f_size; i++)
    {
        PTBN1.SetData(f_pnum[i],i);
    }

    PTBN2 = TBN2;
    unsigned long long n_size;
    if(PTBN1.GetSize() > PTBN2.GetSize()) n_size = PTBN1.GetSize();else n_size = PTBN2.GetSize();
    n_size++;
    U_summ sum;
    sum.U_Ushort=0;
    PTBN1.SetSize(n_size);
    PTBN2.SetSize(n_size);
    PTBN1.Tcomplement();
    PTBN2.Tcomplement();
    for (unsigned long long i=0;i<n_size;i++)
    {
        sum.U_Ushort=PTBN1.GetData(i) + PTBN2.GetData(i) + sum.U_Uchar[1];
        PTBN1.SetData(sum.U_Uchar[0], i);
    }


    PTBN1.UnTcomplement();

    PTBN1.MemoryCleanUp();

    return PTBN1;


}



TBigNum TBigNum::operator-(const TBigNum &TBN2)
{
    TBigNum PTBN1, PTBN2;
    ///PTBN1 = this

    PTBN1.SetSize(f_size);

    for(unsigned long long i = 0; i< f_size; i++)
    {
        PTBN1.SetData(f_pnum[i],i);
    }

    PTBN1.SetSign(f_sign);

    ///___
    PTBN2 = TBN2;
    if ((PTBN2.GetData(PTBN2.GetSize()-1) & 1<<7) != 0)
    {
        PTBN2.SetData((PTBN2.GetData(PTBN2.GetSize()-1)&127) , PTBN2.GetSize()-1);
        PTBN2.SetSign(false);
    }else{
        PTBN2.SetData((PTBN2.GetData(PTBN2.GetSize()-1)|(1<<7)) , PTBN2.GetSize()-1);
        PTBN2.SetSign(true);
    }

    PTBN1 = PTBN1 + PTBN2;
    return PTBN1;
}


char BinToHex(unsigned char UC)
{
    char res = 0;
    switch (UC)
    {
    case 0:
        res = '0';
        break;
    case 1:
        res = '1';
        break;
    case 2:
        res = '2';
        break;
    case 3:
        res = '3';
        break;
    case 4:
        res = '4';
        break;
    case 5:
        res = '5';
        break;
    case 6:
        res = '6';
        break;
    case 7:
        res = '7';
        break;
    case 8:
        res = '8';
        break;
    case 9:
        res = '9';
        break;
    case 10:
        res = 'A';
        break;
    case 11:
        res = 'B';
        break;
    case 12:
        res = 'C';
        break;
    case 13:
        res = 'D';
        break;
    case 14:
        res = 'E';
        break;
    case 15:
        res = 'F';
        break;
    default:
        res = 0;
    }
    return res;
}

void TBigNum::PrintHex()
{
    unsigned char tHex[2];
    for (unsigned long long i = f_size-1; i!=0; i--)
    {
        tHex[0] = f_pnum[i] & 15; /// 00001111
        tHex[1] = f_pnum[i] & 240;/// 11110000
        tHex[1] = tHex[1] >> 4;
        cout << BinToHex(tHex[1])<<BinToHex(tHex[0])<<" ";
    }
        tHex[0] = f_pnum[0] & 15;
        tHex[1] = f_pnum[0] & 240;
        tHex[1] = tHex[1] >> 4;
        cout << BinToHex(tHex[1])<<BinToHex(tHex[0])<<" ";
}



/*
*   Дождь проходит сквозь меня
*   Я человек дуршлаг
*
*   я разобрался с переводом, но пока не успел его реализовать
*/

//ВАЖНО, этот метод используется для вывода десятичных, он не изменяет размера числа
void TBigNum::zdvig() //ВАЖНО, этот метод используется для вывода десятичных, он не изменяет размера числа
{
    unsigned char remember=0;
    /*
    Do you remember the 21st night of September?
    Love was changing the minds of pretenders
    While chasing the clouds away
    */
    U_summ U2b;
    U2b.U_Ushort = 0;



    f_pnum[f_size-1] &= 127;

    for (unsigned long long i = 0; i<f_size; i++)
    {
        U2b.U_Ushort = f_pnum[i];
        f_pnum[i] = (f_pnum[i]<<1) + remember;
        U2b.U_Ushort = U2b.U_Ushort<<1;
        if (U2b.U_Uchar[1])remember = 1;else remember = 0;
        U2b.U_Ushort = 0;
    }

    if(f_sign)f_pnum[f_size-1] |= 1<<7;



}



void TBigNum::PrintBinDec(bool debug)
{
    TBigNum tcopy;
    tcopy.SetSize(f_size); // так делать не хорошо
    for(unsigned long long i = 0; i<f_size; i++)
    {
        tcopy.SetData(f_pnum[i], i);
    }
    tcopy.SetData((tcopy.GetData(tcopy.GetSize()-1)&127), f_size-1);
    unsigned char BinDec[f_size*3];
// оказывается в созданном массиве стабильно оказывается мусорная единичка, впервые отладка меня серьёзно спасла =)
    for(unsigned long long i = 0; i<f_size*3; i++) BinDec[i]=0;


    unsigned char remember; //do u remember

    for(unsigned long long i = 0; i<8*f_size; i++)
    {
        for(unsigned long long g = 0; g < f_size*3; g++)
        {
            if( BinDec[g] >= 5)
            {
                BinDec[g] += 3;
            }
        }


        for(unsigned long long g = 0; g<f_size*3; g++)
        {
            if (BinDec[g]>>4 != 0)
            {
                remember = BinDec[g]>>4;
                BinDec[g] &= 15;
                BinDec[g+1] += remember;
            }
        }

        if(tcopy.GetData(f_size-1) & 1<<7)remember = 1; else remember = 0;

        tcopy.zdvig();

        for(unsigned long long g = 0; g<f_size*3; g++)
        {
            BinDec[g] = (BinDec[g]<<1)+remember;
            if(BinDec[g]&1<<4)
            {
                remember = 1;
                BinDec[g] &= 15;
            }else{
                remember = 0;
            }
        }
    }//успех
//    cout<<"in dec = ";
    if (f_sign)cout<<"-";
    bool fzero = false;
    for(unsigned long long i = f_size*3-1; i!=0; i--)
    {
        if((BinDec[i]!=0)||(fzero))
        {
            fzero = true;
            cout<<+BinDec[i];
        }
    }
    cout<<+BinDec[0]<<"\n";



}


bool TBigNum::StrToBin(string input)
{
    bool newsign = false;
    bool err = false;
    if (input[0] == '-')
    {
        newsign = true;
        input.erase(input.begin());
    }

    unsigned char BCD[input.size()];

    for(unsigned long long i=0; i<input.size(); i++)
    {
        switch(input[i])
        {
        case '0':
            BCD[input.size()-1-i] = 0;
            break;
        case '1':
            BCD[input.size()-1-i] = 1;
            break;
        case '2':
            BCD[input.size()-1-i] = 2;
            break;
        case '3':
            BCD[input.size()-1-i] = 3;
            break;
        case '4':
            BCD[input.size()-1-i] = 4;
            break;
        case '5':
            BCD[input.size()-1-i] = 5;
            break;
        case '6':
            BCD[input.size()-1-i] = 6;
            break;
        case '7':
            BCD[input.size()-1-i] = 7;
            break;
        case '8':
            BCD[input.size()-1-i] = 8;
            break;
        case '9':
            BCD[input.size()-1-i] = 9;
            break;
        default:
            err=true;
            BCD[input.size()-1-i]=0;
        }
    }

    if(err==false)
    {

        ///шит гоус хиар

        SetSize(input.size()+1);
        SetSign(newsign);
        unsigned char remember1 = 0;
        unsigned char remember2 = 0;
        bool inProgress = true;
        unsigned long long counter = 0;

        while((inProgress)&&(input.size()!=0))
        {
            for(unsigned long long i = input.size()-1; i!=0; i--)
            {
                remember2 = BCD[i]&1;
                BCD[i] = BCD[i]>>1;
                if(remember1)BCD[i] |=1<<3;
                remember1 = remember2;
            }
            remember2 = BCD[0]&1;
            BCD[0] = (BCD[0]>>1);
            if(remember1)BCD[0] |= 1<<3;
            ///_________
            f_pnum[counter/8] |= (remember2)<<(counter%8); /// 3дня боли, чтоб решить задачу в одну строчку

            counter++;
            remember1=0;

            inProgress=false;
            for(unsigned long long i = 0; i<input.size();i++)
            {
                if(BCD[i]!=0)inProgress=true;
                if(BCD[i]>=8)BCD[i]-=3;
            }

        }
        MemoryCleanUp();



    }else{
            cout<<"\n\nERROR IN STRING\n\n";
            throw -1;
        }


    return err;
}



void TBigNum::test()
{
    if(f_size > 1)cout << "its works";else cout<<"fuck";
}

