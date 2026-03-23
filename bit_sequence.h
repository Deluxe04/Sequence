#ifndef BIT_SEQUENCE_H
#define BIT_SEQUENCE_H

#include "sequence.h"
#include "array_sequence.h"

// Класс для работы с битами
class Bit 
{
private:
    bool value;
public:
    Bit(bool v = false) : value(v) {}
    Bit(int v) : value(v != 0) {}
    
    operator bool() const { return value; }
    operator int() const { return value ? 1 : 0; }
    
    Bit operator!() const { return Bit(!value); }
    Bit operator&&(const Bit& other) const { return Bit(value && other.value); }
    Bit operator||(const Bit& other) const { return Bit(value || other.value); }
    Bit operator^(const Bit& other) const { return Bit(value != other.value); }
    
    bool getValue() const { return value; }
};

class BitSequence : public MutableArraySequence<Bit> 
{
public:
    using MutableArraySequence<Bit>::MutableArraySequence;

    // Конструктор из сырого массива байт
    BitSequence(const unsigned char* bytes, int numBits) : MutableArraySequence<Bit>() 
    {
        if (numBits < 0) throw InvalidArgumentException("Number of bits cannot be negative");
        
        for (int i = 0; i < numBits; ++i) 
        {
            int byteIndex = i / 8;
            int bitIndex = 7 - (i % 8);
            bool bitValue = (bytes[byteIndex] >> bitIndex) & 1;
            this->Append(Bit(bitValue));
        }
    }

    // Побитовые операции
    BitSequence* And(const BitSequence& other) const 
    {
        int len = this->GetLength();
        if (len != other.GetLength()) throw InvalidArgumentException("Sequences must have same length");
        
        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i) 
        {
            resultBits[i] = Bit(this->Get(i).getValue() && other.Get(i).getValue());
        }
        BitSequence* result = new BitSequence(resultBits, len);
        delete[] resultBits;
        return result;
    }

    BitSequence* Or(const BitSequence& other) const 
    {
        int len = this->GetLength();
        if (len != other.GetLength()) throw InvalidArgumentException("Sequences must have same length");
        
        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i) 
        {
            resultBits[i] = Bit(this->Get(i).getValue() || other.Get(i).getValue());
        }
        BitSequence* result = new BitSequence(resultBits, len);
        delete[] resultBits;
        return result;
    }

    BitSequence* Xor(const BitSequence& other) const 
    {
        int len = this->GetLength();
        if (len != other.GetLength()) throw InvalidArgumentException("Sequences must have same length");
        
        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i) 
        {
            bool val1 = this->Get(i).getValue();
            bool val2 = other.Get(i).getValue();
            resultBits[i] = Bit((val1 && !val2) || (!val1 && val2));
        }
        BitSequence* result = new BitSequence(resultBits, len);
        delete[] resultBits;
        return result;
    }

    BitSequence* Not() const 
    {
        int len = this->GetLength();
        Bit* resultBits = new Bit[len];
        for (int i = 0; i < len; ++i) 
        {
            resultBits[i] = Bit(!this->Get(i).getValue());
        }
        BitSequence* result = new BitSequence(resultBits, len);
        delete[] resultBits;
        return result;
    }
};

#endif // BIT_SEQUENCE_H