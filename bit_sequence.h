#ifndef BIT_SEQUENCE_H
#define BIT_SEQUENCE_H

#include "array_sequence.h"

class Bit 
{
private:
    bool value;
public:
    Bit(bool v = false) : value(v) {}
    Bit(int v) : value(v != 0) {}
    
    operator bool() const 
    { 
        return value; 
    }

    operator int() const 
    { 
        return value ? 1 : 0; 
    }
    
    Bit operator!() const 
    { 
        return Bit(!value);
    }

    Bit operator&&(const Bit& other) const 
    { 
        return Bit(value && other.value);
    }

    Bit operator||(const Bit& other) const 
    { 
        return Bit(value || other.value); 
    }

    Bit operator^(const Bit& other) const 
    { 
        return Bit(value != other.value); 
    }
    
    bool getValue() const 
    { 
        return value;
    }
};

class BitSequence : public MutableArraySequence<Bit> 
{
protected:
    Sequence<Bit>* instance(const Bit* arr, int count) const override
    {
        return new BitSequence(arr, count);
    }
public:
    using MutableArraySequence<Bit>::MutableArraySequence;

    //Конструктор из сырого массива байт
    BitSequence(const unsigned char* bytes, int numBits) : MutableArraySequence<Bit>() 
    {
        if (numBits < 0) 
        {
            throw InvalidArgumentException();
        }
        
        if (numBits > 0 && bytes == nullptr)
        {
            throw InvalidArgumentException();
        }

        for (int i = 0; i < numBits; ++i)
        {
            int  byteIndex = i / 8;
            int  bitIndex  = 7 - (i % 8);
            bool bitValue  = (bytes[byteIndex] >> bitIndex) & 1;
            this->Append(Bit(bitValue));
        }
    }

private:
    static BitSequence* makeBitSeq(Bit* bits, int len)
    {
        BitSequence* result = nullptr;
        try
        {
            result = new BitSequence(bits, len);
        } catch (...)
        {
            delete[] bits;  
            throw;          
        }
        delete[] bits;      
        return result;
    }

public:
    BitSequence* And(const BitSequence& other) const
    {
        int len = this->GetLength();
        if (len != other.GetLength())
        {
            throw InvalidArgumentException();
        }

        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i)
        {
            resultBits[i] = Bit(this->Get(i).getValue() && other.Get(i).getValue());
        }
        return makeBitSeq(resultBits, len);
    }

    BitSequence* Or(const BitSequence& other) const
    {
        int len = this->GetLength();
        if (len != other.GetLength())
        {
            throw InvalidArgumentException();
        }

        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i)
        {
            resultBits[i] = Bit(this->Get(i).getValue() || other.Get(i).getValue());
        }
        return makeBitSeq(resultBits, len);
    }

    BitSequence* Xor(const BitSequence& other) const
    {
        int len = this->GetLength();
        if (len != other.GetLength())
        {
            throw InvalidArgumentException();
        }
        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i)
        {
            bool v1 = this->Get(i).getValue();
            bool v2 = other.Get(i).getValue();
            resultBits[i] = Bit((v1 && !v2) || (!v1 && v2));
        }

        return makeBitSeq(resultBits, len);
    }

    BitSequence* Not() const
    {
        int len = this->GetLength();
        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i)
        {
            resultBits[i] = Bit(!this->Get(i).getValue());
        }
        return makeBitSeq(resultBits, len);
    }
};

#endif // BIT_SEQUENCE_H
