#ifndef LIST_SEQUENCE_H
#define LIST_SEQUENCE_H

#include "sequence.h"
#include "linked_list.h"

template <class T>
class ListSequenceBase : public Sequence<T> 
{
private:
    template <class U>
    friend class ListSequenceBuilder;

protected:
    LinkedList<T>* list;

    virtual Sequence<T>* instance(const T* arr, int count) const = 0;
    
    //Вспом-ный метод для преобразования списка в массив
    T* ListToArray() const 
    {
        T* arr = new T[GetLength()];
        for (int i = 0; i < GetLength(); ++i) 
        {
            arr[i] = list->Get(i);
        }
        return arr;
    }

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
                if (predicate == nullptr|| predicate(item)) 
                {
                    return Option<T>(item);
                }
            }
        }
        return Option<T>();
    }

public:
    //Конструкторы
    ListSequenceBase(const T* items, int count) : list(new LinkedList<T>(items, count)) {}
    ListSequenceBase() : list(new LinkedList<T>()) {}
    ListSequenceBase(const ListSequenceBase<T>& other) : list(new LinkedList<T>(*other.list)) {}

    //Деструктор
    ~ListSequenceBase() override 
    {
        delete list;
    }

    const T& GetFirst() const override 
    {
        return list->GetFirst();
    }

    const T& GetLast() const override 
    {
        return list->GetLast();
    }

    const T& Get(int index) const override 
    {
        return list->Get(index);
    }

    int GetLength() const override 
    {
        return list->GetLength();
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

    //Базовые операции 
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
        
        T* tempArray = ListToArray();
        T* newArray = new T[GetLength() + 1];
        
        for (int i = 0; i < index; ++i) 
        {
            newArray[i] = tempArray[i];
        }
        newArray[index] = item;
        for (int i = index; i < GetLength(); ++i) 
        {
            newArray[i + 1] = tempArray[i];
        }
        
        Sequence<T>* newSeq = instance(newArray, GetLength() + 1);
        delete[] tempArray;
        delete[] newArray;
        return newSeq;
    }

    Sequence<T>* RemoveAt(int index) override 
    {
        if (index < 0 || index >= GetLength()) 
        {
            throw IndexOutOfRangeException();
        }
        
        T* tempArray = ListToArray();
        T* newArray = new T[GetLength() - 1];
        
        for (int i = 0, j = 0; i < GetLength(); ++i) 
        {
            if (i != index) 
            {
                newArray[j++] = tempArray[i];
            }
        }
        
        Sequence<T>* newSeq = instance(newArray, GetLength() - 1);
        delete[] tempArray;
        delete[] newArray;
        return newSeq;
    }

    Sequence<T>* Concat(Sequence<T>* other) override 
    {
        T* tempArray = ListToArray();
        int otherLen = other->GetLength();
        T* newArray = new T[GetLength() + otherLen];
        
        for (int i = 0; i < GetLength(); ++i) 
        {
            newArray[i] = tempArray[i];
        }
        for (int i = 0; i < otherLen; ++i) 
        {
            newArray[GetLength() + i] = other->Get(i);
        }
        
        Sequence<T>* newSeq = instance(newArray, GetLength() + otherLen);
        delete[] tempArray;
        delete[] newArray;
        return newSeq;
    }
    
    const T& operator[](int index) const 
    {
    return list->Get(index);
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
        if (predicate == nullptr) 
        {
            T* arr = new T[len];
            for (int i = 0; i < len; ++i)
            {
                arr[i] = this->Get(i);
            }
            Sequence<T>* result = instance(arr, len);
            delete[] arr;
            return result;
        }
        
        int count = 0;
        for (int i = 0; i < len; ++i) 
        {
            if (predicate(this->Get(i))) ++count;
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
    return list->GetEnumerator();
    }
};

//Mutable версия
template <class T>
class MutableListSequence : public ListSequenceBase<T> 
{
protected:
    using ListSequenceBase<T>::list;
    Sequence<T>* instance(const T* arr, int count) const override 
    {
        return new MutableListSequence<T>(arr, count);
    }

public:
    using ListSequenceBase<T>::GetLength;
    //Конструкторы
    MutableListSequence(const T* items, int count) : ListSequenceBase<T>(items, count) {}
    MutableListSequence() : ListSequenceBase<T>() {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequenceBase<T>(other) {}

    //Переопределяем методы для модификации текущего объекта
    Sequence<T>* InsertAt(const T& item, int index) override 
    {
        if (index < 0 || index > GetLength()) 
        {
            throw IndexOutOfRangeException();
        }
        list->InsertAt(item, index);
        return this;
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
                tempArray[j++] = list->Get(i);
            }
        }
        
        LinkedList<T>* newList = new LinkedList<T>(tempArray, GetLength() - 1);
        delete[] tempArray;
        delete list;
        list = newList;
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* other) override 
    {
        for (int i = 0; i < other->GetLength(); ++i) 
        {
            list->Append(other->Get(i));
        }
        return this;
    }

    T& operator[](int index) 
    {
    return const_cast<T&>(this->list->Get(index));
    }
};

//Immutable версия
template <class T>
class ImmutableListSequence : public ListSequenceBase<T> 
{
protected:
    Sequence<T>* instance(const T* arr, int count) const override 
    {
        return new ImmutableListSequence<T>(arr, count);
    }

public:
    //Конструкторы
    ImmutableListSequence(const T* items, int count) : ListSequenceBase<T>(items, count) {}
    ImmutableListSequence() : ListSequenceBase<T>() {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequenceBase<T>(other) {}
};

#endif // LIST_SEQUENCE_H
