// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw ("Negative length");
    else
    {
        MemLen = (len + (sizeof(TELEM) * 8 - 1)) >> sizeof(TELEM);
        BitLen = len;
        pMem = new TELEM[MemLen];
        if (pMem != nullptr) //!!!
        {
            for (int i = 0; i < MemLen; i++)
                pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    if (bf.BitLen < 0)
        throw ("Negative length");
    else
    {
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        pMem = new TELEM[MemLen];
        if (pMem != nullptr)
        {
            for (int i = 0; i < MemLen; i++)
                pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (0 <= n < BitLen) 
        return (n / (sizeof(TELEM) * 8));
    else
        throw ("Number out of range");
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (0 <= n < BitLen) 
        return (1 << (n % (sizeof(TELEM) * 8)));
    else 
        throw ("Valid number");
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
        throw ("The number is out of the bit field");
    pMem[GetMemIndex(n)] |= (GetMemMask(n));
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw ("The number is out of the bit field");
    }
    else 
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen))
        throw "Out of range";
    else 
        return (pMem[GetMemIndex(n)] & GetMemMask(n)) == 0 ? 0 : 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen)
    {
        bool el;
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] != bf.pMem[i])
                return 0;
        }
        return 1;
    }
    else
        return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len;
    if (BitLen < bf.BitLen)
        len = bf.BitLen;
    else
        len = BitLen;

    TBitField tmp(len);

    for (int i = 0; i < MemLen; i++) 
    {
        tmp.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) 
    {
        tmp.pMem[i] |= bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len;
    if (BitLen < bf.BitLen)
        len = bf.BitLen;
    else
        len = BitLen;

    TBitField tmp(len);

    for (int i = 0; i < MemLen; i++) {
        tmp.pMem[i] = pMem[i];
    }

    for (int i = 0; i < bf.MemLen; i++) {
        tmp.pMem[i] &= bf.pMem[i];
    }

    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp = (*this);

    for (int i = 0; i < BitLen; i++)
    {
        if (tmp.GetBit(i) == 1)
            tmp.ClrBit(i);
        else
            tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    size_t a;
    for (int i = 0; i < bf.GetLength(); i++)
    {
        istr >> a;
        if (a == 0)
            bf.ClrBit(i);
        else
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) 
        ostr << bf.GetBit(i);

    return ostr;
}
