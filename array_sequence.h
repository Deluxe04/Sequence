#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "sequence.h"
#include "dynamic_array.h"

template <class T>
class ArraySequenceBase : public Sequence<T> 
{
private:
    template <class U>
    friend class ArraySequenceBuilder;

protected:
    DynamicArray<T>* items;

    virtual Sequence<T>* instance(const T* arr, int count) const = 0;

    //Вспом-ый метод для поиска элемента
    Option<T> FindElement(bool (*predicate)(const T&), bool fromFirst) const 
    {
        int len = GetLength();
        if (fromFirst) 
        {
            for (int i = 0; i < len; ++i) 
            {
                const T& item = this->Get(i);
                if (predicate == nullptr || predicate(item)) 
                {
                    return Option<T>(item);
                }
            }
        } 
        else 
        {
            for (int i = len - 1; i >= 0; --i) 
            {
                const T& item = this->Get(i);
                if (predicate == nullptr || predicate(item))  
                {
                    return Option<T>(item);
                }
            }
        }
        return Option<T>();
    }
    
public:
    //Конструкторы
    ArraySequenceBase(const T* items, int count) : items(new DynamicArray<T>(items, count)) {}
    ArraySequenceBase() : items(new DynamicArray<T>(0)) {}
    ArraySequenceBase(const ArraySequenceBase<T>& other) : items(new DynamicArray<T>(*other.items)) {}

    //Деструктор
    ~ArraySequenceBase() override 
    {
        delete items;
    }

    //Декомпозиция 
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
        
        Sequence<T>* subSeq = instance(tempArray, subLength);
        delete[] tempArray;
        return subSeq;
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
        if (index < 0 || index > GetLength()) 
        {
            throw IndexOutOfRangeException();
        }
        
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
        
        Sequence<T>* newSeq = instance(tempArray, GetLength() + 1);
        delete[] tempArray;
        return newSeq;
    }

    Sequence<T>* RemoveAt(int index) override 
    {
        if (index < 0 || index >= GetLength()) 
        {
            throw IndexOutOfRangeException();
        }
        
        T* tempArray = new T[GetLength() - 1];
        for (int i = 0, j = 0; i < GetLength(); ++i) 
        {
            if (i != index) 
            {
                tempArray[j++] = this->Get(i);
            }
        }
        
        Sequence<T>* newSeq = instance(tempArray, GetLength() - 1);
        delete[] tempArray;
        return newSeq;
    }

    Sequence<T>* Concat(Sequence<T>* other) override 
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
        
        Sequence<T>* newSeq = instance(tempArray, GetLength() + otherLen);
        delete[] tempArray;
        return newSeq;
    }
    
    const T& operator[](int index) const 
    {
        return items->Get(index);
    }

    Sequence<T>* Map(T (*f)(const T&)) const override 
    {
        int len = GetLength();
        T* tempArray = new T[len];
        for (int i = 0; i < len; ++i) 
        {
            tempArray[i] = f(this->Get(i));
        }
        Sequence<T>* result = instance(tempArray, len);
        delete[] tempArray;
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const override 
    {
        int len = GetLength();
        int count = 0;
        for (int i = 0; i < len; ++i) 
        {
            if (predicate == nullptr || predicate(this->Get(i))) ++count;
        }
        
        T* tempArray = nullptr;
        if (count > 0) 
        {
            tempArray = new T[count];
            int j = 0;
            for (int i = 0; i < len; ++i) 
            {
                const T& item = this->Get(i);
                if (predicate(item)) 
                {
                    tempArray[j++] = item;
                }
            }
        }
    
        Sequence<T>* result = instance(tempArray, count);
        if (tempArray) delete[] tempArray;
        return result;
    }

    T Reduce(T (*func)(T, const T&), T initial) const override 
    {
        T result = initial;
        int len = GetLength();
        for (int i = 0; i < len; ++i) 
        {
            result = func(result, this->Get(i));
        }
        return result;
    }

    Option<T> GetFirst(bool (*predicate)(const T&)) const override 
    {
        return FindElement(predicate, true);
    }

    Option<T> GetLast(bool (*predicate)(const T&)) const override 
    {
        return FindElement(predicate, false);
    }

    IEnumerator<T>* GetEnumerator() const override 
    {
    return items->GetEnumerator();
    }
};

//Mutable версия 
template <class T>
class MutableArraySequence : public ArraySequenceBase<T> 
{
protected:
    using ArraySequenceBase<T>::items;

    Sequence<T>* instance(const T* arr, int count) const override 
    {
        return new MutableArraySequence<T>(arr, count);
    }

public:
    using ArraySequenceBase<T>::GetLength;
    MutableArraySequence(const T* items, int count) : ArraySequenceBase<T>(items, count) {}
    MutableArraySequence() : ArraySequenceBase<T>() {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequenceBase<T>(other) {}

    //Переопределяем методы для модификации текущего объекта
    Sequence<T>* InsertAt(const T& item, int index) override 
    {
        if (index < 0 || index > GetLength()) 
        {
            throw IndexOutOfRangeException();
        }
        
        items->Resize(GetLength() + 1);
        for (int i = GetLength() - 1; i > index; --i) 
        {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        return this;
    }

    Sequence<T>* RemoveAt(int index) override 
    {
        if (index < 0 || index >= GetLength()) 
        {
            throw IndexOutOfRangeException();
        }
        
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

    T& operator[](int index) 
    {
        return const_cast<T&>(items->Get(index));
    }
};

//Immutable версия
template <class T>
class ImmutableArraySequence : public ArraySequenceBase<T> 
{
protected:
    
    Sequence<T>* instance(const T* arr, int count) const override 
    {
        return new ImmutableArraySequence<T>(arr, count);
    }

public:
    ImmutableArraySequence(const T* items, int count) : ArraySequenceBase<T>(items, count) {}
    ImmutableArraySequence() : ArraySequenceBase<T>() {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequenceBase<T>(other) {}
};

#endif // ARRAY_SEQUENCE_H
