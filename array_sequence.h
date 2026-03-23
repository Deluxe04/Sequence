#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "sequence.h"
#include "dynamic_array.h"

template <class T>
class ArraySequenceBase : public Sequence<T> 
{
protected:
    DynamicArray<T>* items;

public:
    // Конструкторы
    ArraySequenceBase(const T* items, int count) : items(new DynamicArray<T>(items, count)) {}
    ArraySequenceBase() : items(new DynamicArray<T>(0)) {}
    ArraySequenceBase(const ArraySequenceBase<T>& other) : items(new DynamicArray<T>(*other.items)) {}

    // Деструктор
    ~ArraySequenceBase() override 
    {
        delete items;
    }

    // Декомпозиция 
    const T& GetFirst() const override 
    {
        return items->Get(0);
    }

    const T& GetLast() const override 
    {
        return items->Get(items->GetSize() - 1);
    }

    const T& Get(int index) const override
    {
        return items->Get(index);
    }

    int GetLength() const override 
    {
        return items->GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override 
    {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex) 
        {
            throw IndexOutOfRangeException();
        }
        
        int subLength = endIndex - startIndex + 1;
        T* tempArray = new T[subLength];
        for (int i = startIndex; i <= endIndex; ++i)
        {
            tempArray[i - startIndex] = this->Get(i);
        }
        
        Sequence<T>* subSeq = this->CreateSequence(tempArray, subLength);
        delete[] tempArray;
        return subSeq;
    }
    // Виртуальный метод для создания пустой последовательности 
    virtual Sequence<T>* CreateSequence(const T* arr, int count) const = 0;
};

//Mutable версия 
template <class T>
class MutableArraySequence : public ArraySequenceBase<T> 
{
public:
    using ArraySequenceBase<T>::items;
    using ArraySequenceBase<T>::GetLength;

    MutableArraySequence(const T* items, int count) : ArraySequenceBase<T>(items, count) {}
    MutableArraySequence() : ArraySequenceBase<T>() {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequenceBase<T>(other) {}

    Sequence<T>* CreateSequence(const T* arr, int count) const override 
    {
        return new MutableArraySequence<T>(arr, count);
    }

    Sequence<T>* Append(const T& item) override 
    {
        return InsertAt(item, GetLength()); 
    }   

    Sequence<T>* Prepend(const T& item) override 
    {
        return InsertAt(item, 0);
    }

    Sequence<T>* InsertAt(const T& item, int index) override 
    {
        if (index < 0 || index > GetLength()) throw IndexOutOfRangeException();
        
        items->Resize(GetLength() + 1);
        for (int i = GetLength() - 1; i > index; --i) 
        {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        return this;
    }

    Sequence<T>* RemoveAt(int index) override {
        if (index < 0 || index >= GetLength()) throw IndexOutOfRangeException();
        
        for (int i = index; i < GetLength() - 1; ++i) 
        {
            items->Set(i, items->Get(i + 1));
        }
        items->Resize(GetLength() - 1);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* other) override 
    {
        int otherLen = other->GetLength();
        int currentLen = GetLength();
        items->Resize(currentLen + otherLen);
        for (int i = 0; i < otherLen; ++i) 
        {
            items->Set(currentLen + i, other->Get(i));
        }
        return this;
    }
};

//Immutable версия
template <class T>
class ImmutableArraySequence : public ArraySequenceBase<T> 
{
public:
    using ArraySequenceBase<T>::items;
    using ArraySequenceBase<T>::GetLength;

    ImmutableArraySequence(const T* items, int count) : ArraySequenceBase<T>(items, count) {}
    ImmutableArraySequence() : ArraySequenceBase<T>() {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequenceBase<T>(other) {}

    Sequence<T>* CreateSequence(const T* arr, int count) const override 
    {
        return new ImmutableArraySequence<T>(arr, count);
    }

    Sequence<T>* Append(const T& item) const 
    {
        return InsertAt(item, GetLength()); 
    }

    Sequence<T>* Prepend(const T& item) const 
    {
        return InsertAt(item, 0);
    }

    Sequence<T>* InsertAt(const T& item, int index) const 
    {
        if (index < 0 || index > GetLength()) throw IndexOutOfRangeException();
        
        T* tempArray = new T[GetLength() + 1];
        for (int i = 0; i < index; ++i) 
        {
            tempArray[i] = this->Get(i);
        }
        tempArray[index] = item;
        for (int i = index; i < GetLength(); ++i) 
        {
            tempArray[i + 1] = this->Get(i);
        }
        
        ImmutableArraySequence<T>* newSeq = new ImmutableArraySequence<T>(tempArray, GetLength() + 1);
        delete[] tempArray;
        return newSeq;
    }

    Sequence<T>* RemoveAt(int index) const 
    {
        if (index < 0 || index >= GetLength()) throw IndexOutOfRangeException();
        
        T* tempArray = new T[GetLength() - 1];
        for (int i = 0, j = 0; i < GetLength(); ++i) 
        {
            if (i != index) {
                tempArray[j++] = this->Get(i);
            }
        }
        
        ImmutableArraySequence<T>* newSeq = new ImmutableArraySequence<T>(tempArray, GetLength() - 1);
        delete[] tempArray;
        return newSeq;
    }

    Sequence<T>* Concat(Sequence<T>* other) const 
    {
        int otherLen = other->GetLength();
        T* tempArray = new T[GetLength() + otherLen];
        
        for (int i = 0; i < GetLength(); ++i) 
        {
            tempArray[i] = this->Get(i);
        }
        for (int i = 0; i < otherLen; ++i) 
        {
            tempArray[GetLength() + i] = other->Get(i);
        }
        
        ImmutableArraySequence<T>* newSeq = new ImmutableArraySequence<T>(tempArray, GetLength() + otherLen);
        delete[] tempArray;
        return newSeq;
    }

    // Переопределяем не-const версии как const
    Sequence<T>* Append(const T& item) override { return const_cast<const ImmutableArraySequence<T>*>(this)->Append(item); }
    Sequence<T>* Prepend(const T& item) override { return const_cast<const ImmutableArraySequence<T>*>(this)->Prepend(item); }
    Sequence<T>* InsertAt(const T& item, int index) override { return const_cast<const ImmutableArraySequence<T>*>(this)->InsertAt(item, index); }
    Sequence<T>* RemoveAt(int index) override { return const_cast<const ImmutableArraySequence<T>*>(this)->RemoveAt(index); }
    Sequence<T>* Concat(Sequence<T>* other) override { return const_cast<const ImmutableArraySequence<T>*>(this)->Concat(other); }
};

#endif // ARRAY_SEQUENCE_H